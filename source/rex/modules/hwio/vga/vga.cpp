// ========================================================
//
// FILE:			/source/rex/modules/hwio/vga/vga.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		VGA namespace implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine header
#include "rex.hpp"

// VGA text macros
#define SLOWPOS(x, y)		(((y) * VGA_MODE13H_WIDTH) + (x))
#define FASTPOS(x, y)		(((y) << 8) + ((y) << 6) + (x))

//
//
// VGA namespace
//
//

// VGA namespace definition (private)
namespace VGA
{

//
//
// Global variables
//
//

uint8_t *buffer_front = (uint8_t *)VGA_VIDMEM_PTR;

//
//
// Functions
//
//

//
// Bootstrap
//

// Initialize the VGA driver with the specified width, height and bpp
bool Initialize(int w, int h, int bpp)
{
	int8_t mode;

	if (w == 320 && h == 200 && bpp == 8)
		mode = 0x13;
	else
		return false;

	// Set VGA mode 0x13
	union REGS r;
	r.h.ah = 0x00;
	r.h.al = mode;

	#if (REX_COMPILER == COMPILER_DJGPP)

		int86(0x10, &r, &r);

	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)

		int386(0x10, &r, &r);

	#endif

	return true;
}

// Shutdown
void Shutdown()
{
	union REGS r;

	#if (REX_COMPILER == COMPILER_DJGPP)

		// Set mode 0x03
		r.x.ax = 0x03;
		int86(0x10, &r, &r);

	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)

		// Set mode 0x03
		r.w.ax = 0x03;
		int386(0x10, &r, &r);

	#endif
}

//
// Utilities
//

// Wait for vertical retrace ("vsync")
void WaitForRetrace()
{
	#if (REX_COMPILER == COMPILER_DJGPP)

		while (inportb(0x3da) & 0x08);
		while (!(inportb(0x3da) & 0x08));

	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)

		while (inp(0x3da) & 0x08);
		while (!(inp(0x3da) & 0x08));

	#endif
}

//
// Palette
//

// Set a color in the palette
void SetPaletteColor(uint8_t i, uint8_t r, uint8_t g, uint8_t b)
{
	// Output the index of the colour
	outp(0x3c8, i);

	// stupid
	outp(0x3c9, (r * 63) / 255);
	outp(0x3c9, (g * 63) / 255);
	outp(0x3c9, (b * 63) / 255);
}

// Set the palette from a file
void SetPalette(string filename)
{
	int i;
	FILE *file;

	file = fopen(filename.c_str(), "rb");

	// Tell VGA that palette data is coming
	outp(0x3c8, 0);

	// Install the palette
	for (i = 0; i < 256; i++)
	{
		unsigned char r = getc(file);
		unsigned char g = getc(file);
		unsigned char b = getc(file);

		// stupid
		outp(0x3c9, (r * 63) / 255);
		outp(0x3c9, (g * 63) / 255);
		outp(0x3c9, (b * 63) / 255);
	}

	fclose(file);
}

// Draw the palette on the back buffer
void DrawPalette()
{
	int i;
	int left, top, right, bottom;

	for(i = 0; i < 256; i++)
	{
		left = (i % 16) * 20;
		top = (i / 16) * 12;
		right = left + 20;
		bottom = top + 12;
		DrawRectangleFilled(left, top, right, bottom, i);
	}
}

//
// Pixel
//

// Place a pixel in the front buffer
void SetPixel(int16_t x, int16_t y, uint8_t color)
{
	// set the memory
	// same as y*320+x, but slightly quicker
	memset(&buffer_front[FASTPOS(x, y)], color, sizeof(uint8_t));
}

// Draw a vertical line
void DrawVerticalLine(int x, int y1, int y2, uint8_t color)
{
	if (y1 > y2)
	{
		for (int y = y2; y < y1; y++)
			SetPixel(x, y, color);
	}
	else
	{
		for (int y = y1; y < y2; y++)
			SetPixel(x, y, color);
	}
}

// Draw a horizontal line
void DrawHorizontalLine(int x1, int x2, int y, uint8_t color)
{
	if (x1 > x2)
		memset(&buffer_front[FASTPOS(x2, y)], color, x1 - x2);
	else
		memset(&buffer_front[FASTPOS(x1, y)], color, x2 - x1);
}

// Draw a filled rectangle
void DrawRectangleFilled(int x, int y, int w, int h, uint8_t color)
{
	int top = FASTPOS(x, y);
	int bottom = FASTPOS(x + w, y + h);

	for (int i = top; i <= bottom; i += VGA_MODE13H_WIDTH)
	{
		memset(&buffer_front[i], color, w);
	}
}

} // namespace VGA
