// ========================================================
//
// FILE:			/source/core/engine/bootstrap.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Startup and shutdown functions.
//
// LAST EDITED:		November 12th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Boolean that is equal to REX_TRUE if the program is supposed to be running.
rex_bool rex_running;

// Startup all systems
void Rex_Startup(rex_int display_mode)
{
	rex_displaymode = display_mode;

	if (display_mode == REX_DISPLAYMODE_TERMINAL)
	{
		// Initialize SDL
		if (SDL_Init(SDL_INIT_EVENTS) != 0)
			Rex_Message(REX_MESSAGE_FAILURE, "SDL failed to initialize: %s", SDL_GetError());
	}
	else if (display_mode == REX_DISPLAYMODE_GRAPHICS)
	{
		// Add a desktop display mode for getting desktop size information
		SDL_DisplayMode dm;

		// Initialize SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			Rex_Message(REX_MESSAGE_FAILURE, "SDL failed to initialize: %s", SDL_GetError());

		// Initialize desktop display mode
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
			Rex_Message(REX_MESSAGE_FAILURE, "SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());

		// Initialize BRender
		BrBegin();

		// Update global desktop size with displaymode size
		rex_desktop_size[0] = dm.w;
		rex_desktop_size[1] = dm.h;
	}

	rex_running = REX_TRUE;
}

// Shutdown everything and return exit code to OS
void Rex_Shutdown(rex_int exit_code)
{
	rex_running = REX_FALSE;

	if (rex_displaymode == REX_DISPLAYMODE_GRAPHICS)
	{
		// End BRender Rendering
		BrRendererEnd();

		// Shutdown BRender
		BrEnd();
	}

	// Shutdown SDL
	SDL_Quit();

	// Exit program
	exit(exit_code);
}
