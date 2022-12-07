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
#define FIX16(a)					((fix16_t)((a) * (1 << 8)))
#define FIX32(a)					((fix32_t)((a) * (1 << 16)))

#define INT16(a)					((int16_t)((a) >> 8))
#define INT32(a)					((int32_t)((a) >> 16))

#define FLOAT16(a)					(((float)(a) / (1 << 8)))
#define FLOAT32(a)					(((float)(a) / (1 << 16)))

// 32-bit fixed point operation shortcut macros
#define ADD32(x, y)					((x) + (y))
#define SUB32(x, y)					((x) - (y))
#define MUL32(x, y)					((fix32_t)(((int64_t)(x) * (y)) >> 16))
#define DIV32(x, y)					((fix32_t)(((int64_t)(x) << 16) / (y)))
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
#define DIV16(x, y)					(((x) << FIX16_FRAC_BITS) / (y))
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

#ifdef UGH

// Rect
typedef struct
{
	int32_t x1, y1, x2, y2;
} rect_t;

#endif

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
// Angles
//

#ifdef AGHH

typedef fixed_32uf_t				angle_t;

#define ANGLE_DEG(deg)				((angle_t)(int32_t)((deg) * 182))
#define ANGLE_RAD(rad)				((angle_t)(int32_t)((rad) * 10430))

#define AngleToDegree(a)			MUL((a), SCALAR(360.0f))
#define DegreeToAngle(d)			((angle_t)BR_MULDIV((d), SCALAR(1.0f), SCALAR(360.0f)))
#define AngleToRadian(a)			MUL((a), SCALAR(2.0f * PI))
#define RadianToAngle(r)			((angle_t)(MUL((r), SCALAR(0.5f / PI))))
#define DegreeToRadian(d)			(BR_MULDIV((d),SCALAR(PI), SCALAR(180.0f)))
#define RadianToDegree(r)			(MUL((r), SCALAR(180.0f / PI)))

#define AngleToScalar(a)			((scalar_t)(a))
#define ScalarToAngle(s)			((angle_t)(int32_t)(s))

#define BR_SIN(a)					BrFixedSin(a)
#define BR_COS(a)					BrFixedCos(a)
#define BR_ASIN(a)					BrFixedASin(a)
#define BR_ACOS(a)					BrFixedACos(a)
#define BR_ATAN2(a,b)				BrFixedATan2(a,b)
#define BR_ATAN2FAST(a,b)			BrFixedATan2Fast(a,b)

#endif
