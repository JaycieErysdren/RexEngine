// ========================================================
//
// FILE:			/source/math/mathtabl.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		MathTable class implementation
//
// LAST EDITED:		December 7th, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

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
