// ========================================================
//
// FILE:			/source/core/picture.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Picture namespace implementation
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

// Picture namespace (private)
namespace Picture
{

//
//
// Functions
//
//

// creates a picture. allocates required pixel buffer and pre-calculates scanline pointers.
void Create(pic_t *picture, int width, int height, int bpp, int bytes_per_row, void *buffer)
{
	memset(picture, 0, sizeof(pic_t));

	if (bpp <= 0) bpp = 8; // default to 8 bpp.

	if (bytes_per_row <= 0) bytes_per_row = (bpp * width + 31) >> 5 << 2;

	// Set default values
	picture->width				= width;
	picture->height				= height;
	picture->bpp				= bpp;
	picture->bytes_per_row		= bytes_per_row;
	picture->buffer				= buffer ? buffer : calloc(height, bytes_per_row);
	picture->shared				= buffer != 0;
	picture->scanlines.b		= (uint8_t **)malloc(height * sizeof(void *));

	while (height--) picture->scanlines.b[height] = (uint8_t *)((uint32_t)picture->buffer + bytes_per_row * height);
}

void CreateMip(pic_t *dst, pic_t *src, clut_t blender)
{
	int x, y;

	Create(dst, src->width, src->height, src->bpp, 0, 0);

	for (y = 0; y < dst->height; y += 2)
	{
		for (x = 0; x < dst->width; x += 2)
		{
			int a = blender[src->scanlines.b[y + 0][x + 0]][src->scanlines.b[y + 0][x + 1]];
			int b = blender[src->scanlines.b[y + 1][x + 0]][src->scanlines.b[y + 1][x + 1]];
			int c = blender[a][b];

			dst->scanlines.b[y][x] = c;
			dst->scanlines.b[y][x + 1] = c;
			dst->scanlines.b[y + 1][x] = c;
			dst->scanlines.b[y + 1][x + 1] = c;
		}
	}
}

// destroys the resources allocated to a picture.
void Destroy(pic_t *picture)
{
	assert(picture);
	if (!picture->shared) free(picture->buffer);
	free(picture->scanlines.b);
}

// resizes a pictures buffer. does not maintain picture contents.
void Resize(pic_t *picture, int width, int height)
{
	if (picture->width != width || picture->height != height)
	{
		int bpp = picture->bpp;
		Destroy(picture);
		Create(picture, width, height, bpp, 0, 0);
	}
}

// A fast picture content clear.
void Clear(pic_t *picture, uint8_t color)
{
	memset(picture->buffer, color, picture->bytes_per_row * picture->height);
}

// A fast picture content copy. s must be compatible.
void Copy(pic_t *dst, pic_t *src)
{
	memcpy(dst->buffer, src->buffer, dst->bytes_per_row * dst->height);
}

void Load(pic_t *picture, string filename)
{
	FILE *fp;
	uint16_t width, height;
	uint16_t num_palette_colors;

	fp = fopen(filename.c_str(), "rb");

	if (fp == NULL)
		return;

	if (fgetc(fp) != 'B' || fgetc(fp) != 'M')
	{
		fclose(fp);
		assert(1);
		//fail("%s is not a bitmap file", filename);
	}

	Utils::FileSkip(fp, 16);
	fread(&width, sizeof(uint16_t), 1, fp);
	Utils::FileSkip(fp, 2);
	fread(&height, sizeof(uint16_t), 1, fp);
	Utils::FileSkip(fp, 22);
	fread(&num_palette_colors, sizeof(uint16_t), 1, fp);

	Create(picture, width, height, 0, 0, 0);

	Utils::FileSkip(fp, 6);
	Utils::FileSkip(fp, num_palette_colors * 4);

	fread(picture->buffer, height, picture->bytes_per_row, fp);

	fclose(fp);
}

void Save(pic_t *picture, string filename)
{
	assert(1);
}

void AssertSame(pic_t *dst, pic_t *src)
{
	assert(dst && src);
	assert(dst->width == src->width);
	assert(dst->height == src->height);
	assert(dst->bpp == src->bpp);
}

void Flip8(pic_t *dst, pic_t *src)
{
	int x, y1, y2;

	AssertSame(dst, src);

	for (y1 = 0, y2 = dst->height - 1; y1 < y2; y1++, y2--)
	{
		uint8_t *s1 = src->scanlines.b[y1];
		uint8_t *s2 = src->scanlines.b[y2];
		uint8_t *d1 = dst->scanlines.b[y1];
		uint8_t *d2 = dst->scanlines.b[y2];

		for (x = dst->width; x--;)
		{
			uint8_t tmp = *s1++;
			*d1++ = *s2++;
			*d2++ = tmp;
		}
	}
}

void Draw8(pic_t *dst, pic_t *src, int x, int y, pic_blit_mode flags)
{
	Blit8(dst, x, y, x + src->width, y + src->height, src, 0, 0, src->width, src->height, flags);
}

void Blit8(pic_t *dst, int x1, int y1, int x2, int y2, pic_t *src, int u1, int v1, int u2, int v2, pic_blit_mode mode)
{
	int w, h, u, uu, vv;

	if ((w = x2 - x1) <= 0 || (h = y2 - y1) <= 0) return;

	u1 = i2f(u1);
	v1 = i2f(v1);
	u2 = i2f(u2);
	v2 = i2f(v2);

	uu = (u2 - u1) / w;
	vv = (v2 - v1) / h;

	if (x1 < 0) {u1 -= x1 * uu; x1 = 0;}
	if (y1 < 0) {v1 -= y1 * vv; y1 = 0;}
	if (x2 > dst->width ) {x2  = dst->width;}
	if (y2 > dst->height) {y2  = dst->height;}

	if ((w = x2 - x1) <= 0 || (h = y2 - y1) <= 0) return;

	#define INNER_LOOP \
	for (;y1 < y2; y1++, v1 += vv) \
	{ \
		uint8_t *I = src->scanlines.b[f2i(v1)]; \
		uint8_t *O = &dst->scanlines.b[y1][x1]; \
		for(w = x2 - x1, u = u1; w--; u += uu, O++) {AFFINE} \
	}

	 switch (mode)
	 {
		case COPY:
		{
			if (uu == i2f(1))
			{
				// High speed picture blitter.
				for (;y1 < y2; y1++, v1 += vv)
				{
					memcpy(&dst->scanlines.b[y1][x1], &src->scanlines.b[f2i(v1)][f2i(u1)], x2 - x1);
				}
			}
			else
			{
				#define AFFINE *O = I[f2i(u)];
				INNER_LOOP
				#undef AFFINE
			}
		}
		case COLORKEY:
		{
			uint8_t pen;
			#define AFFINE if (pen = I[f2i(u)]) *O = pen;
			INNER_LOOP
			#undef AFFINE
		}
	}
}

void LiquidEffect8(pic_t *dst, pic_t *src, int tick)
{
	int y;
	for (y = dst->height; y--;)
	{
		int x = fixsin(y * 32 + tick * 5) >> 13;

		Blit8(dst, x, y, x + dst->width, y + 1, src, 0, y, src->width, y + 1, COPY);
		Blit8(dst, x - dst->width, y, x, y + 1, src, 0, y, src->width, y + 1, COPY);
		Blit8(dst, x + dst->width, y, x + dst->width + dst->width, y + 1, src, 0, y, src->width, y + 1, COPY);
	}
}

void Blend8(pic_t *dst, pic_t *src1, pic_t *src2, clut_t blender)
{
	int x, y;

	for (y = dst->height; y--;)
	{
		uint8_t *a = dst ->scanlines.b[y];
		uint8_t *b = src1->scanlines.b[y];
		uint8_t *c = src2->scanlines.b[y];

		for (x = dst->width; x--;)
		{
			*a++ = blender[*b++][*c++];
		}
	}
}

} // namespace Picture
