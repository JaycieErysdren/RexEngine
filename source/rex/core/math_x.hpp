// ========================================================
//
// FILE:			/source/rex/core/math_x.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Fixed Math header
//
// LAST EDITED:		December 16th, 2022
//
// ========================================================

#ifndef __REX_FIXED_MATH_H__
#define __REX_FIXED_MATH_H__

//
//
// Rex namespace: Fixed Math
//
//

namespace Rex
{

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

} // namespace Rex

#endif // __REX_FIXED_MATH_H__
