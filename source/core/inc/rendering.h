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
// LAST EDITED:		October 12th, 2022
//
// ========================================================

//
// Windowing systems (window.c)
//

// Add an external window
rex_window_external *Rex_WindowExternal_Add(rex_byte_c *title, rex_int x, rex_int y, rex_int width, rex_int height, rex_uint flags);

// Remove an external window
void Rex_WindowExternal_Remove(rex_window_external *window);

// External window resize callback function
void Rex_WindowExternal_CBFN_Resize(rex_int width, rex_int height);

// Swap the buffers for an OpenGL window.
void Rex_ExternalWindow_SwapBuffer(rex_window_external *window);

// Clear the screen for an OpenGL window.
void Rex_ExternalWindow_ClearGL(rex_rgba rgba, rex_ubyte depth);
