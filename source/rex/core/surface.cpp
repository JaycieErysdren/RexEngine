// ========================================================
//
// FILE:			/source/rex/modules/core/io/surface.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Surface implementation
//
// LAST EDITED:		December 14th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Surface
//
//

namespace Rex
{

//
//
// Functions
//
//

//
// Front buffer
//

void PlaceBuffer(void *buffer, rex_int32 buffer_size)
{
	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_VESA)

		VESA::PlaceBuffer((rex_uint8 *)buffer, buffer_size);

	#endif
}

void SurfaceToFrontBuffer(Surface *src)
{
	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_VESA)

		VESA::PlaceBuffer((rex_uint8 *)src->buffer, vid_info.width * vid_info.height * (vid_info.bpp / 8));

	#endif
}

//
// Picture creation / destruction
//

// creates a picture. allocates required pixel buffer and pre-calculates scanline pointers.
void SurfaceCreate(Surface *picture, int width, int height, int bpp, int bytes_per_row, void *buffer)
{
	memset(picture, 0, sizeof(Surface));

	if (bpp <= 0) bpp = 8; // default to 8 bpp.

	if (bytes_per_row <= 0) bytes_per_row = (bpp * width + 31) >> 5 << 2;

	// Set default values
	picture->width				= width;
	picture->height				= height;
	picture->bpp				= bpp;
	picture->bytes_per_row		= bytes_per_row;
	picture->buffer				= buffer ? buffer : calloc(height, bytes_per_row);
	//picture->buffer			= buffer ? buffer : MemPool_Alloc(MEMORY_SURFACES, height * bytes_per_row);
	picture->shared				= buffer != 0;
	picture->scanlines.b		= (rex_uint8 **)malloc(height * sizeof(void *));
	//picture->scanlines.b		= (rex_uint8 **)MemPool_Alloc(MEMORY_SURFACES, height * sizeof(void *));

	#if (REX_TARGET == PLATFORM_DOS)

		switch(bpp)
		{
			case 8:
				while (height--) picture->scanlines.b[height] = (rex_uint8 *)((rex_uint32)picture->buffer + bytes_per_row * height);
				break;

			case 16:
				while (height--) picture->scanlines.w[height] = (rex_uint16 *)((rex_uint32)picture->buffer + bytes_per_row * height);
				break;

			case 24:
			case 32:
				while (height--) picture->scanlines.l[height] = (rex_uint32 *)((rex_uint32)picture->buffer + bytes_per_row * height);
				break;

			default:
				break;
		}

	#else

		switch(bpp)
		{
			case 8:
				while (height--) picture->scanlines.b[height] = (rex_uint8 *)((rex_uint32 *)picture->buffer + bytes_per_row * height);
				break;

			case 16:
				while (height--) picture->scanlines.w[height] = (rex_uint16 *)((rex_uint32 *)picture->buffer + bytes_per_row * height);
				break;

			case 24:
			case 32:
				while (height--) picture->scanlines.l[height] = (rex_uint32 *)((rex_uint32 *)picture->buffer + bytes_per_row * height);
				break;

			default:
				break;
		}

	#endif
}

void SurfaceCreateMip(Surface *dst, Surface *src, clut_t blender)
{
	int x, y;

	SurfaceCreate(dst, src->width, src->height, src->bpp, 0, 0);

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
void SurfaceDestroy(Surface *picture)
{
	//MemPool_Free(MEMORY_SURFACES, picture->buffer);
	//MemPool_Free(MEMORY_SURFACES, picture->scanlines.b);
	//MemPool_Free(MEMORY_SURFACES, picture->scanlines.w);
	//MemPool_Free(MEMORY_SURFACES, picture->scanlines.l);
	assert(picture);
	if (!picture->shared) free(picture->buffer);
	free(picture->scanlines.b);
}

// resizes a pictures buffer. does not maintain picture contents.
void SurfaceResize(Surface *picture, int width, int height)
{
	if (picture->width != width || picture->height != height)
	{
		int bpp = picture->bpp;
		SurfaceDestroy(picture);
		SurfaceCreate(picture, width, height, bpp, 0, 0);
	}
}

// A fast picture content clear.
void SurfaceClear(Surface *picture, rex_uint8 color)
{
	memset(picture->buffer, color, picture->bytes_per_row * picture->height);
}

// A fast picture content copy. s must be compatible.
void SurfaceCopy(Surface *dst, Surface *src)
{
	memcpy(dst->buffer, src->buffer, dst->bytes_per_row * dst->height);
}

void SurfaceLoadBMP(Surface *picture, string filename)
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

	fseek(fp, 16, SEEK_CUR);
	fread(&width, sizeof(uint16_t), 1, fp);
	fseek(fp, 2, SEEK_CUR);
	fread(&height, sizeof(uint16_t), 1, fp);
	fseek(fp, 22, SEEK_CUR);
	fread(&num_palette_colors, sizeof(uint16_t), 1, fp);

	SurfaceCreate(picture, width, height, 0, 0, 0);

	fseek(fp, 6, SEEK_CUR);
	fseek(fp, num_palette_colors * 4, SEEK_CUR);

	fread(picture->buffer, height, picture->bytes_per_row, fp);

	fclose(fp);
}

