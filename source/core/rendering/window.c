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
// LAST EDITED:		October 11th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

//
// External windows
//

// Add an external window
rex_window_external *Rex_WindowExternal_Add(rex_byte_c *title, rex_int x, rex_int y, rex_int width, rex_int height, rex_uint flags)
{
	rex_window_external *rex_win = (rex_window_external *)calloc(1, sizeof(rex_window_external));

	if (rex_win == NULL)
		Rex_Failure("Rex Window failed to initialize! (%s)", SDL_GetError());

	rex_win->title = title;
	rex_win->width = height;
	rex_win->height = height;
	rex_win->pos.x = x;
	rex_win->pos.y = y;
	rex_win->window = SDL_CreateWindow(title, x, y, width, height, flags);

	if (rex_win->window  == NULL)
		Rex_Failure("SDL Window failed to initialize! (%s)", SDL_GetError());

	return rex_win;
}

// Remove an external window
void Rex_WindowExternal_Remove(rex_window_external *window)
{
	SDL_DestroyWindow(window->window);
	//free(window);
}
