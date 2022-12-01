// ========================================================
//
// FILE:			/source/viddrv/vga.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		VGA namespace implementation
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

// Rex3D header
#include "rex3d.hpp"

#define SLOWPOS(x, y)		(((y) * SCREEN_HEIGHT) + (x))
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

//
// Buffers
//

// Back buffer
uint8_t *buffer_back;

// Front buffer (pointer to video memory)
uint8_t *buffer_front;

//
//
// Functions
//
//

//
// Bootstrap
//

// Initialize
bool Initialize()
{
	// Set VGA mode 0x13
	union REGS r;
	r.h.ah = 0x00;
	r.h.al = 0x13;
	int86(0x10, &r, &r);

	// Allocate & clear back buffer
	buffer_back = new uint8_t [64000];
	memset(buffer_back, 0, 64000);

	// Set & clear video memory
	buffer_front = (uint8_t *)(0xa0000 + __djgpp_conventional_base);
	memset(buffer_front, 0, 64000);

	return true;
}

// Shutdown
bool Shutdown()
{
	// Set mode 0x03
	union REGS r;
	r.x.ax = 0x03;
	int86(0x10, &r, &r);

	// Free memory
	delete [] (buffer_back);

	return true;
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

// Place a pixel in the back buffer
void SetPixel(int x, int y, uint8_t color)
{
	//if (color < 255) buffer_back[(y << 8) + (y << 6) + x] = color;
	//buffer_back[(y << 8) + (y << 6) + x] = color;

	// set the memory
	// same as y*320+x, but slightly quicker
	memset(&buffer_back[FASTPOS(x, y)], color, sizeof(uint8_t));
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
		memset(&buffer_back[FASTPOS(x2, y)], color, x1 - x2);
	else
		memset(&buffer_back[FASTPOS(x1, y)], color, x2 - x1);
}

// Draw a filled rectangle
void DrawRectangleFilled(int left, int top, int right, int bottom, uint8_t color)
{
	int top_offset, bottom_offset, i, temp, width;

	if (top > bottom)
	{
		temp = top;
		top = bottom;
		bottom = temp;
	}

	if (left > right)
	{
		temp = left;
		left = right;
		right = temp;
	}

	top_offset = (top << 8) + (top << 6) + left;
	bottom_offset = (bottom << 8) + (bottom << 6) + left;
	width = right - left + 1;

	for(i = top_offset; i <= bottom_offset; i += SCREEN_WIDTH)
	{
		memset(&buffer_back[i], color, width);
	}
}

//
// Rendering
//

// Clear the back buffer
void Clear()
{
	memset(buffer_back, 0, 64000);
}

// Copy the back buffer to the front buffer
void Flip()
{
	// Wait for vertical retrace
	while (inportb(0x3da) & 0x08);
	while (!(inportb(0x3da) & 0x08));

	// Copy the back buffer to the front buffer
	memcpy(buffer_front, buffer_back, 64000);
}

} // namespace VGA
