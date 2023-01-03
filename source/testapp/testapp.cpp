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
// Main entry point
//

int RexMain(int argc, char *argv[])
{
	if (Rex::Init() == false)
	{
		Rex::MessageBox("Error!", "Rex Engine initialization failed!");
		return EXIT_FAILURE;
	}

	Rex::MessageBox("Message:", "Looks like Rex initialized successfully!");

	if (Rex::Quit() == false)
	{
		Rex::MessageBox("Error!", "Rex Engine shutdown failed!");
		return EXIT_FAILURE;
	}

	// Exit gracefully
	return EXIT_SUCCESS;
}
