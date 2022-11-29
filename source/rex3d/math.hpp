// ========================================================
//
// FILE:			/source/math.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Math namespace header
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

//
//
// Math namespace
//
//

// Math namespace definition (public)
namespace Math
{

}

//
//
// Definitions
//
//

// Fixed or floating point macros
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

// Macros for static initialization
#define SCALAR(a)			((scalar_t)(ONE_LS * (a)))
#define FRACTION(a)			((fraction_t)((ONE_LSF * (a)) >= ONE_LSF ? ONE_LSF - 1 : ONE_LSF * (a)))
#define UFRACTION(a)		((ufraction_t)((ONE_LUF * (a)) >= ONE_LUF ? ONE_LUF - 1 : ONE_LUF * (a)))

#elif BASED_FLOAT

// Macros for static initialization
#define SCALAR(a)			((scalar_t)(a))
#define FRACTION(a)			((fraction_t)(a))
#define UFRACTION(a)		((ufraction_t)(a))

// Math macros
#define DIV(a, b)			((a) / (b))
#define MUL(a, b)			((a) * (b))

#endif
