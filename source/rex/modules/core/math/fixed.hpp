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

rex_int32 fixinv(rex_int32 a);
rex_int32 fixmul(rex_int32 a, rex_int32 b);
rex_int32 fixdiv(rex_int32 a, rex_int32 b);
rex_int32 fixdot3(rex_int32 a1, rex_int32 b1, rex_int32 a2, rex_int32 b2, rex_int32 a3, rex_int32 b3);

extern rex_int32 sintable[2048];

#define fixsin(a) sintable[(a) & 2047]
#define fixcos(a) sintable[((a) + 512) & 2047]
