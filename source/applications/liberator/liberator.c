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
// LAST EDITED:		October 28th, 2022
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

	Rex_Shutdown();
}

// Main function
int main(int argc, char *argv[])
{
	//
	// Variables
	//

	rex_int width, height;
	rex_ulong frame_start_ticks, frame_end_ticks;
	rex_float frame_elapsed_ticks, frame_elapsed_seconds;
	rex_int i;
	rex_bool running = REX_TRUE;
	rex_int operation;
	rex_byte filename[512];
	br_actor *world, *camera, *model;
	br_order_table *order_table;

	rex_byte *filter_patterns[2] = { "*.txt", "*.text" };
	rex_byte *openfile;

	struct nk_color nk_clear = {0, 0, 0, 0};
	struct nk_rect nk_bounds = {0, 0, 0, 0};
	rex_nuklear_context *nk_context;

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

	width = rex_desktop_size[0] / 1.25;
	height = rex_desktop_size[1] / 1.25;

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
	((br_camera *)camera->type_data)->aspect = BR_SCALAR(window->buffer_screen->width / 2) / BR_SCALAR(window->buffer_screen->height / 2);
	BrMatrix34Translate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(0), BR_SCALAR(4));

	// Set order table values
	order_table->min_z = ((br_camera *)camera->type_data)->hither_z;
	order_table->max_z = ((br_camera *)camera->type_data)->yon_z;

	// Allocate model actor
	model = BrActorAdd(world, BrActorAllocate(BR_ACTOR_MODEL, NULL));
	model->model = BrModelFind("cube.dat");
	model->material = BrMaterialFind("checkerboard.mat");

	// Allocate model view pixelmap
	br_pixelmap *model_view_color = BrPixelmapMatchTypedSized(window->buffer_color, BR_PMMATCH_OFFSCREEN, BR_PMT_RGB_888, window->buffer_color->width, window->buffer_color->height);
	br_pixelmap *model_view_depth = BrPixelmapMatch(model_view_color, BR_PMMATCH_DEPTH_16);
	model_view_color->origin_x = model_view_depth->origin_x = (br_int_16)(model_view_color->width / 2);
	model_view_color->origin_y = model_view_depth->origin_y = (br_int_16)(model_view_color->height / 2);

	// Allocate nuklear stuff
	nk_context = Rex_Nuklear_Init(window->buffer_color, "ModernDOS8x16.ttf", (rex_float)(window->buffer_color->height / 48));
	nk_bounds.w = window->buffer_color->width / 2;
	nk_bounds.h = window->buffer_color->height;

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

		// Nuklear inputs
		nk_input_begin(&(nk_context->ctx));
		nk_input_motion(&(nk_context->ctx), rex_mouse.x, rex_mouse.y);

		if (MOUSE_PRESSED(MOUSE_LEFT)) nk_input_button(&(nk_context->ctx), NK_BUTTON_LEFT, rex_mouse.x, rex_mouse.y, 1);
		else nk_input_button(&(nk_context->ctx), NK_BUTTON_LEFT, rex_mouse.x, rex_mouse.y, 0);

		if (MOUSE_PRESSED(MOUSE_RIGHT)) nk_input_button(&(nk_context->ctx), NK_BUTTON_RIGHT, rex_mouse.x, rex_mouse.y, 1);
		else nk_input_button(&(nk_context->ctx), NK_BUTTON_RIGHT, rex_mouse.x, rex_mouse.y, 0);

		nk_input_end(&(nk_context->ctx));

		//
		// Program logic
		//

		// Rotate cube based on time elapsed
		BrMatrix34PostRotateY(&model->t.t.mat, BR_ANGLE_DEG(BR_SCALAR(50) * BR_SCALAR(frame_elapsed_ticks)));

		//
		// Updates
		//

		// Update window values
		if (Rex_Window_Update(window))
		{
			((br_camera *)camera->type_data)->aspect = BR_SCALAR(model_view_color->width) / BR_SCALAR(model_view_color->height);

			BrPixelmapResize(model_view_color, window->buffer_color->width, window->buffer_color->height);
			BrPixelmapResize(model_view_depth, window->buffer_color->width, window->buffer_color->height);
			model_view_color->origin_x = model_view_depth->origin_x = (br_int_16)(model_view_color->width / 2);
			model_view_color->origin_y = model_view_depth->origin_y = (br_int_16)(model_view_color->height / 2);

			nk_bounds.w = window->buffer_color->width / 2;
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
				//openfile = Rex_IO_OpenFileDialog("Choose file", "", 2, filter_patterns, "text files", 0);
				printf("button pressed\n");
			}
		}

		nk_end(&(nk_context->ctx));

		//
		// Rendering
		//

		// Render a frame
		BrRendererFrameBegin();
		BrPixelmapFill(model_view_color, BR_COLOUR_RGB(64, 64, 64));
		BrPixelmapFill(model_view_depth, REX_DEPTH_BUFFER_CLEAR);
		BrZbSceneRender(world, camera, model_view_color, model_view_depth);
		BrRendererFrameEnd();

		// Nuklear
		Rex_Nuklear_Render(nk_context, nk_clear, REX_FALSE);

		BrPixelmapRectangleStretchCopy(window->buffer_color, 0, -(window->buffer_color->height / 2), window->buffer_color->width / 2, window->buffer_color->height / 2, model_view_color, -(model_view_color->width / 2), -(model_view_color->height / 2), model_view_color->width, model_view_color->height);

		// Flip buffer
		Rex_Window_DoubleBuffer(window);

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

	Rex_Nuklear_Shutdown(nk_context);

	// Free the main window
	Rex_Window_Remove(window);

	// Shutdown Rex Engine
	Rex_Shutdown();

	return EXIT_SUCCESS;
}
