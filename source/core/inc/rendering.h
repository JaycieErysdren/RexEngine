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
// LAST EDITED:		October 21st, 2022
//
// ========================================================

//
// BRender helpers
//

extern rex_ubyte brender_primitive_heap[1500 * 1024];

//
// Embedded system fonts (fonts.c)
//

extern TTF_Font *font_dos_8x8;		// DOS standard font, 8x8
extern TTF_Font *font_dos_8x14;		// DOS standard font, 8x14
extern TTF_Font *font_dos_8x16;		// DOS standard font, 8x16
extern TTF_Font *font_dos_9x14;		// DOS standard font, 9x14
extern TTF_Font *font_dos_9x16;		// DOS standard font, 9x16

// Start up system fonts
void Rex_Fonts_Startup(void);

// Shut down system fonts
void Rex_Fonts_Shutdown(void);

// Generate an SDL Surface which contains a message's pixel data
void Rex_Fonts_GenerateTextSurface(SDL_Surface **surface, TTF_Font *font, rex_rgba color, rex_byte *s, ...);

// Render text to the given external window
void Rex_Fonts_RenderText(rex_window_external *window, rex_int x, rex_int y, rex_rgba color, TTF_Font *font, rex_byte *s, ...);

//
// Embedded system palettes (palettes.c)
//

// BRender standard palette (STD.PAL)
extern const rex_palette palette_brender;

// Quake palette (PALETTE.LMP)
extern const rex_palette palette_quake;

//
// Windowing systems (window.c)
//

// Definitions
#define REX_DEPTH_BUFFER_CLEAR 0xFFFFFFFF

// Global videomode
extern rex_int rex_videomode;

// Available videomodes
enum rex_videomodes
{
	REX_VIDEOMODE_TERMINAL,				// Terminal mode
	REX_VIDEOMODE_WINDOWED,				// Windowed mode
	REX_VIDEOMODE_FULLSCREEN			// Fullscreen mode
};

// Add an external window
rex_window_external *Rex_WindowExternal_Add(rex_byte_c *title, rex_int x, rex_int y, rex_int width, rex_int height, rex_uint flags);

// Remove an external window
void Rex_WindowExternal_Remove(rex_window_external *window);

// Update the various values on an external window. Returns 1 if size changed.
rex_int Rex_WindowExternal_Update(rex_window_external *window);

// Flip the buffers for a BRender window.
void Rex_ExternalWindow_DoubleBuffer(rex_window_external *window);

// Render a frame from a BRender window to a pixelmap (Using the Z-buffer).
void Rex_ExternalWindow_RenderZb(rex_window_external *window, br_actor *world, br_actor *camera, rex_rgb color, rex_uint depth);

//
// Antiquated functions, remove these
//

// External window resize callback function
void Rex_WindowExternal_CBFN_Resize(rex_int width, rex_int height);

// Clear the screen for an OpenGL window.
void Rex_ExternalWindow_ClearGL(rex_rgba rgba, rex_ubyte depth);

// Render an SDL surface onto an OpenGL external window.
void Rex_ExternalWindow_RenderSurfaceGL(rex_window_external *window, SDL_Surface *surface, rex_int x, rex_int y);
