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
	if (GraphicsContext.platform != NULL) return false;

	// Set variables
	GraphicsContext.width = width;
	GraphicsContext.height = height;
	GraphicsContext.bpp = bpp;
	GraphicsContext.title = title;

	GraphicsContext.platform = Platform_Init_Graphics(width, height, bpp, title);

	if (GraphicsContext.platform == NULL) return false;

	return true;
}

// Quit graphics context
bool Quit_Graphics()
{
	// Clear platform-specific context
	Platform_Quit_Graphics(GraphicsContext.platform);

	return true;
}

// Show a simple message box
bool MessageBox(const char *title, const char *message)
{
	return Platform_MessageBox(title, message);
}

} // namespace Rex
