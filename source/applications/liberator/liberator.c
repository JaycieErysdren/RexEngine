// ========================================================
//
// FILE:			/source/applications/liberator/liberator.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Liberator program entry point.
//
// LAST EDITED:		November 8th, 2022
//
// ========================================================

// Include liberator header
#include "liberator.h"

rex_window *window;

// Print help text
void Liberator_PrintHelpText(void)
{
	printf("Liberator v0.0.1\n");
	printf("By Erysdren Media\n");
	printf("===============================");
	printf("\n");
	printf("Usage:\n");
	printf("Liberator -op0 pak0.pak\n");
	printf("\n");
	printf("Arguments:\n");
	printf("-opX - Select file operation\n");
	printf("\n");
	printf("File operations:\n");
	printf("0 - List file contents\n");
	printf("1 - Extract file contents\n");
	printf("2 - Convert file contents\n");
	printf("3 - View file contents\n");

	Rex_Shutdown(EXIT_SUCCESS);
}

// Main function
int main(int argc, char *argv[])
{
	//
	// Variables
	//

	// Numbers
	rex_int width, height;

	// Nuklear
	struct nk_color nk_clear = {0, 0, 0, 0};
	struct nk_rect nk_bounds = {0, 0, 0, 0};
	rex_nuklear_context *nk_context;

	//
	// Startup
	//

	// Startup Rex Engine
	Rex_Startup();
	Rex_Log(REX_LOG_FILENAME, "Starting Rex Engine");

	//=======================================
	// PAK testing

	pak_t *pak;
	rex_buffer *pak_file;
	rex_int len_pak_file;

	pak = PAK_Open("pak0.pak");

	if (pak == NULL)
		Rex_Message(REX_MESSAGE_FAILURE, "PAK_Open() load failed with error: %s", Rex_GetError());
	else
		Rex_Log(REX_LOG_FILENAME, "Loaded PAK file %s", pak->filename);

	len_pak_file = PAK_GetFileByFilename(pak, "maps/e1m1.bsp", &pak_file);

	if (pak_file == NULL || len_pak_file == 0)
		Rex_Message(REX_MESSAGE_FAILURE, "PAK_GetFileByFilename() load failed with error: %s", Rex_GetError());
	else
		Rex_Log(REX_LOG_FILENAME, "File found, size is %d bytes", len_pak_file);

	if (pak_file)
		free(pak_file);

	Rex_Log(REX_LOG_FILENAME, "Closing Rex Engine");

	PAK_Close(pak);
	Rex_Shutdown(EXIT_SUCCESS);

	//
	//=======================================

	width = (rex_int)(rex_desktop_size[0] / 1.35);
	height = (rex_int)(rex_desktop_size[1] / 1.35);

	// Add main window
	window = Rex_Window_Add(
		"Liberator",
		REX_WINDOW_CENTERED,
		REX_WINDOW_CENTERED,
		width, height,
		REX_WINDOW_DEFAULT_FLAGS
	);

	// Set BRender file-finding hooks
	BrModelFindHook(BrModelFindFailedLoad);
	BrMapFindHook(BrMapFindFailedLoad);
	BrMaterialFindHook(BrMaterialFindFailedLoad);

	// Allocate nuklear stuff
	nk_context = Rex_Nuklear_Init(window->buffer_color, "ModernDOS8x16.ttf", (rex_float)(window->buffer_color->height / 48));
	nk_bounds.w = window->buffer_color->width;
	nk_bounds.h = window->buffer_color->height;

	//
	// Main loop
	//

	while (rex_running == REX_TRUE)
	{
		//
		// Device I/O
		//

		// Read device states
		Rex_IO_ReadDevices();

		// Quit
		if (KEY_PRESSED(KEY_Q))
			rex_running = REX_FALSE;

		// Nuklear inputs
		nk_input_begin(&(nk_context->ctx));
		nk_input_motion(&(nk_context->ctx), rex_mouse.x, rex_mouse.y);

		if (MOUSE_PRESSED(MOUSE_LEFT)) nk_input_button(&(nk_context->ctx), NK_BUTTON_LEFT, rex_mouse.x, rex_mouse.y, 1);
		else nk_input_button(&(nk_context->ctx), NK_BUTTON_LEFT, rex_mouse.x, rex_mouse.y, 0);

		if (MOUSE_PRESSED(MOUSE_RIGHT)) nk_input_button(&(nk_context->ctx), NK_BUTTON_RIGHT, rex_mouse.x, rex_mouse.y, 1);
		else nk_input_button(&(nk_context->ctx), NK_BUTTON_RIGHT, rex_mouse.x, rex_mouse.y, 0);

		nk_input_end(&(nk_context->ctx));

		//
		// Updates
		//

		// Update window values
		if (Rex_Window_Update(window))
		{
			nk_bounds.w = window->buffer_color->width;
			nk_bounds.h = window->buffer_color->height;
		}

		//
		// Nuklear
		//

		if (nk_begin(&(nk_context->ctx), "Test", nk_bounds, 0))
		{
			nk_layout_row_dynamic(&(nk_context->ctx), 32, 1);

			if (nk_button_label(&(nk_context->ctx), "Open File"))
			{
				printf("button pressed\n");
			}
		}

		nk_end(&(nk_context->ctx));

		//
		// Rendering
		//

		// Nuklear
		Rex_Nuklear_Render(nk_context, nk_clear, REX_FALSE);

		// Flip buffer
		Rex_Window_DoubleBuffer(window);
	}

	//
	// Shutdown
	//

	// Shutdown Nuklear module
	Rex_Nuklear_Shutdown(nk_context);

	// Free the main window
	Rex_Window_Remove(window);

	// Shutdown Rex Engine
	Rex_Shutdown(EXIT_SUCCESS);

	// Tell the OS that the program shut down successfully
	return EXIT_SUCCESS;
}
