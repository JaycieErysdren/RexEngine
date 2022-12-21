// ========================================================
//
// FILE:			/source/rex/modules/core/io/mouse.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Mouse header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

#ifndef __REX_MOUSE_H__
#define __REX_MOUSE_H__

//
//
// Rex namespace: Mouse
//
//

namespace Rex
{

//
//
// Function prototypes
//
//

// Returns true if the mouse was successfully able to be read
bool MouseRead(rex_int32 *btns, rex_int32 *x, rex_int32 *y);

// Set the current mouse position
void MouseSet(rex_int32 x, rex_int32 y);

} // namespace Rex

#endif // __REX_MOUSE_H__
