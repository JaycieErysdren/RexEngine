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
// LAST EDITED:		October 21st, 2022
//
// ========================================================

//
// Base types
//

typedef signed int rex_int;							// Signed integer
typedef unsigned int rex_uint;						// Unsigned integer

typedef signed short int rex_short;					// Signed short
typedef unsigned short int rex_ushort;				// Unsigned short

typedef signed long int rex_long;					// Signed long
typedef unsigned long int rex_ulong;				// Unsigned long

typedef signed long long int rex_double;			// Signed double
typedef unsigned long long int rex_udouble;			// Unsigned double

typedef signed char rex_byte;						// Signed byte
typedef unsigned char rex_ubyte;					// Unsigned byte

typedef const signed char rex_byte_c;				// Signed const byte
typedef const unsigned char rex_ubyte_c;			// Unsigned const byte

typedef float rex_float;							// Float

typedef rex_int rex_bool;							// Boolean

typedef void rex_buffer;							// Buffer

typedef intptr_t rex_int_ptr;						// Void * pointer
typedef uintptr_t rex_uint_ptr;						// Unsigned void * pointer

#define REX_TRUE 1									// True
#define REX_FALSE 0									// False
#define REX_NULL NULL								// Null

//
// Simple vectors
//

typedef rex_int rex_vector2i[2];					// Simple 2D vector, int
typedef rex_int rex_vector3i[3];					// Simple 3D vector, int
typedef rex_int rex_vector4i[4];					// Simple 4D vector, int

typedef rex_long rex_vector2l[2];					// Simple 2D vector, long
typedef rex_long rex_vector3l[3];					// Simple 3D vector, long
typedef rex_long rex_vector4l[4];					// Simple 4D vector, long

typedef rex_float rex_vector2f[2];					// Simple 2D vector, float
typedef rex_float rex_vector3f[3];					// Simple 3D vector, float
typedef rex_float rex_vector4f[4];					// Simple 4D vector, float

//
// Coordinates
//

// 2D coordinate, int
typedef struct
{
	rex_int x, y;
} rex_coord2i;

// 3D coordinate, int
typedef struct
{
	rex_int x, y, z;
} rex_coord3i;

// 2D coordinate, long
typedef struct
{
	rex_long x, y;
} rex_coord2l;

// 3D coordinate, long
typedef struct
{
	rex_long x, y, z;
} rex_coord3l;

// 2D coordinate, float
typedef struct
{
	rex_float x, y;
} rex_coord2f;

// 3D coordinate, float
typedef struct
{
	rex_float x, y, z;
} rex_coord3f;

//
// AABB
//

// 2D axis-aligned bounding box
typedef struct
{
	rex_coord2i min, max;
} rex_aabb2i;

// 3D axis-aligned bounding box
typedef struct
{
	rex_coord3i min, max;
} rex_aabb3i;

//
// Colors and palettes
//

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

#define REX_RGBA_WHT REX_RGBA(255, 255, 255, 255)	// RGBA White
#define REX_RGBA_BLK REX_RGBA(0, 0, 0, 255)			// RGBA Black
#define REX_RGBA_RED REX_RGBA(255, 0, 0, 255)		// RGBA Red
#define REX_RGBA_YLW REX_RGBA(128, 128, 0, 255)		// RGBA Yellow
#define REX_RGBA_GRN REX_RGBA(0, 255, 0, 255)		// RGBA Green
#define REX_RGBA_CYN REX_RGBA(0, 128, 128, 255)		// RGBA Cyan
#define REX_RGBA_BLU REX_RGBA(0, 0, 255, 255)		// RGBA Blue
#define REX_RGBA_MGN REX_RGBA(128, 0, 128, 255)		// RGBA Magenta
#define REX_RGBA_GRY REX_RGBA(128, 128, 128, 255)	// RGBA Grey

#define REX_RGB_WHT REX_RGB(255, 255, 255)			// RGB White
#define REX_RGB_BLK REX_RGB(0, 0, 0)				// RGB Black
#define REX_RGB_RED REX_RGB(255, 0, 0)				// RGB Red
#define REX_RGB_YLW REX_RGB(128, 128, 0)			// RGB Yellow
#define REX_RGB_GRN REX_RGB(0, 255, 0)				// RGB Green
#define REX_RGB_CYN REX_RGB(0, 128, 128)			// RGB Cyan
#define REX_RGB_BLU REX_RGB(0, 0, 255)				// RGB Blue
#define REX_RGB_MGN REX_RGB(128, 0, 128)			// RGB Magenta
#define REX_RGB_GRY REX_RGB(128, 128, 128)			// RGB Grey

// 256 color palette structure (RGB)
typedef rex_rgb rex_palette[256];

//
// Windows
//

// External window
typedef struct
{
	rex_coord2i pos;
	SDL_Window *sdl_window;
	rex_byte_c *title;
	rex_int width;
	rex_int height;
	br_pixelmap *buffer_screen;
	br_pixelmap *buffer_color;
	br_pixelmap *buffer_depth;
} rex_window_external;

//
// Resources
//

// Resource container
typedef struct
{
	rex_byte identiifer[32];
	rex_int type;
	rex_buffer *data;
} rex_resource;

// Resource file header
typedef struct
{
	rex_byte identifier[16];
} rex_resfile_header;
