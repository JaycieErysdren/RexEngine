// ========================================================
//
// FILE:			/source/core/logic/shutdown.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Shutdown functions.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Shutdown everything and return exit code
void Rex_Shutdown(rex_int exit_code)
{
	rex_running = REX_FALSE;

	// End BRender Rendering
	BrRendererEnd();

	// Shutdown BRender
	BrEnd();

	// Shutdown SDL
	SDL_Quit();

	// Exit program
	exit(exit_code);
}
