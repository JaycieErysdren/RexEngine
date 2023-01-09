// ========================================================
//
// FILE:			/source/rex/platform/win32.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine: Win32 Platform implementation
//
// LAST EDITED:		December 31st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex Engine: Win32 Platform
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

// Initialize a platform-specific graphics context
void *Platform_Init_Graphics(rex_int width, rex_int height, rex_int bpp, const char *title)
{
	return nullptr;
}

// Shutdown a platform-specific graphics context
void Platform_Quit_Graphics(void *context)
{
	return;
}

//
// Message Handling
//

// Handle messages to the user
bool Platform_MessageHandler(const char *title, const char *message, message_type type, time_t time)
{
	// Type string
	rex_string type_str;

	switch (type)
	{
		case MESSAGE: type_str = "Message"; break;
		case WARNING: type_str = "Warning"; break;
		case FAILURE: type_str=  "Failure"; break;
		default: return false;
	}

	// Concat the strings
	rex_string message_str;
	message_str = title;
	message_str += "\n\n";
	message_str += message;

	// Show Win32 message box
	MessageBox(NULL, type_str.buf, message_str.buf, MB_OK);

	// If it's a critical failure, quit the engine and exit the program
	if (type == FAILURE)
	{
		Quit();
		exit(EXIT_FAILURE);
	}

	return true;
}

} // namespace Rex

//
//
// MAIN PLATFORM ENTRY POINT
//
//

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	return RexMain(0, lpCmdLine);
}
