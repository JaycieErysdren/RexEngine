// ========================================================
//
// FILE:			/source/math/math_x.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Fixed-point math header
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

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
	return ((int64_t)a * b) >> 16;
}

int fixdiv(int a, int b)
{
	return ((int64_t)a << 16) / b;
}

int fixdot3(int a1, int b1, int a2, int b2, int a3, int b3)
{
	return ((int64_t)a1 * b1 + (int64_t)a2 * b2 + (int64_t)a3 * b3) >> 16;
}
