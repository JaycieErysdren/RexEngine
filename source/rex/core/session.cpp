// ========================================================
//
// FILE:			/source/rex/core/session.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Session implementation
//
// LAST EDITED:		December 31st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Session
//
//

namespace Rex
{

//
//
// Functions
//
//

// Show a simple message box
bool MessageBox(const char *title, const char *message)
{
	return Platform_MessageBox(title, message);
}

} // namespace Rex
