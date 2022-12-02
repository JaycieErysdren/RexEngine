// ========================================================
//
// FILE:			/source/core/picture.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Picture namespace header
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

//
//
// Picture namespace
//
//

// Picture namespace definition (public)
namespace Picture
{
	//
	//
	// Types
	//
	//

	typedef uint8_t clut_t[256][256];
	typedef uint8_t palette_t[256][3];
	typedef uint8_t lightmap_t[32][256];
	typedef uint8_t texture66_t[64][64];

	typedef enum
	{
		COPY, COLORKEY
	} pic_blit_mode;

	// Scanlines union
	typedef union
	{
		uint8_t **b;
		uint16_t **w;
		uint32_t **l;
	} scanlines_u;

	// Picture type
	typedef struct
	{
		int32_t width;				// width of picture in pixels
		int32_t height;				// height of picture in pixels
		int32_t bpp;				// bits per pixel
		int32_t bytes_per_row;		// bytes per row. rows are 4 byte aligned
		int32_t shared;				// false if buffer is owned. TODO: reference counting
		void *buffer;				// allocated pixel buffer
		scanlines_u scanlines;		// pre-calculated scanline pointers
	} pic_t;

	//
	//
	// Functions
	//
	//

	void Create(pic_t *picture, int width, int height, int bpp, int bytes_per_row, void* buffer);
	void CreateMip(pic_t *dst, pic_t *src, Palette::clut_t blender);
	void Destroy(pic_t *picture);
	void Resize(pic_t *picture, int width, int height);
	void Clear(pic_t *picture, uint8_t color);
	void Copy(pic_t *dst, pic_t *src);
	void LoadBMP(pic_t *picture, string filename);
	void Save(pic_t *picture, string filename);
	void AssertSame(pic_t *dst, pic_t *src);
	void Flip8(pic_t *dst, pic_t *src);
	void Draw8(pic_t *dst, pic_t *src, int x, int y, pic_blit_mode flags);
	void Blit8(pic_t *dst, int x1, int y1, int x2, int y2, pic_t *src, int u1, int v1, int u2, int v2, pic_blit_mode mode);
	void LiquidEffect8(pic_t *dst, pic_t *src, int tick);
	void Blend8(pic_t *dst, pic_t *src1, pic_t *src2, Palette::clut_t blender);

	void DrawPixel(pic_t *dst, int x, int y, uint8_t color);
	void DrawLine(pic_t *dst, int x1, int y1, int x2, int y2, uint8_t color);
	void DrawHorizontalLine(pic_t *dst, int x1, int x2, int y, uint8_t color);
	void DrawRectangle(pic_t *dst, int x, int y, int w, int h, uint8_t color, bool filled);
	void DrawPolygon(pic_t *dst, int n, int *v, uint8_t color, bool filled);

	void FloodFill(pic_t *dst, int x, int y, uint8_t color);
	uint8_t GetPixel(pic_t *src, int x, int y);
}
