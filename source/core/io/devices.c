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
// LAST EDITED:		October 11th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Keyboard scancode array
rex_byte rex_keys[512];

// Mouse x and y coordinates (relative to screen resolution)
rex_coord_screen rex_mouse;

// Reads all currently active devices (keyboard, mouse, etc)
void Rex_IO_ReadDevices(void)
{
	SDL_Event event;

	SDL_PumpEvents();

	SDL_GetMouseState(&rex_mouse.x, &rex_mouse.y);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				Rex_Shutdown();
				break;

			case SDL_KEYDOWN:
				rex_keys[event.key.keysym.scancode] = REX_TRUE;
				break;

			case SDL_KEYUP:
				rex_keys[event.key.keysym.scancode] = REX_FALSE;
				break;

			default:
				break;
		}
	}
}
