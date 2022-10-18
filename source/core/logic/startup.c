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
// DESCRIPTION:		Startup functions.
//
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Startup everything
void Rex_Startup(void)
{
	SDL_DisplayMode dm;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		Rex_Failure("SDL failed to initialize: %s", SDL_GetError());

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		Rex_Failure("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());

	Rex_Fonts_Startup();

	rex_desktop_size[0] = dm.w;
	rex_desktop_size[1] = dm.h;
}
