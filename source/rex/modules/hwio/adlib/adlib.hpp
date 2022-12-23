// ========================================================
//
// FILE:			/source/rex/modules/fileio/adlib/adlib.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		ADLIB namespace: Header
//
// LAST EDITED:		December 22nd, 2022
//
// ========================================================

#ifndef __ADLIB_H__
#define __ADLIB_H__

#define ADLIB_PORT 0x388

//
//
// ADLIB namespace: Header
//
//

namespace ADLIB
{

//
//
// Functions
//
//

// Send an output to the AdLib ports
void Output(rex_uint8 reg, rex_uint8 data);

// Reset the AdLib card
void Reset();

// Clear all AdLib registers
void ClearRegisters();

// Detect if an AdLib card is present
bool Detect();

} // namespace ADLIB

#endif // __ADLIB_H__
