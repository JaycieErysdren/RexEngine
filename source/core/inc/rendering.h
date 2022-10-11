// ========================================================
//
// FILE:			/source/core/inc/rendering.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the rendering systems.
//
// LAST EDITED:		October 10th, 2022
//
// ========================================================

//
// Windowing systems (window.c)
//

// External windows
SDL_Window *Rex_WindowExternal_Add(const char *title, int x, int y, int width, int height, unsigned int flags);
void Rex_WindowExternal_Remove(SDL_Window *window);

// Internal windows
