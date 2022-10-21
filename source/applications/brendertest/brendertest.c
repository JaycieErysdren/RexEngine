// ========================================================
//
// FILE:			/source/applications/brendertest/brendertest.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		BRenderTest program entry point.
//
// LAST EDITED:		October 21st, 2022
//
// ========================================================

// Include BRenderTest header
#include "brendertest.h"

// Main window
rex_window_external *rex_window;

// Create basic BRender scene
void BrenderTest_CreateScene(br_actor **_world, br_actor **_camera, br_actor **_cube)
{
	br_actor *world, *camera, *cube;
	br_order_table *order_table;

	BrModelFindHook(BrModelFindFailedLoad);
	BrMapFindHook(BrMapFindFailedLoad);
	BrMaterialFindHook(BrMaterialFindFailedLoad);

	world = BrActorAllocate(BR_ACTOR_NONE, NULL);

	order_table = BrZsOrderTableAllocate(1024, BR_ORDER_TABLE_LEAVE_BOUNDS | BR_ORDER_TABLE_BUCKET_SORT, BR_SORT_AVERAGE);
	BrZsActorOrderTableSet(world, order_table);

	camera = BrActorAdd(world, BrActorAllocate(BR_ACTOR_CAMERA, NULL));
	((br_camera *)camera->type_data)->type = BR_CAMERA_PERSPECTIVE_FOV;
	((br_camera *)camera->type_data)->field_of_view = BR_ANGLE_DEG(90);
	((br_camera *)camera->type_data)->hither_z = BR_SCALAR(0.1);
	((br_camera *)camera->type_data)->yon_z = BR_SCALAR(4096);
	((br_camera *)camera->type_data)->aspect = BR_SCALAR(rex_window->buffer_screen->width) / BR_SCALAR(rex_window->buffer_screen->height);

	order_table->min_z = ((br_camera *)camera->type_data)->hither_z;
	order_table->max_z = ((br_camera *)camera->type_data)->yon_z;

	BrMatrix34Translate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(0), BR_SCALAR(6));

	cube = BrActorAdd(world, BrActorAllocate(BR_ACTOR_MODEL, NULL));
	cube->model = global_model_test;
	cube->material = BrMaterialFind("checkerboard.mat");

	*_world = world;
	*_camera = camera;
	*_cube = cube;
}

// Main function
void main(int argc, char *argv[])
{
	// Variables for counting time
	rex_ulong frame_start_ticks, frame_end_ticks;
	rex_float frame_elapsed_ticks, frame_elapsed_seconds;

	// Error checker
	rex_int error;

	// Whether the program should be running or not
	rex_bool running = REX_TRUE;

	// Some BRender actors
	br_actor *world, *camera, *cube;

	// Text pixelmap
	br_pixelmap *buffer_overlay;

	// Startup Rex Engine
	Rex_Startup();

	error = Rex_Formats_idTech_MDL(REX_FORMATOP_VIEW, "player.mdl");
	if (error) Rex_Failure("Loading player.mdl failed. Error: %s", Rex_GetError(error));

	// Add main window
	rex_window = Rex_WindowExternal_Add(
		"BRenderTest",
		REX_WINDOW_EXTERNAL_CENTERED,
		REX_WINDOW_EXTERNAL_CENTERED,
		rex_desktop_size[0] / 2, rex_desktop_size[1] / 2,
		REX_WINDOW_EXTERNAL_DEFAULT_FLAGS
	);

	// Create basic BRender scene
	BrenderTest_CreateScene(&world, &camera, &cube);

	buffer_overlay = BrPixelmapAllocateSub(rex_window->buffer_color, 0, 0, rex_window->buffer_color->width, rex_window->buffer_color->height);

	// Fix z-up for Quake models
	BrMatrix34PostRotateZ(&cube->t.t.mat, BR_ANGLE_DEG(90));

	// Main loop
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
			BrMatrix34PostTranslate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(0), BR_SCALAR(-1));

		// Move backwards
		if (KEY_PRESSED(KEY_S))
			BrMatrix34PostTranslate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(0), BR_SCALAR(1));

		// Move leftwards
		if (KEY_PRESSED(KEY_A))
			BrMatrix34PostTranslate(&camera->t.t.mat, BR_SCALAR(-1), BR_SCALAR(0), BR_SCALAR(0));

		// Move rightwards
		if (KEY_PRESSED(KEY_D))
			BrMatrix34PostTranslate(&camera->t.t.mat, BR_SCALAR(1), BR_SCALAR(0), BR_SCALAR(0));

		// Move upwards
		if (KEY_PRESSED(KEY_SPACE))
			BrMatrix34PostTranslate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(1), BR_SCALAR(0));

		// Move downwards
		if (KEY_PRESSED(KEY_LCTRL))
			BrMatrix34PostTranslate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(-1), BR_SCALAR(0));

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
		BrMatrix34PostRotateY(&cube->t.t.mat, BR_ANGLE_DEG(BR_SCALAR(50) * BR_SCALAR(frame_elapsed_ticks)));

		//
		// Rendering
		//

		// Render a frame
		Rex_ExternalWindow_RenderZb(rex_window, world, camera, REX_RGB_GRY, REX_DEPTH_BUFFER_CLEAR);

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

	// Free the main window
	Rex_WindowExternal_Remove(rex_window);

	// Shutdown Rex Engine
	Rex_Shutdown();
}
