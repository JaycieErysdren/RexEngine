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
// LAST EDITED:		December 31st, 2022
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
	return true;
}

// Free all interrupts
bool Platform_Quit()
{
	return true;
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

//
// Graphics
//

// Show a simple message box
bool Platform_MessageBox(const char *title, const char *message)
{
	cout << title << endl;
	cout << message << endl;

	return true;
}

} // namespace Rex

//
//
// MAIN PLATFORM ENTRY POINT
//
//

int main(int argc, char *argv[])
{
	return RexMain(argc, argv);
}
