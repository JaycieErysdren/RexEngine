// ========================================================
//
// FILE:			/source2/include/rextypes.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Rex Engine types.
//
// LAST EDITED:		November 18th, 2022
//
// ========================================================

//
// Base types
//

typedef int8_t rex_int_8;							// Signed 8-bit integer
typedef uint8_t rex_uint_8;							// Unsigned 8-bit integer

typedef int16_t rex_int_16;							// Signed 16-bit integer
typedef uint16_t rex_uint_16;						// Unsigned 16-bit integer

typedef int32_t rex_int_32;							// Signed 32-bit integer
typedef uint32_t rex_uint_32;						// Unsigned 32-bit integer

typedef int64_t rex_int_64;							// Signed 64-bit integer
typedef uint64_t rex_uint_64;						// Unsigned 64-bit integer

typedef _Float32 rex_float_32;						// 32-bit Float
typedef _Float64 rex_float_64;						// 64-bit Float

#define REX_TRUE 1									// True
#define REX_FALSE 0									// False
#define REX_NULL NULL								// Null

#define REX_MIN(a, b) ((a) < (b) ? (a) : (b))		// Returns the minimum value
#define REX_MAX(a, b) ((a) < (b) ? (b) : (a))		// Returns the maximum value

//
// Vectors
//

// 2D vector, Signed 32-bit integer
typedef struct
{
	rex_int_32 v[2];
} rex_vector2;

// 3D vector, Signed 32-bit integer
typedef struct
{
	rex_int_32 v[3];
} rex_vector3;

// 4D vector, Signed 32-bit integer
typedef struct
{
	rex_int_32 v[4];
} rex_vector4;

//
// Colors
//

// RGB structure
typedef struct
{
	rex_uint_8 r, g, b;
} rex_rgb;

// RGBA structure
typedef struct
{
	rex_uint_8 r, g, b, a;
} rex_rgba;

// rex_rgb shortcut
#define REX_RGB(R, G, B) (rex_rgb){R, G, B}

// rex_rgba shortcut
#define REX_RGBA(R, G, B, A) (rex_rgba){R, G, B, A}

// rex_rgb color shortcuts
#define REX_RGB_WHT REX_RGB(255, 255, 255)			// RGB White
#define REX_RGB_BLK REX_RGB(0, 0, 0)				// RGB Black
#define REX_RGB_RED REX_RGB(255, 0, 0)				// RGB Red
#define REX_RGB_YLW REX_RGB(128, 128, 0)			// RGB Yellow
#define REX_RGB_GRN REX_RGB(0, 255, 0)				// RGB Green
#define REX_RGB_CYN REX_RGB(0, 128, 128)			// RGB Cyan
#define REX_RGB_BLU REX_RGB(0, 0, 255)				// RGB Blue
#define REX_RGB_MGN REX_RGB(128, 0, 128)			// RGB Magenta
#define REX_RGB_GRY REX_RGB(128, 128, 128)			// RGB Grey

// rex_rgba color shortcuts
#define REX_RGBA_WHT REX_RGBA(255, 255, 255, 255)	// RGBA White
#define REX_RGBA_BLK REX_RGBA(0, 0, 0, 255)			// RGBA Black
#define REX_RGBA_RED REX_RGBA(255, 0, 0, 255)		// RGBA Red
#define REX_RGBA_YLW REX_RGBA(128, 128, 0, 255)		// RGBA Yellow
#define REX_RGBA_GRN REX_RGBA(0, 255, 0, 255)		// RGBA Green
#define REX_RGBA_CYN REX_RGBA(0, 128, 128, 255)		// RGBA Cyan
#define REX_RGBA_BLU REX_RGBA(0, 0, 255, 255)		// RGBA Blue
#define REX_RGBA_MGN REX_RGBA(128, 0, 128, 255)		// RGBA Magenta
#define REX_RGBA_GRY REX_RGBA(128, 128, 128, 255)	// RGBA Grey

// 8-bit 256 color palette structure
typedef rex_rgb rex_palette[256];
