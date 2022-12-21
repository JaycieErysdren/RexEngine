// ========================================================
//
// FILE:			/source/applications/paktui/paktui.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		PAKTUI program entry point.
//
// LAST EDITED:		November 12th, 2022
//
// ========================================================

// Include PAKTUI header
#include "paktui.h"

// Add border box
void AddBorderBox(WINDOW *window, rex_int x, rex_int y, rex_int w, rex_int h)
{
	mvwhline(window, y, x, 0, w);
	mvwhline(window, y + h, x, 0, w);
	mvwvline(window, y, x, 0, h);
	mvwvline(window, y, x + w, 0, h);
	mvwaddch(window, y, x, ACS_ULCORNER);
	mvwaddch(window, y + h, x, ACS_LLCORNER);
	mvwaddch(window, y, x + w, ACS_URCORNER);
	mvwaddch(window, y + h, x + w, ACS_LRCORNER);
}

// Draw function
void UpdateDraw(WINDOW *window)
{
	// Variables
	rex_int cols, rows;

	// Get width and height of window
	getmaxyx(window, rows, cols);

	// Clear screen
	wclear(window);

	// Add left box
	AddBorderBox(window, 0, 0, (cols / 2) - 1, (rows - 1));
	mvwprintw(window, 1, 1, "Hello World !!!");

	// Add right box
	AddBorderBox(window, (cols / 2), 0, (cols / 2) - 1, (rows - 1));
	mvwprintw(window, 1, (cols / 2) + 1, "Hotkeys:");
	mvwprintw(window, 2, (cols / 2) + 1, "R : Reload");
	mvwprintw(window, 3, (cols / 2) + 1, "Q : Quit");

	// Refresh
	wrefresh(window);
}

// Update everything
void Update(WINDOW *window)
{
	UpdateDraw(window);
}

// Main function
int main(int argc, char *argv[])
{
	// VariablesQ
	WINDOW *window;

	// Startup Rex Engine
	Rex_Startup(REX_DISPLAYMODE_TERMINAL);

	// Startup Curses
	window = initscr();
	noecho();
	curs_set(REX_FALSE);

	// Initial draw
	UpdateDraw(window);

	while (rex_running == REX_TRUE)
	{
		// Read inputs
		switch(wgetch(window))
		{
			// Update on resize
			case KEY_RESIZE:
				Update(window);
				break;

			// Update on r
			case 'r':
				Update(window);
				break;

			// Quit on q
			case 'q':
				rex_running = REX_FALSE;
				break;

			// Do nothing
			default:
				break;
		}
	}

	// Shutdown Curses
	endwin();

	// Shutdown Rex Engine
	Rex_Shutdown(EXIT_SUCCESS);

	// Tell the OS that the program shut down successfully
	return EXIT_SUCCESS;
}
