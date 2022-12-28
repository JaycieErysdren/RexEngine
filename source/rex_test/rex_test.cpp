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

// Video info
Rex::VidInfo vidinfo;

// Intiialization function
void Initialize()
{
	// Initialize Rex Engine
	Rex::Initialize();

	// Initialize graphics
	if (Rex::InitializeGraphics(640, 480, 16) == false)
	{
		cout << "failed to initialize graphics driver" << endl;
		exit(EXIT_FAILURE);
	}

	vidinfo = Rex::GetVidInfo();
}

void Shutdown()
{
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

	// create surf
	Rex::Surface *s_bbuffer = Rex::AddSurface(640, 480, 16, NULL);

	// Main loop
	while (!Rex::KeyTest(REX_SC_ESCAPE))
	{
		// Flip to front buffer
		rex_color c;
		c.u16 = REX_COLOR_RGB_565(255, 0, 0);
		s_bbuffer->Clear(c);
		s_bbuffer->Flip();
	}

	// Free memory associated with surface
	Rex::FreeSurface(s_bbuffer);

	// goodbye
	Shutdown();

	// Exit gracefully
	return EXIT_SUCCESS;
}
