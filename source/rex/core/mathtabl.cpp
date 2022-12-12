// ========================================================
//
// FILE:			/source/rex/modules/core/io/mouse.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Mouse implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Mouse
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
MathTable::MathTable()
{
	for (rex_int32 i = 0; i < 360; i++)
	{
		sin[i] = REX_SCALAR(std::sin(i / 180.0f * PI));
		cos[i] = REX_SCALAR(std::cos(i / 180.0f * PI));
		tan[i] = REX_SCALAR(std::tan(i / 180.0f * PI));
	}	
}

} // namespace Rex
