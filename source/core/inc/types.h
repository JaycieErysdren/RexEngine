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
// LAST EDITED:		October 10th, 2022
//
// ========================================================

typedef int rex_int;
typedef unsigned int rex_uint;

typedef short rex_short;
typedef unsigned short rex_ushort;

typedef long rex_long;
typedef unsigned long rex_ulong;

typedef long long rex_double;
typedef unsigned long long rex_udouble;

typedef char rex_byte;
typedef unsigned char rex_ubyte;

typedef const char rex_byte_c;
typedef const unsigned char rex_ubyte_c;

// True
#define REX_TRUE 1

// False
#define REX_FALSE 0

// Normal 3D coordinate
typedef struct
{
	rex_int x, y, z;
} rex_coord;

// Large 3D coordinate
typedef struct
{
	rex_long x, y, z;
} rex_coord_l;

// Normal Axis-aligned bounding box
typedef struct
{
	rex_coord min, max;
} rex_aabb;

// Large Axis-aligned bounding box
typedef struct
{
	rex_coord_l min, max;
} rex_aabb_l;

// External window
typedef struct
{
	rex_coord pos;
	SDL_Window *window;
	rex_byte_c *title;
	rex_int width;
	rex_int height;
} rex_window_external;
