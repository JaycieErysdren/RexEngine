// ========================================================
//
// FILE:			/source/rex/modules/core/io/keyboard.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Keyboard implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Keyboard
//
//

namespace Rex
{

//
//
// Functions
//
//

// Returns true if the key is currently held down
bool KeyTest(int scancode)
{
	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_DOS)

		return DOS::KeyTest(scancode) ? true : false;

	#else

	return false;

	#endif
}

} // namespace Rex