void SurfaceSave(Surface *picture, string filename)
{
	assert(1);
}

void SurfaceAssertSame(Surface *dst, Surface *src)
{
	assert(dst && src);
	assert(dst->width == src->width);
	assert(dst->height == src->height);
	assert(dst->bpp == src->bpp);
}

void SurfaceFlip8(Surface *dst, Surface *src)
{
	int x, y1, y2;

	SurfaceAssertSame(dst, src);

	for (y1 = 0, y2 = dst->height - 1; y1 < y2; y1++, y2--)
	{
		rex_uint8 *s1 = src->scanlines.b[y1];
		rex_uint8 *s2 = src->scanlines.b[y2];
		rex_uint8 *d1 = dst->scanlines.b[y1];
		rex_uint8 *d2 = dst->scanlines.b[y2];

		for (x = dst->width; x--;)
		{
			rex_uint8 tmp = *s1++;
			*d1++ = *s2++;
			*d2++ = tmp;
		}
	}
}

void SurfaceDraw8(Surface *dst, Surface *src, int x, int y, blit_mode flags)
{
	SurfaceBlit8(dst, x, y, x + src->width, y + src->height, src, 0, 0, src->width, src->height, flags);
}

void SurfaceBlit8(Surface *dst, int x1, int y1, int x2, int y2, Surface *src, int u1, int v1, int u2, int v2, blit_mode mode)
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
		rex_uint8 *I = src->scanlines.b[f2i(v1)]; \
		rex_uint8 *O = &dst->scanlines.b[y1][x1]; \
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
			rex_uint8 pen;
			#define AFFINE if (pen = I[f2i(u)]) *O = pen;
			INNER_LOOP
			#undef AFFINE
		}
	}
}

void SurfaceLiquidEffect8(Surface *dst, Surface *src, int tick)
{
	int y;
	for (y = dst->height; y--;)
	{
		int x = fixsin(y * 32 + tick * 5) >> 13;

		SurfaceBlit8(dst, x, y, x + dst->width, y + 1, src, 0, y, src->width, y + 1, COPY);
		SurfaceBlit8(dst, x - dst->width, y, x, y + 1, src, 0, y, src->width, y + 1, COPY);
		SurfaceBlit8(dst, x + dst->width, y, x + dst->width + dst->width, y + 1, src, 0, y, src->width, y + 1, COPY);
	}
}

void SurfaceBlend8(Surface *dst, Surface *src1, Surface *src2, clut_t blender)
{
	int x, y;

	for (y = dst->height; y--;)
	{
		rex_uint8 *a = dst ->scanlines.b[y];
		rex_uint8 *b = src1->scanlines.b[y];
		rex_uint8 *c = src2->scanlines.b[y];

		for (x = dst->width; x--;)
		{
			*a++ = blender[*b++][*c++];
		}
	}
}

// Get the color of a pixel from the specified x and y coordinate
rex_uint8 SurfaceGetPixel(Surface *pic, int x, int y)
{
	if (x < 0 || y < 0 || x > pic->width || y > pic->height)
	{
		return 0;
	}
	else
	{
		switch (pic->bpp)
		{
			case 8:
				return pic->scanlines.b[y][x];

			case 16:
				return pic->scanlines.w[y][x];

			case 32:
				return pic->scanlines.l[y][x];

			default:
				return 0;
		}
	}
}

// Plot a pixel at the specificed x and y coordinate
void SurfaceDrawPixel(Surface *dst, int x, int y, rex_uint8 color)
{
	if (x >= dst->width || x < 0 || y >= dst->height || y < 0)
		return;

	memset(&dst->scanlines.b[y][x], color, sizeof(rex_uint8));
}

