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
	rex_ubyte r, g, b, a;
} rex_rgba;

// RGB structure
typedef struct
{
	rex_ubyte r, g, b;
} rex_rgb;

// rex_rgb shortcut
#define REX_RGB(R, G, B) (rex_rgb){R, G, B}

// rex_rgba shortcut
#define REX_RGBA(R, G, B, A) (rex_rgba){R, G, B, A}

#define REX_RGBA_RED REX_RGBA(255, 0, 0, 255)		// RGBA Red
#define REX_RGBA_YLW REX_RGBA(128, 128, 0, 255)		// RGBA Yellow
#define REX_RGBA_GRN REX_RGBA(0, 255, 0, 255)		// RGBA Green
#define REX_RGBA_CYN REX_RGBA(0, 128, 128, 255)		// RGBA Cyan
#define REX_RGBA_BLU REX_RGBA(0, 0, 255, 255)		// RGBA Blue
#define REX_RGBA_MGN REX_RGBA(128, 0, 128, 255)		// RGBA Magenta
#define REX_RGBA_GRY REX_RGBA(128, 128, 128, 255)	// RGBA Grey

#define REX_RGB_RED REX_RGB(255, 0, 0)				// RGBA Red
#define REX_RGB_YLW REX_RGB(128, 128, 0)			// RGB Yellow
#define REX_RGB_GRN REX_RGB(0, 255, 0)				// RGBA Green
#define REX_RGB_CYN REX_RGB(0, 128, 128)			// RGB Cyan
#define REX_RGB_BLU REX_RGB(0, 0, 255)				// RGBA Blue
#define REX_RGB_MGN REX_RGB(128, 0, 128)			// RGB Magenta
#define REX_RGB_GRY REX_RGB(128, 128, 128)			// RGB Grey

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
