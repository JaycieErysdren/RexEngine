// ========================================================
//
// FILE:			/source/core/io/devices.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Device I/O functions.
//
// LAST EDITED:		October 10th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

char _rex_keys[512];

void Rex_IO_ReadDevices(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				Rex_Shutdown();
				break;

			case SDL_KEYDOWN:
				_rex_keys[event.key.keysym.scancode] = SDL_TRUE;
				break;
			case SDL_KEYUP:
				_rex_keys[event.key.keysym.scancode] = SDL_FALSE;
				break;

			default:
				break;
		}
	}
}
