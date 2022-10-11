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

// Keyboard scancode array
rex_byte _rex_keys[512];

// Mouse x and y coordinates (relative to screen resolution)
rex_coord _rex_mouse;

// Reads all currently active devices (keyboard, mouse, etc)
void Rex_IO_ReadDevices(void)
{
	SDL_Event event;

	SDL_PumpEvents();

	SDL_GetMouseState(&_rex_mouse.x, &_rex_mouse.y);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				Rex_Shutdown();
				break;

			case SDL_KEYDOWN:
				_rex_keys[event.key.keysym.scancode] = REX_TRUE;
				break;

			case SDL_KEYUP:
				_rex_keys[event.key.keysym.scancode] = REX_FALSE;
				break;

			default:
				break;
		}
	}
}
