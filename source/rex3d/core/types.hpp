// ========================================================
//
// FILE:			/source/core/types.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Rex3D engine types
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

//
// Engine limits
//

#define MAX_POINT				1024
#define MAX_WALL				4192
#define MAX_SECTOR				1024
#define MAX_OBJECT				1024
#define MAX_TEXTURE				256
#define MAX_POLYGON				64

//
// Graphics definitions
//

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

typedef uint8_t lightmap_t[32][256];
typedef uint8_t texture66_t[64][64];

//
// Shortcut macros
//

#define MIN(a, b)					(((a) < (b)) ? (a) : (b))
#define MAX(a, b)					(((a) > (b)) ? (a) : (b))
#define CLAMP(a, min, max)			MIN(MAX(a, min), max)
#define ABS(a)						(((a) < 0) ? -(a) : (a))
#define SGN(a)						((a < 0) ? -1 : ((a > 0) ? 1 : 0))

//
// Fixed-point types & macros
//

// 32-bit and 16-bit fixed point types
typedef int16_t						fix16_t;
typedef int32_t						fix32_t;

// Precalculated 32-bit and 16-bit sin tables
extern const fix32_t sintable32[1024];
extern const fix16_t sintable16[1024];

// 16-bit fixed point bits, masks
#define FIX16_INT_BITS				10
#define FIX16_FRAC_BITS				(16 - FIX16_INT_BITS)

#define FIX16_INT_MASK				(((1 << FIX16_INT_BITS) - 1) << FIX16_FRAC_BITS)
#define FIX16_FRAC_MASK				((1 << FIX16_FRAC_BITS) - 1)

// 32-bit fixed point bits, masks
#define FIX32_INT_BITS				22
#define FIX32_FRAC_BITS				(32 - FIX32_INT_BITS)

#define FIX32_INT_MASK				(((1 << FIX32_INT_BITS) - 1) << FIX32_FRAC_BITS)
#define FIX32_FRAC_MASK				((1 << FIX32_FRAC_BITS) - 1)

// 32-bit and 16-bit fixed point conversion macros
#define FIX16(value)				((fix16_t)((value) * (1 << FIX16_FRAC_BITS)))
#define FIX32(value)				((fix32_t)((value) * (1 << FIX32_FRAC_BITS)))

// 32-bit fixed point operation shortcut macros
#define ADD32(x, y)					((x) + (y))
#define SUB32(x, y)					((x) - (y))
#define MUL32(x, y)					(((x) >> FIX32_FRAC_BITS / 2) * ((y) >> FIX32_FRAC_BITS / 2))
#define DIV32(x, y)					(((x) << FIX32_FRAC_BITS / 2) / ((y) >> FIX32_FRAC_BITS / 2))
#define NEG32(x)					(0 - (x))
#define SIN32(x)					(sintable32[(x) & 1023])
#define COS32(x)					(sintable32[((x) + 256) & 1023])
#define MULDIV32(x, y, z)			(DIV32(MUL32((x), (y)), z))
#define INT2FIX32(x)				((fix32_t)((x) << FIX32_FRAC_BITS))
#define FIX2INT32(x)				((int32_t)((x) >> FIX32_FRAC_BITS))

// 16-bit fixed point operation shortcut macros
#define ADD16(x, y)					((x) + (y))
#define SUB16(x, y)					((x) - (y))
#define MUL16(x, y)					(((x) * (y)) >> FIX16_FRAC_BITS)
#define DIV16(x, y)					(((x) << FIX16_FRAC_BITS) / y)
#define NEG16(x)					(0 - (x))
#define SIN16(x)					(sintable16[(x) & 1023])
#define COS16(x)					(sintable16[((x) + 256) & 1023])
#define MULDIV16(x, y, z)			(DIV16(MUL16((x), (y)), z))
#define INT2FIX16(x)				((fix16_t)((x) << FIX16_FRAC_BITS))
#define FIX2INT16(x)				((int16_t)((x) >> FIX16_FRAC_BITS))

//
// Portal2D types
//

// Point
typedef struct
{
	int32_t x, y;
} point_t;

// Rect
typedef struct
{
	int32_t x1, y1, x2, y2;
} rect_t;

// Motion
typedef struct
{
	int32_t x, xx, xxx;
	int32_t y, yy, yyy;
	int32_t z, zz, zzz;
} motion_t;

// Vector
typedef struct
{
	int32_t x, y, z;
} vec3_t;

typedef int32_t plane_t[4];
typedef plane_t matrix_t[4];

//
// Portal2D fixed-point macros
//

// Fixed-point conversion macros
#define f2i(a) ((a) >> 16)
#define i2f(a) ((a) << 16)
#define f2fl(a) (((float)(a)) / i2f(1))
#define fl2f(a) ((int)((a) * i2f(1)))

// Fixed-point ceiling macro
#define fixceil(a) (((a) + 0xFFFF) >> 16)
