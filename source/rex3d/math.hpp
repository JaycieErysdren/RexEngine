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



#elif BASED_FLOAT

#define SCALAR(a)		((scalar_t)(a))
#define DIV(a, b)		((a)/(b))
#define MUL(a, b)		((a)*(b))

#endif
