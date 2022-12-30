// ========================================================
//
// FILE:			/source/rex/platform/sdl.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine: SDL Platform implementation
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex Engine: SDL Platform
//
//

namespace Rex
{

//
//
// Global Variables
//
//

// Mouse
rex_int32 mouse_x, mouse_y, mouse_b;

// Keyboard
bool keys[256];

//
//
// Functions
//
//

//
// Bootstrap
//

// Initialize all interrupts
bool Platform_Init()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return false;

	return true;
}

// Free all interrupts
bool Platform_Quit()
{
	// Quit SDL
	SDL_Quit();

	return true;
}

//
// Devices
//

// Read data from all currently connected input devices (keyboards, mice, etc)
bool Platform_ReadInputDevices()
{
	SDL_Event event;

	// Pump events
	SDL_PumpEvents();

	// Get mouse info
	mouse_b = SDL_GetMouseState(&mouse_x, &mouse_y);

	// Poll events
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				keys[event.key.keysym.scancode] = true;
				break;

			case SDL_KEYUP:
				keys[event.key.keysym.scancode] = false;
				break;

			default:
				break;
		}
	}

	return false;
}

// Get mouse data
bool Platform_MouseGet(rex_int *mx, rex_int *my, rex_int *mb)
{
	if (mx) *mx = (rex_int)mouse_x;
	if (my) *my = (rex_int)mouse_y;
	if (mb) *mb = (rex_int)mouse_b;

	return true;
}

// Set mouse data
bool Platform_MouseSet(rex_int mx, rex_int my, rex_int mb)
{
	return false;
}

// Returns true if the specified key is pressed
bool Platform_KeyboardGet(rex_int scancode)
{
	return keys[scancode];
}

//
// Timer
//

// Returns the number of ticks elapsed since program start
rex_int64 Platform_GetTicks64()
{
	return (rex_int64)SDL_GetTicks64();
}

} // namespace Rex
