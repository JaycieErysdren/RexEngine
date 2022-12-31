// ========================================================
//
// FILE:			/source/testapp/testapp.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		TestApp entry point
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

// Header
#include "testapp.hpp"

//
// Message
//

void Message(const char *title, const char *message)
{
	#if (REX_PLATFORM == PLATFORM_WIN32)

	MessageBox(NULL, title, message, MB_OK | MB_TASKMODAL);

	#else

	cout << title << endl;
	cout << message << endl;

	#endif
}

//
// Main entry point
//

int RexMain(int argc, char *argv[])
{
	if (Rex::Init() == false)
	{
		Message("Error", "Rex Engine initialization failed");
		return EXIT_FAILURE;
	}

	Message("Message", "Awaiting user input...");

	if (Rex::Quit() == false)
	{
		Message("Error", "Rex Engine shutdown failed");
		return EXIT_FAILURE;
	}

	// Exit gracefully
	return EXIT_SUCCESS;
}
