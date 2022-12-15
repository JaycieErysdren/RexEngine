// ========================================================
//
// FILE:			/source/rex/core/mathtabl.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: MathTable class
//
// LAST EDITED:		December 15th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: MathTable class
//
//

namespace Rex
{

//
//
// Functions
//
//

// Class initializer
void MathTable::Initialize()
{
	for (rex_int i = 0; i < 360; i++)
	{
		sin[i] = REX_SCALAR(std::sin(i / 180.0f * PI));
		cos[i] = REX_SCALAR(std::cos(i / 180.0f * PI));
		tan[i] = REX_SCALAR(std::tan(i / 180.0f * PI));
	}	
}

} // namespace Rex
