// ========================================================
//
// FILE:			/source/core/logic/math.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Math helper functions.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Returns a random integer between high and low.
rex_int Rex_Random(rex_int low, rex_int high)
{
	return (rand() % (high - low + 1)) + low;
}
