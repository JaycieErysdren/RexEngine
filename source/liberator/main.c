// ========================================================
//
// FILE:			/source/liberator/main.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Liberator program entry point.
//
// LAST EDITED:		October 10th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

SDL_Window *liberator_window_main;

void main(int argc, char *argv[])
{
	Rex_Startup();

	liberator_window_main = Rex_WindowExternal_Add("Liberator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);

	while (SDL_TRUE)
	{
		Rex_IO_ReadDevices();
	}

	Rex_Shutdown();
}
