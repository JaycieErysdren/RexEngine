// ========================================================
//
// FILE:			/source/rex/modules/core/io/surface.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Surface header
//
// LAST EDITED:		December 14th, 2022
//
// ========================================================

#ifndef __REX_SURFACE_H__
#define __REX_SURFACE_H__

//
//
// Rex namespace: Graphics
//
//

namespace Rex
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
} blit_mode;

// Scanlines union
typedef union
{
	uint8_t **b;
	uint16_t **w;
	uint32_t **l;
} scanlines_u;

//
//
// Classes
//
//

class Surface
{
	public:
		int32_t width;				// width of picture in pixels
		int32_t height;				// height of picture in pixels
		int32_t bpp;				// bits per pixel
		int32_t bytes_per_row;		// bytes per row. rows are 4 byte aligned
		int32_t shared;				// false if buffer is owned. TODO: reference counting
		void *buffer;				// allocated pixel buffer
		scanlines_u scanlines;		// pre-calculated scanline pointers
};

//
//
// Function prototypes
//
//

//
// Front buffer
//

void SurfaceToFrontBuffer(Surface *src);

//
// Creation, destruction, etc
//

void SurfaceCreate(Surface *picture, int width, int height, int bpp, int bytes_per_row, void* buffer);
void SurfaceCreateMip(Surface *dst, Surface *src, clut_t blender);
void SurfaceDestroy(Surface *picture);
void SurfaceResize(Surface *picture, int width, int height);
void SurfaceClear(Surface *picture, uint8_t color);
void SurfaceCopy(Surface *dst, Surface *src);
void SurfaceLoadBMP(Surface *picture, string filename);
void SurfaceSave(Surface *picture, string filename);
void SurfaceAssertSame(Surface *dst, Surface *src);
void SurfaceFlip8(Surface *dst, Surface *src);
void SurfaceDraw8(Surface *dst, Surface *src, int x, int y, blit_mode flags);
void SurfaceBlit8(Surface *dst, int x1, int y1, int x2, int y2, Surface *src, int u1, int v1, int u2, int v2, blit_mode mode);
void SurfaceLiquidEffect8(Surface *dst, Surface *src, int tick);
void SurfaceBlend8(Surface *dst, Surface *src1, Surface *src2, clut_t blender);

// Set a horizontal line from a buffer
void SurfaceSetHorizontalLine(Surface *dst, rex_int x, rex_int y, rex_int width, void *buffer);

void SurfaceDrawPixel(Surface *dst, int x, int y, uint8_t color);
void SurfaceDrawLine(Surface *dst, int x1, int y1, int x2, int y2, uint8_t color);
void SurfaceDrawHorizontalLine(Surface *dst, int x1, int x2, int y, uint8_t color);
void SurfaceDrawVerticalLine(Surface *dst, int x, int y1, int y2, uint8_t color);
void SurfaceDrawRectangle(Surface *dst, int x, int y, int w, int h, uint8_t color, bool filled);
void SurfaceDrawPolygon(Surface *dst, int n, int *v, uint8_t color, bool filled);

void SurfaceFloodFill(Surface *dst, int x, int y, uint8_t color);
uint8_t SurfaceGetPixel(Surface *src, int x, int y);

} // namespace Rex

#endif // __REX_SURFACE_H__
