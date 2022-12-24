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

// HTML tag attribute
class Attribute
{
	public:
		string type;
		string content;
};

// HTML tag
class Tag
{
	public:
		string type;
		string content;

		vector<Tag> children;
		vector<Attribute> attributes;
};

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

	// Main loop
	while (!Rex::KeyTest(REX_SC_ESCAPE))
	{
		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 0);

		// Flip the rendering buffers
		Rex::SurfaceToFrontBuffer(&pic_bbuffer);
	}

	// goodbye
	Shutdown();

	// Exit gracefully
	return EXIT_SUCCESS;
}
