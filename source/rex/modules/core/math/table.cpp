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

// Rex Engine headers
#include "rex.hpp"

// Class initializer
MathTable::MathTable()
{
	for (int i = 0; i < 360; i++)
	{
		sin[i] = SCALAR(sinf(i / 180.0f * M_PI));
		cos[i] = SCALAR(cosf(i / 180.0f * M_PI));
		tan[i] = SCALAR(tanf(i / 180.0f * M_PI));
	}	
}
