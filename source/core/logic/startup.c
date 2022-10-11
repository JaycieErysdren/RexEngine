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
// LAST EDITED:		October 10th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Startup everything
void Rex_Startup(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		Rex_Failure("SDL failed to initialize! (%s)", SDL_GetError());
}
