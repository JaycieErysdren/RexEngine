// ========================================================
//
// FILE:			/source/rex/core/log.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Surface header
//
// LAST EDITED:		January 8th, 2023
//
// ========================================================

#ifndef __REX_SURFACE_H__
#define __REX_SURFACE_H__

//
//
// Rex namespace: Surface
//
//

namespace Rex
{

//
//
// Enums
//
//

typedef enum
{
	// text effects
	ANSI_RESET = 0,
	ANSI_BRIGHT_ON = 1,
	ANSI_UNDERLINE_ON = 4,
	ANSI_INVERT_ON = 7,
	ANSI_BRIGHT_OFF = 21,
	ANSI_UNDERLINE_OFF = 24,
	ANSI_INVERT_OFF = 27,

	// foreground colors
	ANSI_FG_BLACK = 30,
	ANSI_FG_RED = 31,
	ANSI_FG_GREEN = 32,
	ANSI_FG_YELLOW = 33,
	ANSI_FG_BLUE = 34,
	ANSI_FG_MAGENTA = 35,
	ANSI_FG_CYAN = 36,
	ANSI_FG_WHITE = 37,

	// background colors
	ANSI_BG_BLACK = 40,
	ANSI_BG_RED = 41,
	ANSI_BG_GREEN = 42,
	ANSI_BG_YELLOW = 43,
	ANSI_BG_BLUE = 44,
	ANSI_BG_MAGENTA = 45,
	ANSI_BG_CYAN = 46,
	ANSI_BG_WHITE = 47,
} ansi_codes;

typedef enum
{
	RAW = 1,
	PNG,
	BMP,
	TGA,
	JPG,
} surface_save_format;

//
//
// Classes
//
//

// Color
class Color
{
	public:

		// constructors
		Color();
		Color(rex_uint8 c);
		Color(rex_uint8 r, rex_uint8 g, rex_uint8 b);
		Color(rex_uint8 r, rex_uint8 g, rex_uint8 b, rex_uint8 a);

		// retrieve and set 8-bit color value
		rex_uint8 Color8();
		rex_uint8 Color8(rex_uint8 c);

		// retrieve and set 16-bit color value
		rex_uint16 Color16();
		rex_uint16 Color16(rex_uint8 r, rex_uint8 g, rex_uint8 b);

		// retrieve and set 32-bit color value
		rex_uint32 Color32();
		rex_uint32 Color32(rex_uint8 r, rex_uint8 g, rex_uint8 b, rex_uint8 a);

		// retrieve and set red component
		rex_uint8 R();
		rex_uint8 R(rex_uint8 r);

		// retrieve and set green component
		rex_uint8 G();
		rex_uint8 G(rex_uint8 g);

		// retrieve and set blue component
		rex_uint8 B();
		rex_uint8 B(rex_uint8 b);

		// retrieve and set alpha component
		rex_uint8 A();
		rex_uint8 A(rex_uint8 a);

		// retrieve color depth
		rex_uint8 Depth();

		// operators
		void operator=(const rex_int index);
		//void operator=(const rex_int rgba16[3]);
		void operator=(const rex_int rgba32[4]);

	private:

		// type of color this is
		enum
		{
			NONE,
			U8,
			U16,
			U32,
		} tag;

		// color value
		union
		{
			rex_uint8 u8;
			rex_uint16 u16;
			rex_uint32 u32;
		} val;
};

// Surface
class Surface
{
	public:
		//
		// Constructors/Destructors
		//

		// Constructors
		Surface();
		Surface(rex_uint width, rex_uint height, rex_uint bpp);
		Surface(rex_uint width, rex_uint height, rex_uint bpp, void *pixels);
		Surface(const char *filename);

		// Destructor
		~Surface();

		//
		// Create/Destroy
		//

		// Create blank surface
		bool Create(rex_uint width, rex_uint height, rex_uint bpp);

		// Create surface from pixel buffer
		bool Create(rex_uint width, rex_uint height, rex_uint bpp, void *pixels);

		// Destroy the surface pixel buffer, leaving it ready to reuse
		bool Destroy();

		//
		// Load/Save
		//

		// Load image into Surface
		bool Load(const char *filename);

		// Load image into Surface, with desired BPP
		bool Load(const char *filename, rex_int desired_bpp);

		// Save Surface to file
		bool Save(const char *filename);
		bool Save(const char *filename, surface_save_format format);

		//
		// Display
		//

		// Display this surface on screen
		bool Flip();

		//
		// Clearing functions
		//

		bool Clear();
		bool Clear(Color *color);

		//
		// Drawing functions
		//

		// Plot a single pixel
		bool DrawPixel(rex_int x, rex_int y, Color &color);

		//
		// Return read-only info
		//

		rex_int Width();
		rex_int Height();
		rex_int BPP();
		rex_int BytesPerRow();

	private:
		// read-only variables
		rex_int width;
		rex_int height;
		rex_int bpp;
		rex_int bytes_per_row;
		void *pixels;
};

} // namespace Rex

#endif // __REX_SURFACE_H__
