// ========================================================
//
// FILE:			/source/testbed/main.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Testbed program entry point
//
// LAST EDITED:		December 8th, 2022
//
// ========================================================

// Duke4Ever header
#include "testbed.hpp"

#define CYCLES 30

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// General variables
	int i;
	char console_buffer[256];

	// Mouse variables
	vec2i_t mpos;
	int16_t mx, my, mb, mxprev, myprev;

	// Picture buffers
	Picture::pic_t pic_font;
	Picture::pic_t pic_bbuffer;
	Picture::pic_t pic_background;
	Picture::pic_t pic_cursor;

	// Initialize DOS
	DOS::Initialize();

	// Initialize VESA
	if (VESA::Initialize(640, 480, 8) == false) return EXIT_FAILURE;
	VESA::VidInfo vidinfo = VESA::GetVidInfo();

	// Create pictures
	Console::Initialize();
	Picture::InitializeFrontBuffer();
	Picture::LoadBMP(&pic_font, "gfx/font8x8.bmp");
	Picture::LoadBMP(&pic_cursor, "local/cursor.bmp");
	Picture::LoadBMP(&pic_background, "local/forest.bmp");
	Picture::Create(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);

	// Main loop
	while (!DOS::KeyTest(KB_ESC))
	{
		//
		// User inputs
		//

		// Read mouse
		mb = DOS::MouseRead(&mx, &my);
		DOS::MouseSet(160, 100);

		mpos.x += mx - 160;
		mpos.y += my - 100;

		// Clip mouse
		if (mpos.x < 0) mpos.x = 0;
		if (mpos.x > pic_bbuffer.width - pic_cursor.width) mpos.x = pic_bbuffer.width - pic_cursor.width;
		if (mpos.y < 0) mpos.y = 0;
		if (mpos.y > pic_bbuffer.height - pic_cursor.height) mpos.y = pic_bbuffer.height - pic_cursor.height;

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 0);

		// Blit the background
		Picture::Blit8(&pic_bbuffer, 0, 0, pic_bbuffer.width, pic_bbuffer.height, &pic_background, 0, 0, pic_background.width, pic_background.height, Picture::COPY);

		// Blit the mouse
		Picture::Blit8(&pic_bbuffer, mpos.x, mpos.y, mpos.x + pic_cursor.width, mpos.y + pic_cursor.height, &pic_cursor, 0, 0, pic_cursor.width, pic_cursor.height, Picture::COLORKEY);

		sprintf(console_buffer, "mx: %d my: %d", mpos.x, mpos.y);
		Console::AddText(0, 0, console_buffer);

		// Render the console text
		Console::Render(&pic_bbuffer, &pic_font, 16);

		// Flip the rendering buffers
		Picture::CopyToFrontBuffer(&pic_bbuffer);
	}

	// Shutdown VESA
	VESA::Shutdown();

	// Shutdown DOS
	DOS::Shutdown();

	// Cleanup memory
	Console::Shutdown();
	Picture::Destroy(&pic_font);
	Picture::Destroy(&pic_bbuffer);
	Picture::Destroy(&pic_background);
	Picture::Destroy(&pic_cursor);
	Picture::ShutdownFrontBuffer();

	// Exit gracefully
	return EXIT_SUCCESS;
}
