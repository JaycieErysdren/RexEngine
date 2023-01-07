// ========================================================
//
// FILE:			/source/rex/core/io.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: I/O implementation
//
// LAST EDITED:		January 6th, 2023
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: I/O
//
//

namespace Rex
{

//
//
// Functions
//
//

// Returns true if the specified keyboard scancode is currently pressed
bool KeyboardGet(rex_int scancode)
{
	return Platform_KeyboardGet(scancode);
}

} // namespace Rex
