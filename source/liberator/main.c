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
// LAST EDITED:		October 11th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

rex_window_external *window1;
rex_window_external *window2;

void main(int argc, char *argv[])
{
	Rex_Startup();

	window1 = Rex_WindowExternal_Add("Liberator1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
	window2 = Rex_WindowExternal_Add("Liberator2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

	while (REX_TRUE)
	{
		Rex_IO_ReadDevices();

		if (KEY_DOWN(SDL_SCANCODE_Q))
			Rex_WindowExternal_Remove(window1);

		if (KEY_DOWN(SDL_SCANCODE_C))
			Rex_WindowExternal_Remove(window2);
	}

	Rex_Shutdown();
}
