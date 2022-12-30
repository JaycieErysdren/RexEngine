// ========================================================
//
// FILE:			/source/rex/platform/null.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine: Null Platform implementation
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex Engine: Null Platform
//
//

namespace Rex
{

//
//
// Functions
//
//

//
// Bootstrap
//

// Initialize all interrupts
bool Platform_Init()
{
	return false;
}

// Free all interrupts
bool Platform_Quit()
{
	return false;
}

//
// Devices
//

// Read data from all currently connected input devices (keyboards, mice, etc)
bool Platform_ReadInputDevices()
{
	return false;
}

// Get mouse data
bool Platform_MouseGet(rex_int *mx, rex_int *my, rex_int *mb)
{
	return false;
}

// Set mouse data
bool Platform_MouseSet(rex_int mx, rex_int my, rex_int mb)
{
	return false;
}

// Returns true if the specified key is pressed
bool Platform_KeyboardGet(rex_int scancode)
{
	return false;
}

//
// Timer
//

// Returns the number of ticks elapsed since program start
rex_int64 Platform_GetTicks64()
{
	return -1;
}

} // namespace Rex
