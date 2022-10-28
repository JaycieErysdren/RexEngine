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
// LAST EDITED:		October 28th, 2022
//
// ========================================================

//
// BRender helpers (brender.c)
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

// Start up system fonts.
void Rex_Fonts_Startup(void);

// Shut down system fonts.
void Rex_Fonts_Shutdown(void);

// Generate an SDL Surface which contains a message's pixel data.
void Rex_Fonts_GenerateTextSurface(SDL_Surface **surface, TTF_Font *font, rex_rgba color, rex_byte *s, ...);

// Render text to the given external window.
void Rex_Fonts_RenderText(rex_window *window, rex_int x, rex_int y, rex_rgba color, TTF_Font *font, rex_byte *s, ...);

// Generate a BRender Pixelmap which contains a formatted message's pixel data.
br_pixelmap *Rex_Fonts_GenerateTextPixelmap(TTF_Font *font, rex_rgba color, rex_byte *s, ...);

//
// Nuklear helpers (nuklear.c)
//

// Rex Nuklear Context
typedef struct
{
	struct nk_context ctx;
	struct nk_rect scissors;
	struct br_pixelmap *pm;
	struct br_pixelmap *font_tex;
	struct nk_font_atlas atlas;
} rex_nuklear_context;

// Initialize Nuklear with source pixelmap, font and font size.
rex_nuklear_context *Rex_Nuklear_Init(br_pixelmap *pm, const rex_byte *font_filename, rex_float font_size);

// Render the given Nuklear context to the defined pixelmap.
void Rex_Nuklear_Render(const rex_nuklear_context *context, const struct nk_color clear, const rex_ubyte enable_clear);

// Shutdown the given Nuklear context.
void Rex_Nuklear_Shutdown(rex_nuklear_context *context);

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
#define REX_WINDOW_CENTERED SDL_WINDOWPOS_CENTERED
#define REX_WINDOW_DEFAULT_FLAGS (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)

// Number of active windows
extern rex_uint rex_num_windows;

// Global videomode
extern rex_int rex_videomode;

// Available videomodes
enum rex_videomodes
{
	REX_VIDEOMODE_TERMINAL,				// Terminal mode
	REX_VIDEOMODE_WINDOWED,				// Windowed mode
	REX_VIDEOMODE_FULLSCREEN			// Fullscreen mode
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
