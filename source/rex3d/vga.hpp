// ========================================================
//
// FILE:			/source/vga.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		VGA namespace header
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

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
	bool Initialize();

	// Return to text mode
	bool Shutdown();

	//
	// Palette
	//

	// Set a color in the palette
	void SetPaletteColor(uint8_t i, uint8_t r, uint8_t g, uint8_t b);

	// Set the palette from a file
	void SetPalette(string filename);

	// Draw the palette on the back buffer
	void ShowPalette();

	//
	// Pixel
	//

	// Place a pixel in the back buffer
	void PlacePixel(int x, int y, uint8_t color);

	// Draw a filled rectangle
	void DrawRectangleFilled(int left, int top, int right, int bottom, uint8_t color);

	//
	// Rendering
	//

	// Clear the back buffer
	void Clear();

	// Copy the back buffer to the front buffer
	void Flip();
}
