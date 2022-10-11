// ========================================================
//
// FILE:			/source/core/rendering/window.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Windowing systems.
//
// LAST EDITED:		October 10th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

//
// External windows
//

int _rex_num_windows_external;

// Add an external window
SDL_Window *Rex_WindowExternal_Add(const char *title, int x, int y, int width, int height, unsigned int flags)
{
	SDL_Window *win = SDL_CreateWindow(title, x, y, width, height, flags);

	if (win == NULL)
		Rex_Failure("SDL Window failed to initialize! (%s)", SDL_GetError());

	_rex_num_windows_external += 1;
	
	return win;
}

// Remove an external window
void Rex_WindowExternal_Remove(SDL_Window *window)
{
	SDL_DestroyWindow(window);

	_rex_num_windows_external -= 1;

	if (_rex_num_windows_external <= 0)
		Rex_Failure("No windows to display!");
}
