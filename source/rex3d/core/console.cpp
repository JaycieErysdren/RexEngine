// ========================================================
//
// FILE:			/source/core/console.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Console namespace implementation
//
// LAST EDITED:		December 7th, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

// Console namespace (private)
namespace Console
{

//
//
// Global variables
//
//

Picture::pic_t pic_console;

//
//
// Functions
//
//

// Initialize console buffer
void Initialize()
{
	Picture::Create(&pic_console, 40, 10, 8, 0, 0);
}

// Destroy console buffer
void Shutdown()
{
	Picture::Destroy(&pic_console);
}

// Add text to the console buffer
void AddText(int x, int y, const char *text)
{
	uint8_t *p;
	for (p = &pic_console.scanlines.b[y][x]; (*p++ = *text++) != 0;);
}

// Add formatted text to the console buffer (doesn't work)
void AddTextF(int x, int y, const char *fmt, ...)
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
void Render(Picture::pic_t *dst, Picture::pic_t *font, int font_size)
{
	int x, y;

	for (y = pic_console.height; y--;)
	{
		for (x = pic_console.width; x--;)
		{
			int xx = x * font_size, yy = y * font_size;

			int c = pic_console.scanlines.b[y][x] << 3;

			Picture::Blit8(dst, xx, yy, xx + font_size, yy + font_size, font, c, 0, c + 8, 8, Picture::COLORKEY);
		}
	}
}

} // namespace Console
