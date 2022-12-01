// ========================================================
//
// FILE:			/source/math/sintable.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Fixed-point sin table header
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

extern int sintable[2048];

#define fixsin(a) sintable[(a) & 2047]
#define fixcos(a) sintable[((a) + 512) & 2047]
