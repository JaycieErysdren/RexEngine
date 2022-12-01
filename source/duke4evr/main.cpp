// ========================================================
//
// FILE:			/source/duke4evr/main.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Duke4Ever program entry point
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

// Duke4Ever header
#include "duke4evr.hpp"

// Main entry point
int main(int argc, char *argv[])
{
	// Variables
	Picture::pic_t pic_font;
	Picture::pic_t pic_bbuffer;
	Picture::pic_t pic_fbuffer;

	// Initialize DOS
	DOS::Initialize();

	// Initialize VGA
	VGA::Initialize();
	VGA::SetPalette("gfx/duke3d.pal");

	// Create pictures
	Console::Initialize();
	Picture::LoadBMP(&pic_font, "gfx/font8x8.bmp");
	Picture::Create(&pic_fbuffer, SCREEN_WIDTH, SCREEN_HEIGHT, 8, 0, (void *)VGA_VIDMEM_PTR);
	Picture::Create(&pic_bbuffer, SCREEN_WIDTH, SCREEN_HEIGHT, 8, 0, 0);

	// Main loop
	while (!DOS::KeyTest(KB_ESC))
	{
		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Add some console text
		Console::AddText(1, 1, "hello world");

		// Add some console text
		Console::Render(&pic_bbuffer, &pic_font);

		// Flip buffers
		Picture::Copy(&pic_fbuffer, &pic_bbuffer);
	}

	// Shutdown VGA
	VGA::Shutdown();

	// Shutdown DOS
	DOS::Shutdown();

	// Cleanup memory
	Picture::Destroy(&pic_font);
	Picture::Destroy(&pic_fbuffer);
	Picture::Destroy(&pic_bbuffer);

	// Exit gracefully
	return EXIT_SUCCESS;
}
