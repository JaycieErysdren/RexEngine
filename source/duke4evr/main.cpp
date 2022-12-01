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
	Picture::pic_t pic_cursor;
	char console_buffer[256];

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
	Picture::LoadBMP(&pic_cursor, "gfx/cursor.bmp");

	// Main loop
	while (!DOS::KeyTest(KB_ESC))
	{
		//
		// Input handling
		//

		// Get the mouse cursor input
		int16_t mx, my, mb;
		mb = DOS::MouseRead(&mx, &my);
		if (mx >= SCREEN_WIDTH - pic_cursor.width) mx = SCREEN_WIDTH - pic_cursor.width;
		if (my >= SCREEN_HEIGHT - pic_cursor.height) my = SCREEN_HEIGHT - pic_cursor.height;

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Add some console text
		sprintf(console_buffer, "Mouse X: %d", mx);
		Console::AddText(0, 0, console_buffer);
		sprintf(console_buffer, "Mouse Y: %d", my);
		Console::AddText(0, 1, console_buffer);
		sprintf(console_buffer, "Mouse B: %d", mb);
		Console::AddText(0, 2, console_buffer);

		// Render the mouse cursor
		Picture::Draw8(&pic_bbuffer, &pic_cursor, mx, my, Picture::COLORKEY);

		// Render the console text
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
