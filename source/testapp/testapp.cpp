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
		Rex::MessageBox("Error!", "Rex Engine initialization failed.");
		return EXIT_FAILURE;
	}

	Rex::rex_string str1 = "hello";
	Rex::rex_string str2 = "bobby";

	cout << str1.buf << endl;
	cout << str2.buf << endl;

	str1 += str2;

	cout << str1.buf << endl;
	cout << str2.buf << endl;

	// console colors test
	#ifdef NOPE

	//gotoxy(0, ScreenRows() - 1);
	void *raw_screen_buffer = malloc(4000);
	FILE *raw_screen_file = fopen("assets/gfx/end.bin", "rb");
	fread(raw_screen_buffer, sizeof(unsigned char), 4000, raw_screen_file);
	fclose(raw_screen_file);
	memcpy((void *)0xB8000, raw_screen_buffer, 4000);
	free(raw_screen_buffer);

	#endif

	Rex::MessageBox("Message:", "Looks like Rex initialized successfully!");

	if (Rex::Quit() == false)
	{
		Rex::MessageBox("Error!", "Rex Engine shutdown failed.");
		return EXIT_FAILURE;
	}

	// Exit gracefully
	return EXIT_SUCCESS;
}
