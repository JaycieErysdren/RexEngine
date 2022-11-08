// ========================================================
//
// FILE:			/source/applications/junkapp/junkapp.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		JunkApp program entry point.
//
// LAST EDITED:		November 8th, 2022
//
// ========================================================

// Include liberator header
#include "junkapp.h"

// Main function
int main(int argc, char *argv[])
{
	// Variables
	WINDOW *curses_window;

	// Startup Rex Engine
	Rex_Log(REX_LOG_FILENAME, "Initializing Rex Engine");
	Rex_Startup();

	// Startup Curses
	Rex_Log(REX_LOG_FILENAME, "Initializing Curses");
	curses_window = initscr();
	box(curses_window, 0, 0);
	wrefresh(curses_window);
	mvwprintw(curses_window, 25, 25, "Hello World !!!");
	wrefresh(curses_window);
	wgetch(curses_window);
	endwin();

	// Shutdown Rex Engine
	Rex_Log(REX_LOG_FILENAME, "Stopping Rex Engine");
	Rex_Shutdown(EXIT_SUCCESS);

	// Tell the OS that the program shut down successfully
	return EXIT_SUCCESS;
}
