// ========================================================
//
// FILE:			/source/core/console.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Console namespace header
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

// Console namespace (public)
namespace Console
{
	// Initialize console buffer
	void Initialize();

	// Add text to the console buffer
	void AddText(int x, int y, const char *text);

	// Add formatted text to the console buffer (doesn't work)
	void AddTextF(int x, int y, const char *fmt, ...);

	// Render the console to the given buffer
	void Render(Picture::pic_t *dst, Picture::pic_t *font);
}
