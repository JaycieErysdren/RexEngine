// ========================================================
//
// FILE:			/source/core/logic/startup.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Rex Engine startup functions.
//
// LAST EDITED:		October 28th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Startup everything
void Rex_Startup(void)
{
	// Add a desktop display mode for getting desktop size information
	SDL_DisplayMode dm;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		Rex_Failure("SDL failed to initialize: %s", SDL_GetError());

	// Initialize desktop display mode
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		Rex_Failure("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());

	// Initialize BRender
	BrBegin();

	// Update global desktop size with displaymode size
	rex_desktop_size[0] = dm.w;
	rex_desktop_size[1] = dm.h;
}
