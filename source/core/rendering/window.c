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
rex_window_external *Rex_WindowExternal_Add(rex_byte_c *title, rex_int x, rex_int y, rex_int width, rex_int height, rex_uint flags)
{
	SDL_Window *win = SDL_CreateWindow(title, x, y, width, height, flags);

	if (win == NULL)
		Rex_Failure("SDL Window failed to initialize! (%s)", SDL_GetError());

	rex_window_external *rex_win = calloc(1, sizeof(rex_window_external));

	if (rex_win == NULL)
		Rex_Failure("Rex Window failed to initialize! (%s)", SDL_GetError());

	rex_win->title = title;
	rex_win->width = height;
	rex_win->height = height;
	rex_win->pos.x = x;
	rex_win->pos.y = y;
	rex_win->pos.z = 0;

	_rex_num_windows_external += 1;
	
	return rex_win;
}

// Remove an external window
void Rex_WindowExternal_Remove(rex_window_external *window)
{
	SDL_DestroyWindow(window->window);

	free(window);

	_rex_num_windows_external -= 1;

	if (_rex_num_windows_external <= 0)
		Rex_Failure("No windows to display!");
}
