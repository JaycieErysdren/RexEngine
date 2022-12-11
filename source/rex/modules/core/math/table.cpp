// ========================================================
//
// FILE:			/source/rex/modules/core/math/table.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		MathTable class implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// This module's headers
#include "math.hpp"

// Class initializer
MathTable::MathTable()
{
	for (rex_int32 i = 0; i < 360; i++)
	{
		sin[i] = REX_SCALAR(std::sin(i / 180 * PI));
		cos[i] = REX_SCALAR(std::cos(i / 180 * PI));
		tan[i] = REX_SCALAR(std::tan(i / 180 * PI));
	}	
}
