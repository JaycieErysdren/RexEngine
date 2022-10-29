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
// LAST EDITED:		October 28th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Shutdown everything
void Rex_Shutdown(void)
{
	// End BRender Rendering
	BrRendererEnd();

	// Shutdown BRender
	BrEnd();

	// Shutdown SDL
	SDL_Quit();

	// Exit program
	exit(EXIT_SUCCESS);
}
