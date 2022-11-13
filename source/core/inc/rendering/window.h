// ========================================================
//
// FILE:			/source/core/inc/rendering/window.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the windowing system functions.
//
// LAST EDITED:		November 12th, 2022
//
// ========================================================

// Definitions
#define REX_DEPTH_BUFFER_CLEAR 0xFFFFFFFF
#define REX_WINDOW_CENTERED SDL_WINDOWPOS_CENTERED
#define REX_WINDOW_DEFAULT_FLAGS (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)

// Number of active windows
extern rex_uint rex_num_windows;

// Global displaymode
extern rex_int rex_displaymode;

// Available displaymodes
enum rex_displaymodes
{
	REX_DISPLAYMODE_TERMINAL,			// Terminal mode
	REX_DISPLAYMODE_GRAPHICS			// Graphics mode
};

// Add a Rex window
rex_window *Rex_Window_Add(rex_byte_c *title, rex_int x, rex_int y, rex_int width, rex_int height, rex_uint flags);

// Remove a Rex window
void Rex_Window_Remove(rex_window *window);

// Update the various values on a Rex window. Returns 1 if size changed.
rex_int Rex_Window_Update(rex_window *window);

// Flip the buffers for a Rex BRender-enabled window.
void Rex_Window_DoubleBuffer(rex_window *window);

// Render a frame from the specified scene to the specified window's color pixelmap (Using the Z-buffer).
void Rex_Window_RenderZb(rex_window *window, br_actor *world, br_actor *camera, rex_rgb color, rex_uint depth);
