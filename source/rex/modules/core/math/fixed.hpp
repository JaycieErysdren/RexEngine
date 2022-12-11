// ========================================================
//
// FILE:			/source/rex/modules/core/math/fixed.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Fixed-point math header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

//
//
// Functions
//
//

int fixinv(int a);
int fixmul(int a, int b);
int fixdiv(int a, int b);
int fixdot3(int a1, int b1, int a2, int b2, int a3, int b3);

extern int sintable[2048];

#define fixsin(a) sintable[(a) & 2047]
#define fixcos(a) sintable[((a) + 512) & 2047]
