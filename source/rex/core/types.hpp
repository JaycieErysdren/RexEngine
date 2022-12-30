// ========================================================
//
// FILE:			/source/rex/core/types.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine base types
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

#ifndef __REX_TYPES_H__
#define __REX_TYPES_H__

//
//
// Engine limits
//
//

#define REX_MAX_ACTOR 128

//
//
// Base types
//
//

typedef int8_t rex_int8;							// Signed 8-bit integer
typedef uint8_t rex_uint8;							// Unsigned 8-bit integer

typedef int16_t rex_int16;							// Signed 16-bit integer
typedef uint16_t rex_uint16;						// Unsigned 16-bit integer

typedef int32_t rex_int32;							// Signed 32-bit integer
typedef uint32_t rex_uint32;						// Unsigned 32-bit integer

typedef int64_t rex_int64;							// Signed 64-bit integer
typedef uint64_t rex_uint64;						// Unsigned 64-bit integer

typedef float rex_float;							// 32-bit Float
typedef double rex_double;							// 64-bit Float

typedef bool rex_bool;								// Boolean

typedef string rex_string;							// String

typedef rex_int32 rex_int;							// Basic signed integer
typedef rex_uint32 rex_uint;						// Basic unsigned integer

// Color union type
typedef union
{
	rex_uint8 u8;
	rex_uint16 u16;
	rex_uint32 u32;
} rex_color;

//
//
// Math
//
//

//
// Make sure PI is defined
//

#if !defined(PI)
#define PI 3.14159265358979323846
#endif

//
//
// GCC attributes
//
//

#if (REX_COMPILER == COMPILER_DJGPP)

#define ATTR_PACKED						__attribute__ ((packed))

#else

#define ATTR_PACKED

#endif

//
//
// Shortcut macros
//
//

#define REX_MIN(a, b)					(((a) < (b)) ? (a) : (b))
#define REX_MAX(a, b)					(((a) > (b)) ? (a) : (b))
#define REX_CLAMP(a, min, max)			REX_MIN(REX_MAX(a, min), max)
#define REX_ABS(a)						(((a) < 0) ? -(a) : (a))
#define REX_SGN(a)						(((a) < 0) ? -1 : (((a) > 0) ? 1 : 0))

//
//
// Fixed-point types & macros
//
//

// Various fixed-point types
typedef rex_int32 rex_fixed_32s;		// 32 bit signed fixed				15.16
typedef rex_int16 rex_fixed_32sf;		// 32 bit signed fixed fraction		0.15

typedef rex_uint32 rex_fixed_32u;		// 32 bit unsigned fixed			16.16
typedef rex_uint16 rex_fixed_32uf;		// 32 bit unsigned fixed fraction	0.16

typedef rex_int16 rex_fixed_16s;		// 16 bit signed fixed				7.8
typedef rex_int8 rex_fixed_16sf;		// 16 bit signed fixed fraction		0.7

typedef rex_uint16 rex_fixed_16u;		// 16 bit unsigned fixed			8.8
typedef rex_uint8 rex_fixed_16uf;		// 16 bit unsigned fixed fraction	0.8

// Base fixed-point types (scalar)
typedef rex_fixed_32s					rex_scalar;
typedef rex_fixed_32sf					rex_fraction;
typedef rex_fixed_32uf					rex_ufraction;

#define REX_SCALAR_EPSILON				0x0001
#define REX_SCALAR_MAX					0x7fffffff
#define REX_SCALAR_MIN					0x80000000

// 1 in various fixed-point forms
#define REX_ONE_32S						(1 << 16)
#define REX_ONE_32SF					(1 << 15)
#define REX_ONE_32U						(1 << 16)
#define REX_ONE_32UF					(1 << 16)

#define REX_ONE_16S						(1 << 8)
#define REX_ONE_16SF					(1 << 7)
#define REX_ONE_16U						(1 << 8)
#define REX_ONE_16UF					(1 << 8) 

#define REX_MASK_32S					(0xFFFF << 16)
#define REX_MASK_32SF					(0xFFFF >> 16)
#define REX_MASK_32U					(0xFFFF << 16)
#define REX_MASK_32UF					(0xFFFF >> 16)

#define REX_MASK_16S					(0xFFFF << 8)
#define REX_MASK_16SF					(0xFFFF >> 8)
#define REX_MASK_16U					(0xFFFF << 8)
#define REX_MASK_16UF					(0xFFFF >> 8)

//
// Macros for static initialization
//

// Initialize scalar
#define REX_SCALAR(x)					((rex_scalar)(REX_ONE_32S * (x)))

// Initialize signed fraction
#define REX_FRACTION(x)					((rex_fraction)((REX_ONE_32SF * (x)) >= REX_ONE_32SF ? REX_ONE_32SF - 1 : REX_ONE_32SF * (x)))

