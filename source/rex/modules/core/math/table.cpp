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
		sin[i] = SCALAR(std::sin(i / 180 * PI));
		cos[i] = SCALAR(std::cos(i / 180 * PI));
		tan[i] = SCALAR(std::tan(i / 180 * PI));
	}	
}
