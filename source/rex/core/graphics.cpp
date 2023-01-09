// ========================================================
//
// FILE:			/source/rex/core/graphics.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Graphics implementation
//
// LAST EDITED:		January 3rd, 2023
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Graphics
//
//

namespace Rex
{

// Initialize a platform-specific graphics context.
// On a multitasking OS, this will be a window.
// On other operating systems, this will be a fullscreen graphics context.
bool Init_Graphics(rex_int width, rex_int height, rex_int bpp, const char *title)
{
	// If there's already a context, return false
	if (engine_context->graphics_context != nullptr)
	{
		Message("Init_Graphics()", "Cannot initialize graphics, there is already one initialzied!", WARNING);
		return false;
	}

	// create new context
	engine_context->graphics_context = new EngineGraphicsContext;

	// Set variables
	engine_context->graphics_context->width = width;
	engine_context->graphics_context->height = height;
	engine_context->graphics_context->bpp = bpp;
	engine_context->graphics_context->title = title;

	engine_context->graphics_context->platform = Platform_Init_Graphics(width, height, bpp, title);

	if (engine_context->graphics_context->platform == nullptr)
	{
		Message("Init_Graphics()", "Failued to initialzie platform graphics context!", FAILURE);
		return false;
	}

	return true;
}

// Quit graphics context
bool Quit_Graphics()
{
	if (engine_context->graphics_context != nullptr)
	{
		// clear platform-specific context
		if (engine_context->graphics_context->platform != nullptr)
			Platform_Quit_Graphics(engine_context->graphics_context->platform);

		// free memory from graphics context
		delete engine_context->graphics_context;

		return true;
	}

	return false;
}

} // namespace Rex
