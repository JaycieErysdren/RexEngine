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
// LAST EDITED:		October 13th, 2022
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

	if (rex_win->window == NULL)
		Rex_Failure("SDL Window failed to initialize! (%s)", SDL_GetError());

	rex_win->context = SDL_GL_CreateContext(rex_win->window);

	return rex_win;
}

// Remove an external window
void Rex_WindowExternal_Remove(rex_window_external *window)
{
	SDL_DestroyWindow(window->window);
	//free(window);
}

// External window resize callback function
void Rex_WindowExternal_CBFN_Resize(rex_int width, rex_int height)
{
	if (height <= 0) height = 1;
	if (width <= 0) width = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Swap the buffers for an OpenGL window.
void Rex_ExternalWindow_SwapBuffer(rex_window_external *window)
{
	SDL_GL_SwapWindow(window->window);
}

// Clear the screen for an OpenGL window.
void Rex_ExternalWindow_ClearGL(rex_rgba rgba, rex_ubyte depth)
{
	glClearColor((GLclampf)rgba.r / 255, (GLclampf)rgba.g / 255, (GLclampf)rgba.b / 255, (GLclampf)rgba.a / 255);
	glClearDepth((GLclampf)depth / 255);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
