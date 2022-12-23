// ========================================================
//
// FILE:			/source/rex_test/rex_test.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Main game entry point
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

// Header
#include "rex_test.hpp"

#define CYCLES 30

//
// Globals
//

// Picture buffers
Rex::Surface pic_font;
Rex::Surface pic_bbuffer;
Rex::Surface pic_decker;

// Video info
Rex::VidInfo vidinfo;

// Intiialization function
void Initialize()
{
	// Initialize Rex Engine
	Rex::Initialize();

	// Initialize graphics
	if (Rex::InitializeGraphics(640, 480, 8) == false)
	{
		cout << "failed to initialize graphics driver" << endl;
		exit(EXIT_FAILURE);
	}

	vidinfo = Rex::GetVidInfo();

	// Create picture buffers
	Rex::SurfaceLoadBMP(&pic_font, "gfx/font8x8.bmp");
	Rex::SurfaceLoadBMP(&pic_decker, "decker/0b.bmp");
	Rex::SurfaceCreate(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);
}

void Shutdown()
{
	// Cleanup picture buffers
	Rex::SurfaceDestroy(&pic_font);
	Rex::SurfaceDestroy(&pic_decker);
	Rex::SurfaceDestroy(&pic_bbuffer);

	// Shutdown Graphics
	Rex::ShutdownGraphics();

	// Shutdown Rex Engine
	Rex::Shutdown();
}

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// initialize engine, allocate memory, etc
	Initialize();

	// Main loop
	while (!Rex::KeyTest(REX_SC_ESCAPE))
	{
		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 0); // ceiling

		// Decker
		Rex::SurfaceCopy(&pic_bbuffer, &pic_decker);

		// Flip the rendering buffers
		Rex::SurfaceToFrontBuffer(&pic_bbuffer);
	}

	// Shutdown engine, free up memory, etc
	Shutdown();

	// Exit gracefully
	return EXIT_SUCCESS;
}
