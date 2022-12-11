// ========================================================
//
// FILE:			/source/rex/modules/hwio/vga/vga.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		VGA namespace header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

//
//
// Text macros
//
//

#if (REX_COMPILER == COMPILER_DJGPP)

	#define VGA_VIDMEM_PTR (0xA0000 + __djgpp_conventional_base)

#endif

#if (REX_COMPILER == COMPILER_WATCOM)

	#define VGA_VIDMEM_PTR (0xA0000L)

#endif

#define VGA_MODE13H_WIDTH 320
#define VGA_MODE13H_HEIGHT 200

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
	// Utilities
	//

	// Wait for vertical retrace ("vsync")
	void WaitForRetrace();

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
}
