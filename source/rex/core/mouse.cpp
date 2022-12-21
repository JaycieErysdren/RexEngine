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

// Returns true if the mouse was successfully able to be read
bool MouseRead(rex_int32 *btns, rex_int32 *x, rex_int32 *y)
{
	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_DOS)

		int16_t mx, my, mb;

		mb = DOS::MouseRead(&mx, &my);

		*btns = (rex_int32)mb;
		*x = (rex_int32)mx;
		*y = (rex_int32)my;

		return true;

	#else

	return false;

	#endif
}

// Set the current mouse position
void MouseSet(rex_int32 x, rex_int32 y)
{
	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_DOS)

		DOS::MouseSet((int16_t)x, (int16_t)y);

	#endif
}

} // namespace Rex
