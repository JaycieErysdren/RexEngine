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
// LAST EDITED:		October 24th, 2022
//
// ========================================================

// Include liberator header
#include "liberator.h"

rex_window_external *rex_window;

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

	Rex_Shutdown();
}

// Main function
void main(int argc, char *argv[])
{
	//
	// Variables
	//

	rex_ulong frame_start_ticks, frame_end_ticks;
	rex_float frame_elapsed_ticks, frame_elapsed_seconds;
	rex_int i;
	rex_bool running = REX_TRUE;
	rex_int operation;
	rex_byte filename[512];
	br_actor *world, *camera, *model;
	br_order_table *order_table;

	//
	// Arguments
	//

	// If no arguments, print help text
	//if (argc == 1) Liberator_PrintHelpText();

	// Process commandline arguments
	while (*++argv)
	{
		if (!strcmp(*argv, "-?"))
		{
			Liberator_PrintHelpText();
		}
		else if (sscanf(*argv, "-op%d", &operation) == 1)
		{
			Rex_Message("Input operation: %d - %s", operation, Rex_Formats_OptHelper(operation));
		}
		else if (**argv == '-')
		{
			Rex_Warning("Invalid switch %s", *argv);
		}
		else
		{
			sprintf(filename, *argv);
			Rex_Message("Input filename: %s", filename);
		}
	}

	//
	// Startup
	//

	// Startup Rex Engine
	Rex_Startup();

	// Add main window
	rex_window = Rex_WindowExternal_Add(
		"Liberator",
		REX_WINDOW_EXTERNAL_CENTERED,
		REX_WINDOW_EXTERNAL_CENTERED,
		rex_desktop_size[0] / 2, rex_desktop_size[1] / 2,
		REX_WINDOW_EXTERNAL_DEFAULT_FLAGS
	);

	// Set BRender file-finding hooks
	BrModelFindHook(BrModelFindFailedLoad);
	BrMapFindHook(BrMapFindFailedLoad);
	BrMaterialFindHook(BrMaterialFindFailedLoad);

	// Allocate world
	world = BrActorAllocate(BR_ACTOR_NONE, NULL);

	// Allocate order table
	order_table = BrZsOrderTableAllocate(1024, BR_ORDER_TABLE_LEAVE_BOUNDS | BR_ORDER_TABLE_BUCKET_SORT, BR_SORT_AVERAGE);
	BrZsActorOrderTableSet(world, order_table);

	// Allocate camera actor
	camera = BrActorAdd(world, BrActorAllocate(BR_ACTOR_CAMERA, NULL));

	// Set camera values
	((br_camera *)camera->type_data)->type = BR_CAMERA_PERSPECTIVE_FOV;
	((br_camera *)camera->type_data)->field_of_view = BR_ANGLE_DEG(90);
	((br_camera *)camera->type_data)->hither_z = BR_SCALAR(0.1);
	((br_camera *)camera->type_data)->yon_z = BR_SCALAR(4096);
	((br_camera *)camera->type_data)->aspect = BR_SCALAR(rex_window->buffer_screen->width) / BR_SCALAR(rex_window->buffer_screen->height);
	BrMatrix34Translate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(0), BR_SCALAR(4));

	// Set order table values
	order_table->min_z = ((br_camera *)camera->type_data)->hither_z;
	order_table->max_z = ((br_camera *)camera->type_data)->yon_z;

	// Allocate model actor
	model = BrActorAdd(world, BrActorAllocate(BR_ACTOR_MODEL, NULL));
	model->model = BrModelFind("cube.dat");
	model->material = BrMaterialFind("checkerboard.mat");

	//
	// Main loop
	//

	while (running)
	{
		//
		// Time counting
		//

		// Get start-of-frame time
		frame_start_ticks = SDL_GetTicks64();

		//
		// Updates
		//

		// Update window values
		if (Rex_WindowExternal_Update(rex_window))
		{
			((br_camera *)camera->type_data)->aspect = BR_SCALAR(rex_window->buffer_screen->width) / BR_SCALAR(rex_window->buffer_screen->height);
		}

		//
		// Device I/O
		//

		// Read device states
		Rex_IO_ReadDevices();

		// Quit
		if (KEY_PRESSED(KEY_Q))
			running = REX_FALSE;

		// Move forwards
		if (KEY_PRESSED(KEY_W))
			BrMatrix34PreTranslate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(0), BR_SCALAR(-1));

		// Move backwards
		if (KEY_PRESSED(KEY_S))
			BrMatrix34PreTranslate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(0), BR_SCALAR(1));

		// Move leftwards
		if (KEY_PRESSED(KEY_A))
			BrMatrix34PreTranslate(&camera->t.t.mat, BR_SCALAR(-1), BR_SCALAR(0), BR_SCALAR(0));

		// Move rightwards
		if (KEY_PRESSED(KEY_D))
			BrMatrix34PreTranslate(&camera->t.t.mat, BR_SCALAR(1), BR_SCALAR(0), BR_SCALAR(0));

		// Move upwards
		if (KEY_PRESSED(KEY_SPACE))
			BrMatrix34PreTranslate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(1), BR_SCALAR(0));

		// Move downwards
		if (KEY_PRESSED(KEY_LCTRL))
			BrMatrix34PreTranslate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(-1), BR_SCALAR(0));

		// Look leftwards
		if (KEY_PRESSED(KEY_LEFT))
			BrMatrix34PreRotateY(&camera->t.t.mat, BR_ANGLE_DEG(1));

		// Look rightwards
		if (KEY_PRESSED(KEY_RIGHT))
			BrMatrix34PreRotateY(&camera->t.t.mat, BR_ANGLE_DEG(-1));

		//
		// Program logic
		//

		// Rotate cube based on time elapsed
		BrMatrix34PostRotateY(&model->t.t.mat, BR_ANGLE_DEG(BR_SCALAR(50) * BR_SCALAR(frame_elapsed_ticks)));

		//
		// Rendering
		//

		// Render a frame
		Rex_ExternalWindow_RenderZb(rex_window, world, camera, REX_RGB_GRY, REX_DEPTH_BUFFER_CLEAR);

		// FPS counter
		BrPixelmapTextF(rex_window->buffer_color, -(rex_window->buffer_color->width / 2) + 16, -(rex_window->buffer_color->height / 2) + 16, BR_COLOUR_RGB(255, 0, 0), BrFontFixed3x5, "FPS: %.2f", frame_elapsed_seconds);

		// Flip buffer
		Rex_ExternalWindow_DoubleBuffer(rex_window);

		//
		// Time counting
		//

		// Get end-of-frame time
		frame_end_ticks = SDL_GetTicks64();

		// Calculate elapsed frame time in ticks and seconds
		frame_elapsed_ticks = (frame_end_ticks - frame_start_ticks) / 1000.0f;
		frame_elapsed_seconds = 1.0f / frame_elapsed_ticks;
	}

	//
	// Shutdown
	//

	// Free the main window
	Rex_WindowExternal_Remove(rex_window);

	// Shutdown Rex Engine
	Rex_Shutdown();
}
