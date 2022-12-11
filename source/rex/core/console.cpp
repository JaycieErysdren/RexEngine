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

//
//
// Functions
//
//

// Initialize console buffer
void ConsoleInitialize()
{
	SurfaceCreate(&pic_console, 40, 10, 8, 0, 0);
}

// Destroy console buffer
void ConsoleShutdown()
{
	SurfaceDestroy(&pic_console);
}

// Add text to the console buffer
void ConsoleAddText(int x, int y, const char *text)
{
	uint8_t *p;
	for (p = &pic_console.scanlines.b[y][x]; (*p++ = *text++) != 0;);
}

// Add formatted text to the console buffer (doesn't work)
void ConsoleAddTextF(int x, int y, const char *fmt, ...)
{
	va_list args;
	uint8_t *p;
	char text[256];

	va_start(args, fmt);
	vsprintf(text, fmt, args);
	va_end(args);

	for (p = &pic_console.scanlines.b[y][x]; (*p++ = (*text)++) != 0;);
}

// Render the console to the given buffer
void ConsoleRender(Surface *dst, Surface *font, int font_size)
{
	int x, y;

	for (y = pic_console.height; y--;)
	{
		for (x = pic_console.width; x--;)
		{
			int xx = x * font_size, yy = y * font_size;

			int c = pic_console.scanlines.b[y][x] << 3;

			SurfaceBlit8(dst, xx, yy, xx + font_size, yy + font_size, font, c, 0, c + 8, 8, COLORKEY);
		}
	}
}

} // namespace Rex
