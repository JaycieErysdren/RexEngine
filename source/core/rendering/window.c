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
// LAST EDITED:		October 21st, 2022
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
	// Allocate an external window
	rex_window_external *window = (rex_window_external *)calloc(1, sizeof(rex_window_external));

	// Buffer for OpenGL driver arguments
	rex_byte driver_args[512];

	// Drawable area
	rex_int drawable_width, drawable_height;

	// BRender error checker
	br_error brender_error;

	// Check for allocation error
	if (window == NULL)
		Rex_Failure("Rex Window failed to initialize!");

	// Setup rex_window_external variables
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
		Rex_Failure("SDL Window failed to initialize! (%s)", SDL_GetError());

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
    if (brender_error != BRE_OK) Rex_Failure("BrDevBeginVar() failed!");

	// Allocate color and depth buffers
	window->buffer_color = BrPixelmapMatchTypedSized(window->buffer_screen, BR_PMMATCH_OFFSCREEN, BR_PMT_RGB_888, drawable_width, drawable_height);
	window->buffer_depth = BrPixelmapMatch(window->buffer_color, BR_PMMATCH_DEPTH_16);

	// Fix buffer origins
	window->buffer_screen->origin_x = window->buffer_color->origin_x = window->buffer_depth->origin_x = (br_int_16)(drawable_width / 2);
	window->buffer_screen->origin_y = window->buffer_color->origin_y = window->buffer_depth->origin_y = (br_int_16)(drawable_height / 2);

	// Begin BRender rendering
    BrRendererBegin(window->buffer_color, NULL, NULL, brender_primitive_heap, sizeof(brender_primitive_heap));

	// Set global videomode
	rex_videomode = REX_VIDEOMODE_WINDOWED;

	// Return pointer to window
	return window;
}

// Remove an external window
void Rex_WindowExternal_Remove(rex_window_external *window)
{
	if (window->buffer_depth) BrPixelmapFree(window->buffer_depth);
	if (window->buffer_color) BrPixelmapFree(window->buffer_color);
	if (window->buffer_screen) BrPixelmapFree(window->buffer_screen);
	if (window->sdl_window) SDL_DestroyWindow(window->sdl_window);
}

// Update the various values on an external window. Returns 1 if size changed.
rex_int Rex_WindowExternal_Update(rex_window_external *window)
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
void Rex_ExternalWindow_DoubleBuffer(rex_window_external *window)
{
	BrPixelmapDoubleBuffer(window->buffer_screen, window->buffer_color);
	//SDL_GL_SwapWindow(window->sdl_window);
}

// Render a frame from the specified scene to the specified window's screen buffer (Using the Z-buffer).
void Rex_ExternalWindow_RenderZb(rex_window_external *window, br_actor *world, br_actor *camera, rex_rgb color, rex_uint depth)
{
	BrRendererFrameBegin();
	BrPixelmapFill(window->buffer_color, BR_COLOUR_RGB(color.r, color.g, color.b));
	BrPixelmapFill(window->buffer_depth, depth);
	BrZbSceneRender(world, camera, window->buffer_color, window->buffer_depth);
	BrRendererFrameEnd();
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
