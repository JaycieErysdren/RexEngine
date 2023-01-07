// ========================================================
//
// FILE:			/source/rex/core/graphics.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Graphics header
//
// LAST EDITED:		January 2nd, 2023
//
// ========================================================

#ifndef __REX_GRAPHICS_H__
#define __REX_GRAPHICS_H__

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
bool Init_Graphics(rex_int width, rex_int height, rex_int bpp, const char *title);

// Quit graphics context
bool Quit_Graphics();

} // namespace Rex

#endif // __REX_GRAPHICS_H__
