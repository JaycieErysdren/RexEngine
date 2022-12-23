// ========================================================
//
// FILE:			/source/vivaduke/bootstrap.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Viva Duke Nukem
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Bootstrap functions for VDN
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

// vivaduke header
#include "vivaduke.hpp"

void VDN_EndScreen()
{
	// VGA text mode end screen
	gotoxy(0, ScreenRows() - 1);
	void *raw_screen_buffer = malloc(4000);
	FILE *raw_screen_file = fopen("gfx/end.bin", "rb");
	fread(raw_screen_buffer, sizeof(unsigned char), 4000, raw_screen_file);
	fclose(raw_screen_file);
	movedata(_my_ds(), (unsigned)raw_screen_buffer, _dos_ds, 0xB8000, 4000);
	free(raw_screen_buffer);
}
