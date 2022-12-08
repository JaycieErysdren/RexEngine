// ========================================================
//
// FILE:			/source/viddrv/vga.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		VGA namespace header
//
// LAST EDITED:		December 7th, 2022
//
// ========================================================

//
//
// Text macros
//
//

#define VGA_VIDMEM_PTR (0xa0000 + __djgpp_conventional_base)

//
//
// VGA namespace
//
//

// VGA namespace definition (public)
namespace VGA
{
	//
	//
	// Functions
	//
	//

	//
	// Bootstrap
	//

	// Set mode 13h
	bool Initialize(int w, int h, int bpp);

	// Return to text mode
	void Shutdown();

	//
	// Palette
	//

	// Set a color in the palette
	void SetPaletteColor(uint8_t i, uint8_t r, uint8_t g, uint8_t b);

	// Set the palette from a file
	void SetPalette(string filename);

	// Draw the palette on the back buffer
	void DrawPalette();

	//
	// Pixel
	//

	// Place a pixel in the back buffer
	void SetPixel(int16_t x, int16_t y, uint8_t color);

	// Draw a vertical line
	void DrawVerticalLine(int x, int y1, int y2, uint8_t color);

	// Draw a horizontal line
	void DrawHorizontalLine(int x1, int x2, int y, uint8_t color);

	// Draw a filled rectangle
	void DrawRectangleFilled(int x, int y, int w, int h, uint8_t color);

	//
	// Rendering
	//

	// Clear the back buffer
	void Clear(uint8_t color);

	// Copy the back buffer to the front buffer
	void Flip();
}
