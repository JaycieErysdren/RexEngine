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

// Initialize a platform-specific graphics context
void *Platform_Init_Graphics(rex_int width, rex_int height, rex_int bpp, const char *title)
{
	return NULL;
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
	rex_string type_str;

	switch (type)
	{
		case MESSAGE: type_str = "Message: "; break;
		case WARNING: type_str = "Warning: "; break;
		case FAILURE: type_str=  "Failure: "; break;
		default: return false;
	}

	cout << "\n";
	cout << "+--------------------------------------+" << "\n";
	cout << "| " << type_str.buf << "\n";
	cout << "| " << "\n";
	cout << "| " << title << "\n";
	cout << "| " << "\n";
	cout << "| " << message << "\n";
	cout << "| " << "\n";
	cout << "| " << "Press enter to continue." << "\n";
	cout << "+--------------------------------------+" << "\n";
	cout << endl;
	while (getchar() != '\n');

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

int main(int argc, char **argv)
{
	return RexMain(argc, argv);
}