void SurfaceDrawLine(Surface *dst, int x1, int y1, int x2, int y2, rex_uint8 color)
{
	int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

	dx = x2 - x1; // the horizontal distance of the line
	dy = y2 - y1; // the vertical distance of the line
	dxabs = ABS(dx);
	dyabs = ABS(dy);
	sdx = SGN(dx);
	sdy = SGN(dy);
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;

	if (px > -1 && px < dst->width && py > -1 && py < dst->height)
		SurfaceDrawPixel(dst, px, py, color);

	if (dxabs >= dyabs) // the line is more horizontal than vertical
	{
		for (i = 0; i < dxabs; i++)
		{
			y += dyabs;

			if (y >= dxabs)
			{
				y -= dxabs;
				py += sdy;
			}

			px += sdx;

			if (px > -1 && px < dst->width && py > -1 && py < dst->height)
				SurfaceDrawPixel(dst, px, py, color);
		}
	}
	else // the line is more vertical than horizontal
	{
		for (i = 0; i < dyabs; i++)
		{
			x += dxabs;

			if (x >= dyabs)
			{
				x -= dyabs;
				px += sdx;
			}

			py += sdy;

			if (px > -1 && px < dst->width && py > -1 && py < dst->height)
				SurfaceDrawPixel(dst, px, py, color);
		}
	}
}

// Set a horizontal line from a buffer
void SurfaceSetHorizontalLine(Surface *dst, rex_int x, rex_int y, rex_int width, void *buffer)
{
	memcpy(&dst->scanlines.b[y][x], buffer, width * (dst->bpp / 8));
}

// Draw a horizontal line
void SurfaceDrawHorizontalLine(Surface *dst, int x1, int x2, int y, rex_uint8 color)
{
	if (x1 > x2)
		memset(&dst->scanlines.b[y][x2], color, x1 - x2);
	else
		memset(&dst->scanlines.b[y][x1], color, x2 - x1);
}

// Draw a vertical line
void SurfaceDrawVerticalLine(Surface *dst, int x, int y1, int y2, rex_uint8 color)
{
	int i;
	if (y1 > y2)
	{
		for (i = y2; i < y1; i++)
		{
			memset(&dst->scanlines.b[i][x], color, sizeof(rex_uint8));
		}
	}
	else
	{
		for (i = y1; i < y2; i++)
		{
			memset(&dst->scanlines.b[i][x], color, sizeof(rex_uint8));
		}
	}
}

// Draw a filled or outlined rectangle
void SurfaceDrawRectangle(Surface *dst, int x, int y, int w, int h, rex_uint8 color, bool filled)
{
	// Sanity check
	if (x < 0 || y < 0 || x > dst->width || y > dst->height || (x + w) > dst->width || (y + h) > dst->height)
		return;

	if (filled == true)
	{
		// fill it in with horizontal lines
		for (int i = 0; i < h; i++)
		{
			memset(&dst->scanlines.b[y + i][x], color, w);
		}
	}
	else
	{
		// draw horizontal lines
		memset(&dst->scanlines.b[y][x], color, w - 1);
		memset(&dst->scanlines.b[y + h - 1][x], color, w - 1);

		// draw vertical lines
		for (int i = 0; i < h; i++)
		{
			memset(&dst->scanlines.b[y + i][x], color, sizeof(rex_uint8));
			memset(&dst->scanlines.b[y + i][x + w - 1], color, sizeof(rex_uint8));
		}
	}
}

// Flood fill from the specified point
void SurfaceFloodFill(Surface *dst, int x, int y, rex_uint8 color)
{
	if (SurfaceGetPixel(dst, x, y) != color)
	{
		SurfaceDrawPixel(dst, x, y, color);
		SurfaceFloodFill(dst, x + 1, y, color);
		SurfaceFloodFill(dst, x, y + 1, color);
		SurfaceFloodFill(dst, x - 1, y, color);
		SurfaceFloodFill(dst, x, y - 1, color);
	}
}

// Draw a polygon with n sides
void SurfaceDrawPolygon(Surface *dst, int n, int *v, rex_uint8 color, bool filled)
{
	if (filled == true)
	{

	}
	else
	{
		// Draw lines
		for (int i = 0; i < n - 1; i++)
		{
			SurfaceDrawLine(dst, v[(i << 1) + 0], v[(i << 1) + 1], v[(i << 1) + 2], v[(i << 1) + 3], color);
		}

		// Draw last line
		SurfaceDrawLine(dst, v[0], v[1], v[(n << 1) - 2], v[(n << 1) - 1], color);
	}
}

} // namespace Rex
