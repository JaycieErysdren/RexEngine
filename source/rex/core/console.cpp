// ========================================================
//
// FILE:			/source/rex/modules/core/io/console.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Console implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

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

//
//
// Global variables
//
//

Surface pic_console;
char *console_text_buffer;
char *console_text_buffer_backup;

//
//
// Functions
//
//

// Initialize console buffer
void ConsoleInitialize()
{
	SurfaceCreate(&pic_console, 40, 10, 8, 0, 0);
	console_text_buffer = (char *)calloc(256, sizeof(char));
	console_text_buffer_backup = console_text_buffer;
}

// Destroy console buffer
void ConsoleShutdown()
{
	SurfaceDestroy(&pic_console);
	console_text_buffer = console_text_buffer_backup;
	if (console_text_buffer) free(console_text_buffer);
}

// Add text to the console buffer
void ConsoleAddText(rex_int32 x, rex_int32 y, const char *text)
{
	rex_uint8 *p;
	for (p = &pic_console.scanlines.b[y][x]; (*p++ = *text++) != 0;);
}

// Add formatted text to the console buffer (doesn't work)
void ConsoleAddTextF(rex_int32 x, rex_int32 y, const char *fmt, ...)
{
	va_list args;
	rex_uint8 *p;
	char text[256];

	va_start(args, fmt);
	vsprintf(text, fmt, args);
	va_end(args);

	for (p = &pic_console.scanlines.b[y][x]; (*p++ = (*text)++) != 0;);
}

// Render the console to the given buffer
void ConsoleRender(Surface *dst, Surface *font, rex_int32 font_size)
{
	rex_int32 x, y;

	for (y = pic_console.height; y--;)
	{
		for (x = pic_console.width; x--;)
		{
			rex_int32 xx = x * font_size, yy = y * font_size;

			rex_int32 c = pic_console.scanlines.b[y][x] << 3;

			SurfaceBlit8(dst, xx, yy, xx + font_size, yy + font_size, font, c, 0, c + 8, 8, COLORKEY);
		}
	}
}

// Add formatted text to the console buffer (doesn't work)
void ConsoleTextF(Surface *dst, Surface *font, rex_int32 font_size, rex_int32 col, rex_int32 row, const char *fmt, ...)
{
	SurfaceClear(&pic_console, 0);

	console_text_buffer = console_text_buffer_backup;

	va_list args;
	rex_uint8 *p;

	va_start(args, fmt);
	vsprintf(console_text_buffer, fmt, args);
	va_end(args);

	for (p = &pic_console.scanlines.b[row][col]; (*p++ = *console_text_buffer++) != 0;);

	ConsoleRender(dst, font, font_size);
}

} // namespace Rex
