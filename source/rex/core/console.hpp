// ========================================================
//
// FILE:			/source/rex/modules/core/io/console.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Console header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

#ifndef __REX_CONSOLE_H__
#define __REX_CONSOLE_H__

//
//
// Rex namespace: Console
//
//

namespace Rex
{

//
//
// Functions
//
//

// Initialize console buffer
void ConsoleInitialize();

// Destroy console buffer
void ConsoleShutdown();

// Add text to the console buffer
void ConsoleAddText(rex_int32 x, rex_int32 y, const char *text);

// Add formatted text to the console buffer (doesn't work)
void ConsoleAddTextF(rex_int32 x, rex_int32 y, const char *fmt, ...);

// Render the console to the given buffer
void ConsoleRender(Surface *dst, Surface *font, rex_int32 font_size);

void ConsoleTextF(Surface *dst, Surface *font, rex_int32 font_size, rex_int32 x, rex_int32 y, const char *fmt, ...);

} // namespace Rex

#endif // __REX_CONSOLE_H__
