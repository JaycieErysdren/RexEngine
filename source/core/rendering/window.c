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
// Video modes
//

rex_int rex_videomode;

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

	rex_videomode = REX_VIDEOMODE_WINDOWED;

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

// Render an SDL surface onto an OpenGL external window.
void Rex_ExternalWindow_RenderSurfaceGL(rex_window_external *window, SDL_Surface *surface, rex_int x, rex_int y)
{
	if (surface == NULL) Rex_Failure("Surface is null!");
	GLuint texture;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, window->width, window->height, 0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(x, y);
		glTexCoord2i(1, 0); glVertex2i(x + surface->w, y);
		glTexCoord2i(1, 1); glVertex2i(x + surface->w, y + surface->h);
		glTexCoord2i(0, 1); glVertex2i(x, y + surface->h);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
