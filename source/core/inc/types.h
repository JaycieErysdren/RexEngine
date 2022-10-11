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
// LAST EDITED:		October 11th, 2022
//
// ========================================================

typedef int rex_int;							// Integer
typedef unsigned int rex_uint;					// Unsigned integer

typedef short rex_short;						// Short
typedef unsigned short rex_ushort;				// Unsigned short

typedef long rex_long;							// Long
typedef unsigned long rex_ulong;				// Unsigned long

typedef long long rex_double;					// Double
typedef unsigned long long rex_udouble;			// Unsigned double

typedef char rex_byte;							// Byte
typedef unsigned char rex_ubyte;				// Unsigned byte

typedef const char rex_byte_c;					// Const byte
typedef const unsigned char rex_ubyte_c;		// Unsigned const byte

#define REX_TRUE 1								// True
#define REX_FALSE 0								// False

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

// External window
typedef struct
{
	rex_coord_screen pos;
	SDL_Window *window;
	rex_byte_c *title;
	rex_int width;
	rex_int height;
} rex_window_external;
