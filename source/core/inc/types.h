// ========================================================
//
// FILE:			/source/core/inc/types.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Rex Engine types.
//
// LAST EDITED:		October 13th, 2022
//
// ========================================================

typedef int rex_int;								// Integer
typedef unsigned int rex_uint;						// Unsigned integer

typedef short rex_short;							// Short
typedef unsigned short rex_ushort;					// Unsigned short

typedef long rex_long;								// Long
typedef unsigned long rex_ulong;					// Unsigned long

typedef long long rex_double;						// Double
typedef unsigned long long rex_udouble;				// Unsigned double

typedef char rex_byte;								// Byte
typedef unsigned char rex_ubyte;					// Unsigned byte

typedef const char rex_byte_c;						// Const byte
typedef const unsigned char rex_ubyte_c;			// Unsigned const byte

typedef float rex_float;							// Float

#define REX_TRUE 1									// True
#define REX_FALSE 0									// False

#define REX_RGBA_RED (rex_rgba){255, 0, 0, 255}		// RGBA Red
#define REX_RGBA_YLW (rex_rgba){128, 128, 0, 255}	// RGBA Yellow
#define REX_RGBA_GRN (rex_rgba){0, 255, 0, 255}		// RGBA Green
#define REX_RGBA_CYN (rex_rgba){0, 128, 128, 255}	// RGBA Cyan
#define REX_RGBA_BLU (rex_rgba){0, 0, 255, 255}		// RGBA Blue
#define REX_RGBA_MGN (rex_rgba){128, 0, 128, 255}	// RGBA Magenta
#define REX_RGBA_GRY (rex_rgba){128, 128, 128, 255}	// RGBA Grey

#define REX_RGB_RED (rex_rgb){255, 0, 0}			// RGBA Red
#define REX_RGB_YLW (rex_rgb){128, 128, 0}			// RGB Yellow
#define REX_RGB_GRN (rex_rgb){0, 255, 0}			// RGBA Green
#define REX_RGB_CYN (rex_rgb){0, 128, 128}			// RGB Cyan
#define REX_RGB_BLU (rex_rgb){0, 0, 255}			// RGBA Blue
#define REX_RGB_MGN (rex_rgb){128, 0, 128}			// RGB Magenta
#define REX_RGB_GRY (rex_rgb){128, 128, 128}		// RGB Grey

// 2D screen coordinate
typedef struct
{
	rex_int x, y;
} rex_coord_screen;

// 2D screen axis-aligned bounding box
typedef struct
{
	rex_coord_screen min, max;
} rex_aabb_screen;

// 3D coordinate
typedef struct
{
	rex_long x, y, z;
} rex_coord;

// 3D axis-aligned bounding box
typedef struct
{
	rex_coord min, max;
} rex_aabb;

// RGBA structure
typedef struct
{
	rex_byte r, g, b, a;
} rex_rgba;

// RGB structure
typedef struct
{
	rex_byte r, g, b;
} rex_rgb;

// 256 color palette structure (RGB)
typedef rex_rgb rex_palette[256];

// External window
typedef struct
{
	rex_coord_screen pos;
	SDL_Window *window;
	SDL_GLContext context;
	rex_byte_c *title;
	rex_int width;
	rex_int height;
} rex_window_external;
