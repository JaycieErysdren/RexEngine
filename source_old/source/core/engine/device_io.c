// ========================================================
//
// FILE:			/source/core/engine/device_io.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Device I/O functions.
//
// LAST EDITED:		November 12th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Keyboard scancode array
rex_byte rex_keys[512];

// Mouse button scancode array
rex_byte rex_mouse_buttons[32];

// Mouse x and y coordinates (relative to screen resolution)
rex_coord2i rex_mouse;

// Mouse x and y delta since last read
rex_vector2i rex_mouse_delta;

// Mouse scroll wheel x and y since last read
rex_vector2i rex_mouse_scroll;

// Mouse scroll wheel x and y delta since last read
rex_vector2i rex_mouse_scroll_delta;

// The current desktop resolution
rex_vector2i rex_desktop_size;

// Reads all currently active devices (keyboard, mouse, etc)
void Rex_IO_ReadDevices(void)
{
	SDL_Event event;
	rex_coord2i rex_mouse_previous = rex_mouse;
	rex_vector2i rex_mouse_scroll_previous;

	if (rex_displaymode == REX_DISPLAYMODE_GRAPHICS)
	{
		SDL_DisplayMode dm;

		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
			Rex_Message(REX_MESSAGE_FAILURE, "SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());

		rex_desktop_size[0] = dm.w;
		rex_desktop_size[1] = dm.h;
	}

	rex_mouse_scroll_previous[0] = rex_mouse_scroll[0];
	rex_mouse_scroll_previous[1] = rex_mouse_scroll[1];

	SDL_PumpEvents();

	// Get mouse state into the global mouse position coordinates
	SDL_GetMouseState(&rex_mouse.x, &rex_mouse.y);

	// If the mouse has moved, calculate the delta
	if (rex_mouse_previous.x != rex_mouse.x)
		rex_mouse_delta[0] = rex_mouse.x - rex_mouse_previous.x;
	else
		rex_mouse_delta[0] = 0;

	if (rex_mouse_previous.y != rex_mouse.y)
		rex_mouse_delta[1] = rex_mouse.y - rex_mouse_previous.y;
	else
		rex_mouse_delta[1] = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				rex_running = REX_FALSE;
				break;
			
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
						break;

					default:
						break;
				}

			case SDL_MOUSEWHEEL:
				rex_mouse_scroll[0] += event.wheel.x;
				rex_mouse_scroll[1] += event.wheel.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				rex_mouse_buttons[event.button.button] = REX_TRUE;
				break;

			case SDL_MOUSEBUTTONUP:
				rex_mouse_buttons[event.button.button] = REX_FALSE;
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

	// If the mousewheel has moved, calculate the delta
	if (rex_mouse_scroll_previous[0] != rex_mouse_scroll[0])
		rex_mouse_scroll_delta[0] = rex_mouse_scroll[0] - rex_mouse_scroll_previous[0];
	else
		rex_mouse_scroll_delta[0] = 0;

	if (rex_mouse_scroll_previous[1] != rex_mouse_scroll[1])
		rex_mouse_scroll_delta[1] = rex_mouse_scroll[1] - rex_mouse_scroll_previous[1];
	else
		rex_mouse_scroll_delta[1] = 0;
}
