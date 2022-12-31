// ========================================================
//
// FILE:			/source/rex/platform/platform.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine: Platform header
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

#ifndef __REX_PLATFORM_H__
#define __REX_PLATFORM_H__

//
//
// Rex Engine: Platform
//
//

namespace Rex
{

//
// Bootstrap
//

// Initialize platform-specific handlers
bool Platform_Init();

// Shutdown platform-specific handlers
bool Platform_Quit();

//
// Devices
//

// Read data from all currently connected input devices (keyboards, mice, etc)
bool Platform_ReadInputDevices();

// Get mouse data
bool Platform_MouseGet(rex_int *mx, rex_int *my, rex_int *mb);

// Set mouse data
bool Platform_MouseSet(rex_int mx, rex_int my, rex_int mb);

// Returns true if the specified key is pressed
bool Platform_KeyboardGet(rex_int scancode);

//
// Timer
//

// Returns the number of ticks elapsed since program start
rex_int64 Platform_GetTicks64();

//
// Graphics
//

// Show a simple message box
bool Platform_MessageBox(const char *title, const char *message);

} // namespace Rex

#endif // __REX_PLATFORM_H__
