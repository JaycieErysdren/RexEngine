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

int main(int argc, char *argv[])
{
	if (Rex::Initialize() == false)
	{
		cout << "Rex Engine initialization failed" << endl;
		return EXIT_FAILURE;
	}

	cout << "awaiting user input..." << endl;
	cin.get();

	if (Rex::Shutdown() == false)
	{
		cout << "Rex Engine shutdown failed" << endl;
		return EXIT_FAILURE;
	}

	// Exit gracefully
	return EXIT_SUCCESS;
}
