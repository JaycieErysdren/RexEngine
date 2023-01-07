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

struct
{
	rex_int width;
	rex_int height;
	rex_int bpp;
	rex_string title;
	void *platform;
} GraphicsContext;

// Initialize a platform-specific graphics context.
// On a multitasking OS, this will be a window.
// On other operating systems, this will be a fullscreen graphics context.
bool Init_Graphics(rex_int width, rex_int height, rex_int bpp, const char *title)
{
	// If there's already a context, return false
	if (GraphicsContext.platform)
	{
		Message("Init_Graphics()", "Cannot initialize graphics, there is already one initialzied!", WARNING);
		return false;
	}

	// Set variables
	GraphicsContext.width = width;
	GraphicsContext.height = height;
	GraphicsContext.bpp = bpp;
	GraphicsContext.title = title;

	GraphicsContext.platform = Platform_Init_Graphics(width, height, bpp, title);

	if (GraphicsContext.platform == NULL)
	{
		Message("Init_Graphics()", "Failued to initialzie platform graphics context!", FAILURE);
		return false;
	}

	return true;
}

// Quit graphics context
bool Quit_Graphics()
{
	// Clear platform-specific context
	Platform_Quit_Graphics(GraphicsContext.platform);

	return true;
}

} // namespace Rex
