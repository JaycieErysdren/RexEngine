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
// DESCRIPTION:		Windowing system functions.
//
// LAST EDITED:		November 12th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Global displaymode
rex_int rex_displaymode;

//
// Windows
//

// Number of active windows
rex_uint rex_num_windows;

// Add a Rex window
rex_window *Rex_Window_Add(rex_byte_c *title, rex_int x, rex_int y, rex_int width, rex_int height, rex_uint flags)
{
	// Allocate a Rex window
	rex_window *window = (rex_window *)calloc(1, sizeof(rex_window));

	// Buffer for OpenGL driver arguments
	rex_byte driver_args[512];

	// Drawable area
	rex_int drawable_width, drawable_height;

	// BRender error checker
	br_error brender_error;

	// Check for allocation error
	if (window == NULL)
		Rex_MakeError("Failed to allocate memory for rex_window type!");

	// Setup rex_window variables
	window->title = title;
	window->width = width;
	window->height = height;
	window->pos.x = x;
	window->pos.y = y;
	window->sdl_window = SDL_CreateWindow(title, x, y, width, height, flags);

	// Initialize SDL OpenGL contexts
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

	// Check for SDL error
	if (window->sdl_window == NULL)
		Rex_MakeError("SDL Window failed to initialize! (%s)", SDL_GetError());

	// Print driver arguments into the buffer
	snprintf(driver_args, BR_ASIZE(driver_args), "WINDOW_HANDLE=0x%" PRIxPTR, (rex_int_ptr)window->sdl_window);

	// Pass the driver arguments into the BRender OpenGL driver
	BrDevAddStatic(NULL, BrDrvGLBegin, driver_args);

	// Get drawable area
	SDL_GL_GetDrawableSize(window->sdl_window, &drawable_width, &drawable_height);

    brender_error = BrDevBeginVar(&window->buffer_screen, "opengl",
		BRT_WIDTH_I32, (br_int_32)drawable_width,
		BRT_HEIGHT_I32, (br_int_32)drawable_height,
		BRT_PIXEL_BITS_I32, 24,
		BR_NULL_TOKEN
    );

	// Check for BRender error
    if (brender_error != BRE_OK)
		Rex_Message(REX_MESSAGE_FAILURE, "BrDevBeginVar() failed!");

	// Allocate color and depth buffers
	window->buffer_color = BrPixelmapMatchTypedSized(window->buffer_screen, BR_PMMATCH_OFFSCREEN, BR_PMT_RGB_888, drawable_width, drawable_height);
	window->buffer_depth = BrPixelmapMatch(window->buffer_color, BR_PMMATCH_DEPTH_16);

	// Fix buffer origins
	window->buffer_screen->origin_x = window->buffer_color->origin_x = window->buffer_depth->origin_x = (br_int_16)(drawable_width / 2);
	window->buffer_screen->origin_y = window->buffer_color->origin_y = window->buffer_depth->origin_y = (br_int_16)(drawable_height / 2);

	// Begin BRender rendering
    BrRendererBegin(window->buffer_color, NULL, NULL, brender_primitive_heap, sizeof(brender_primitive_heap));

	rex_num_windows += 1;

	// Return pointer to window
	return window;
}

// Remove a R window
void Rex_Window_Remove(rex_window *window)
{
	if (window->buffer_depth) BrPixelmapFree(window->buffer_depth);
	if (window->buffer_color) BrPixelmapFree(window->buffer_color);
	if (window->buffer_screen) BrPixelmapFree(window->buffer_screen);
	if (window->sdl_window) SDL_DestroyWindow(window->sdl_window);

	free(window);

	rex_num_windows -= 1;
}

// Update the various values on a Rex window. Returns 1 if size changed.
rex_int Rex_Window_Update(rex_window *window)
{
	rex_int w, h;
	rex_int dw, dh;

	// Update window size values
	SDL_GetWindowSize(window->sdl_window, &w, &h);
	SDL_GL_GetDrawableSize(window->sdl_window, &dw, &dh);

	if (w != window->width || h != window->height)
	{
		window->width = w;
		window->width = h;

		// Resize pixel buffers
		BrPixelmapResize(window->buffer_depth, dw, dh);
		BrPixelmapResize(window->buffer_color, dw, dh);
		BrPixelmapResize(window->buffer_screen, dw, dh);

		// Fix pixel buffer origins
		window->buffer_screen->origin_x = window->buffer_color->origin_x = window->buffer_depth->origin_x = (br_int_16)(dw / 2);
		window->buffer_screen->origin_y = window->buffer_color->origin_y = window->buffer_depth->origin_y = (br_int_16)(dh / 2);

		return REX_TRUE;
	}
	else
	{
		return REX_FALSE;
	}
}

// Flip the buffers for a BRender window.
void Rex_Window_DoubleBuffer(rex_window *window)
{
	BrPixelmapDoubleBuffer(window->buffer_screen, window->buffer_color);
}

// Render a frame from the specified scene to the specified window's color pixelmap (Using the Z-buffer).
void Rex_Window_RenderZb(rex_window *window, br_actor *world, br_actor *camera, rex_rgb color, rex_uint depth)
{
	BrRendererFrameBegin();
	BrPixelmapFill(window->buffer_color, BR_COLOUR_RGB(color.r, color.g, color.b));
	BrPixelmapFill(window->buffer_depth, depth);
	BrZbSceneRender(world, camera, window->buffer_color, window->buffer_depth);
	BrRendererFrameEnd();
}
