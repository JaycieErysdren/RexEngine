// ========================================================
//
// FILE:			/source/math/math_x.hpp
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

//
//
// Functions
//
//

int fixinv(int a);
int fixmul(int a, int b);
int fixdiv(int a, int b);
int fixdot3(int a1, int b1, int a2, int b2, int a3, int b3);

//
// Fixed sin, cos, tan, cot
//

fix32_t sinFixed(uint8_t brads);
fix32_t cosFixed(uint8_t brads);
fix32_t tanFixed(uint8_t brads);
fix32_t cotFixed(uint8_t brads);
