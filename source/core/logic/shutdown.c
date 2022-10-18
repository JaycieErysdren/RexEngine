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
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Shutdown everything
void Rex_Shutdown(void)
{
	Rex_Fonts_Shutdown();
	SDL_Quit();
	exit(EXIT_SUCCESS);
}
