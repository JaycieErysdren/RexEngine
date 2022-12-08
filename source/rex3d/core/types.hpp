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
// LAST EDITED:		December 7th, 2022
//
// ========================================================

//
// Graphics definitions
//

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

//
// Struct packing
//

#define ATTR_PACKED					__attribute__ ((packed))

//
// Shortcut macros
//

#define MIN(a, b)					(((a) < (b)) ? (a) : (b))
#define MAX(a, b)					(((a) > (b)) ? (a) : (b))
#define CLAMP(a, min, max)			MIN(MAX(a, min), max)
#define ABS(a)						(((a) < 0) ? -(a) : (a))
#define SGN(a)						((a < 0) ? -1 : ((a > 0) ? 1 : 0))

//
// Portal2D
//

// Fixed-point conversion macros
#define f2i(a) ((a) >> 16)
#define i2f(a) ((a) << 16)
#define f2fl(a) (((float)(a)) / i2f(1))
#define fl2f(a) ((int)((a) * i2f(1)))

// Fixed-point ceiling macro
#define fixceil(a) (((a) + 0xFFFF) >> 16)

//
// Fixed-point types & macros
//

// Various fixed-point types
typedef int32_t fixed_32s_t;		// 32 bit signed fixed				15.16
typedef int16_t fixed_32sf_t;		// 32 bit signed fixed fraction		0.15

typedef uint32_t fixed_32u_t;		// 32 bit unsigned fixed			16.16
typedef uint16_t fixed_32uf_t;		// 32 bit unsigned fixed fraction	0.16

typedef int16_t fixed_16s_t;		// 16 bit signed fixed				7.8
typedef int8_t fixed_16sf_t;		// 16 bit signed fixed fraction		0.7

typedef uint16_t fixed_16u_t;		// 16 bit unsigned fixed			8.8
typedef int8_t fixed_16uf_t;		// 16 bit unsigned fixed fraction	0.8

// Base fixed-point types (scalar)
typedef fixed_32s_t					scalar_t;
typedef fixed_32sf_t				fraction_t;
typedef fixed_32uf_t				ufraction_t;

#define SCALAR_EPSILON				0x0001
#define SCALAR_MAX					0x7fffffff
#define SCALAR_MIN					0x80000000

// 1 in various fixed-point forms
#define X_ONE_32S					(1 << 16)
#define X_ONE_32SF					(1 << 15)
#define X_ONE_32U					(1 << 16)
#define X_ONE_32UF					(1 << 16)

#define X_ONE_16S					(1 << 8)
#define X_ONE_16SF					(1 << 7)
#define X_ONE_16U					(1 << 8)
#define X_ONE_16UF					(1 << 8) 

//
// Macros for static initialization
//

// Initialize scalar
#define SCALAR(x)					((scalar_t)(X_ONE_32S * (x)))

// Initialize signed fraction
#define FRACTION(x)					((fraction_t)((X_ONE_32SF * (x)) >= X_ONE_32SF ? X_ONE_32SF - 1 : X_ONE_32SF * (x)))

// Initialize unsigned fraction
#define UFRACTION(x)				((ufraction_t)((X_ONE_32UF * (x)) >= X_ONE_32UF ? X_ONE_32UF - 1 : X_ONE_32UF * (x)))

//
// Macros for type conversion
//

// Float to scalar
#define FloatToScalar(f)			((scalar_t)((f) * (float)X_ONE_32S))

// Scalar to float
#define ScalarToFloat(s)			((s) / (float)X_ONE_32S)

// Integer to scalar
#define IntegerToScalar(i)			((scalar_t)((i) * (int32_t)X_ONE_32S))

// Scalar to integer
#define ScalarToInteger(s)			((int32_t)(s) / (int32_t)X_ONE_32S)

// Fixed to scalar
#define FixedToScalar(f)			(f)

// Scalar to fixed
#define ScalarToFixed(s)			(s)

// Signed fraction to scalar
#define FractionToScalar(f)			((scalar_t)((f) * 2))

// Scalar to signed fraction
#define ScalarToFraction(s)			((fraction_t)((s) / 2))

// Unsigned fraction to scalar
#define UFractionToScalar(f)		((scalar_t)(f))

// Scalar to unsigned fraction
#define ScalarToUFraction(s)		((ufraction_t)(s))

// Integer to fixed
#define IntegerToFixed(i)			((i) << 16)

// Fixed to integer
#define FixedToInteger(i)			((i) >> 16)

// Float to fixed
#define FloatToFixed(f)				((scalar_t)((f) * (float)X_ONE_32S))

// Fixed to float
#define FixedToFloat(s)				((s) * (1.0 / (float)X_ONE_32S))

//
// Various arithmetic operations
//

#define MUL(a, b)					(((int64_t)(a) * (b)) >> 16)
#define DIV(a, b)					(((int64_t)(a) << 16) / (b))

// result: (a * b) / c
#define MULDIV(a, b, c)				(DIV(MUL((a), (b)), (c)))
#define SAFEMULDIV(a, b, c)			(SAFEDIV(MUL((a), (b)), (c)))

#define SAFEDIV(a, b)				((((a) == 0) || (b) == 0) ? SCALAR_MIN : DIV((a), (b)))

//
// Vector types & macros
//

// 2-point scalar vector
typedef struct
{
	scalar_t x, y;
} vec2s_t;

// 3-point scalar vector
typedef struct
{
	scalar_t x, y, z;
} vec3s_t;

// 2-point integer vector
typedef struct
{
	int32_t x, y;
} vec2i_t;

// 3-point integer vector
typedef struct
{
	int32_t x, y, z;
} vec3i_t;

// Macros for static initialization
#define VEC2S(a, b)					((vec2s_t){SCALAR((a)), SCALAR((b))})
#define VEC2I(a, b)					((vec2i_t){(a), (b)})
#define VEC3S(a, b, c)				((vec3s_t){SCALAR((a)), SCALAR((b)), SCALAR((c))})
#define VEC3I(a, b, c)				((vec3i_t){(a), (b), (c)})

//
// Rect
//

typedef struct
{
	int32_t x1, y1;
	int32_t x2, y2;
} rect_t;

#define RECT(x1, y1, x2, y2)		((rect_t){(x1), (y1), (x2), (y2)})
