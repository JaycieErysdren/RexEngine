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
// LAST EDITED:		November 30th, 2022
//
// ========================================================

//
// Conversion macros
//

#define X2I(a)				((a) >> 16)
#define I2X(a)				((a) << 16)
#define X2F(a)				(((float)(a)) / I2X(1))
#define F2X(a)				((int)((a) * I2X(1)))

//
// Useful macros
//

#define MIN(a, b)			(((a) < (b)) ? (a) : (b))	// min: Choose smaller of two values.
#define MAX(a, b)			(((a) > (b)) ? (a) : (b))	// max: Choose bigger of two values.
#define CLAMP(a, min, max)	MIN(MAX(a, min), max)		// clamp: Clamp value into set range.

//
// Math-specific types & macros
//

#if BASED_FIXED

// 1 in various fixed-point forms
#define ONE_LS				(1 << 16)
#define ONE_LSF				(1 << 15)
#define ONE_LU				(1 << 16)
#define ONE_LUF				(1 << 16)

#define ONE_SS				(1 << 8)
#define ONE_SSF				(1 << 7)
#define ONE_SU				(1 << 8)
#define ONE_SUF				(1 << 8) 

// Fixed point helpers
#define SCALAR_EPSILON		0x0001
#define SCALAR_MAX			0x7fffffff
#define SCALAR_MIN			0x80000000

// Base types
typedef int64_t				scalar_t;
typedef int16_t				fraction_t;
typedef uint16_t			ufraction_t;

// Macros for static initialization
#define SCALAR(a)			((scalar_t)(ONE_LS * (a)))
#define FRACTION(a)			((fraction_t)((ONE_LSF * (a)) >= ONE_LSF ? ONE_LSF - 1 : ONE_LSF * (a)))
#define UFRACTION(a)		((ufraction_t)((ONE_LUF * (a)) >= ONE_LUF ? ONE_LUF - 1 : ONE_LUF * (a)))

#elif BASED_FLOAT

// Base types
typedef float				scalar_t;
typedef float				fraction_t;
typedef float				ufraction_t;

// Macros for static initialization
#define SCALAR(a)			((scalar_t)(a))
#define FRACTION(a)			((fraction_t)(a))
#define UFRACTION(a)		((ufraction_t)(a))

// Math macros
#define DIV(a, b)			((a) / (b))
#define MUL(a, b)			((a) * (b))

#endif
