// ========================================================
//
// FILE:			/source/rex/modules/core/math/fixed.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Fixed-point math implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// This module's headers
#include "math.hpp"

//
//
// Functions
//
//

int fixinv(int a)
{
	return fixdiv(i2f(1), a); 
}

int fixmul(int a, int b)
{
	return ((rex_int64)a * b) >> 16;
}

int fixdiv(int a, int b)
{
	return ((rex_int64)a << 16) / b;
}

int fixdot3(int a1, int b1, int a2, int b2, int a3, int b3)
{
	return ((rex_int64)a1 * b1 + (rex_int64)a2 * b2 + (rex_int64)a3 * b3) >> 16;
}
