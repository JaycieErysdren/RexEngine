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
	Rex::SurfaceCreate(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);
}

void Shutdown()
{
	// Cleanup picture buffers
	Rex::SurfaceDestroy(&pic_font);
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
	// hello
	Initialize();

	// Make a page
	Tag document;
	Tag h1;
	Tag p;
	Tag hr;
	Tag br;

	document.type = "html";
	h1.type = "h1";
	p.type = "p";
	hr.type = "hr";
	br.type = "br";

	h1.content = "Header";
	p.content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	document.children.push_back(h1);
	document.children.push_back(br);
	document.children.push_back(hr);
	document.children.push_back(br);
	document.children.push_back(p);
	document.children.push_back(br);
	document.children.push_back(hr);
	document.children.push_back(br);
	document.children.push_back(p);
	document.children.push_back(br);
	document.children.push_back(p);

	// Main loop
	while (!Rex::KeyTest(REX_SC_ESCAPE))
	{
		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 0);

		// Render HTML
		HTML_Render(&pic_bbuffer, &pic_font, document, 0, 0);

		// Flip the rendering buffers
		Rex::SurfaceToFrontBuffer(&pic_bbuffer);
	}

	// goodbye
	Shutdown();

	// Exit gracefully
	return EXIT_SUCCESS;
}