// Initialize unsigned fraction
#define REX_UFRACTION(x)				((rex_ufraction)((REX_ONE_32UF * (x)) >= REX_ONE_32UF ? REX_ONE_32UF - 1 : REX_ONE_32UF * (x)))

//
// Macros for type conversion
//

// Float to scalar
#define RexFloatToScalar(f)				((rex_scalar)((f) * (rex_float)REX_ONE_32S))

// Scalar to float
#define RexScalarToFloat(s)				((s) / (rex_float)REX_ONE_32S)

// Integer to scalar
#define RexIntegerToScalar(i)			((rex_scalar)((i) * (rex_int32)REX_ONE_32S))

// Scalar to integer
#define RexScalarToInteger(s)			((rex_int32)(s) / (rex_int32)REX_ONE_32S)

// Fixed to scalar
#define RexFixedToScalar(f)				(f)

// Scalar to fixed
#define RexScalarToFixed(s)				(s)

// Signed fraction to scalar
#define RexFractionToScalar(f)			((rex_scalar)((f) * 2))

// Scalar to signed fraction
#define RexScalarToFraction(s)			((rex_fraction)((s) / 2))

// Unsigned fraction to scalar
#define RexUFractionToScalar(f)			((rex_scalar)(f))

// Scalar to unsigned fraction
#define RexScalarToUFraction(s)			((rex_ufraction)(s))

// Integer to fixed
#define RexIntegerToFixed(i)			((i) << 16)

// Fixed to integer
#define RexFixedToInteger(i)			((i) >> 16)

// Float to fixed
#define RexFloatToFixed(f)				((rex_scalar)((f) * (rex_float)X_ONE_32S))

// Fixed to float
#define RexFixedToFloat(s)				((s) * (1.0 / (rex_float)X_ONE_32S))

//
// Various arithmetic operations
//

#define REX_MUL(a, b)					(((rex_int64)(a) * (b)) >> 16)
#define REX_DIV(a, b)					(((rex_int64)(a) << 16) / (b))
#define REX_MULDIV(a, b, c)				(REX_DIV(REX_MUL((a), (b)), (c)))

#define REX_SAFEDIV(a, b)				((((a) == 0) || (b) == 0) ? REX_SCALAR_MIN : REX_DIV((a), (b)))
#define REX_SAFEMULDIV(a, b, c)			(REX_SAFEDIV(REX_MUL((a), (b)), (c)))

#define REX_FLOOR(a)					((a) & REX_MASK_32S)
#define REX_CEIL(a)						(((a) & REX_MASK_32SF == 0) ? (a) : ((a) + REX_ONE_32S) & REX_MASK_32S)

#define REX_SQRT(a)						(sqrt((double)(a)))

//
// Vector types & macros
//

// 2-point scalar vector
typedef struct
{
	rex_scalar x, y;
} rex_vec2s;

// 3-point scalar vector
typedef struct
{
	rex_scalar x, y, z;
} rex_vec3s;

// 2-point integer vector
typedef struct
{
	rex_int32 x, y;
} rex_vec2i;

// 3-point integer vector
typedef struct
{
	rex_int32 x, y, z;
} rex_vec3i;

// 2-point float vector
typedef struct
{
	rex_float x, y;
} rex_vec2f;

// 3-point float vector
typedef struct
{
	rex_float x, y, z;
} rex_vec3f;

// Macros for static initialization
#define REX_VEC2S(a, b)					((rex_vec2s){REX_SCALAR((a)), REX_SCALAR((b))})
#define REX_VEC2I(a, b)					((rex_vec2i){(a), (b)})
#define REX_VEC3S(a, b, c)				((rex_vec3s){REX_SCALAR((a)), REX_SCALAR((b)), REX_SCALAR((c))})
#define REX_VEC3I(a, b, c)				((rex_vec3i){(a), (b), (c)})

// Lerp
#define REX_LERP(a, b, c)				((rex_scalar)(REX_MUL((a), (c)) + REX_MUL((b), REX_SCALAR(1.0f) - (c))))
#define REX_LERP_VEC2S(a, b, c)			((rex_vec2s){REX_LERP((a.x), (b.x), (c)), REX_LERP((a.y), (b.y), (c))})

// Dot product
#define REX_DOT_VEC2S(a, b)				(REX_MUL((a.x), (b.x)) + REX_MUL((a.y), (b.y)))
#define REX_DOT_VEC3S(a, b)				(REX_MUL((a.x), (b.x)) + REX_MUL((a.y), (b.y)) + REX_MUL((a.z), (b.z)))

#endif // __REX_TYPES_H__
