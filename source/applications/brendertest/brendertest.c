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
	((br_camera *)camera->type_data)->field_of_view = BR_ANGLE_DEG(75);
	((br_camera *)camera->type_data)->hither_z = BR_SCALAR(0.1);
	((br_camera *)camera->type_data)->aspect = BR_SCALAR(rex_window->width) / BR_SCALAR(rex_window->height);

	order_table->min_z = ((br_camera *)camera->type_data)->hither_z;
	order_table->max_z = ((br_camera *)camera->type_data)->yon_z;

	BrMatrix34Translate(&camera->t.t.mat, BR_SCALAR(0), BR_SCALAR(0), BR_SCALAR(2));

	cube = BrActorAdd(world, BrActorAllocate(BR_ACTOR_MODEL, NULL));
	cube->model = BrModelFind("cube.dat");
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

	// Whether the program should be running or not
	rex_bool running = REX_TRUE;

	// Some BRender actors
	br_actor *world, *camera, *cube;

	// Startup Rex Engine
	Rex_Startup();

	// Add main window
	rex_window = Rex_WindowExternal_Add(
		"BRenderTest",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		rex_desktop_size[0] / 2, rex_desktop_size[1] / 2,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	// Create basic BRender scene
	BrenderTest_CreateScene(&world, &camera, &cube);

	// Main loop
	while (running)
	{
		// Get start-of-frame time
		frame_start_ticks = SDL_GetTicks64();

		// Update window values
		if (Rex_WindowExternal_Update(rex_window))
		{
			((br_camera *)camera->type_data)->aspect = BR_SCALAR(rex_window->width) / BR_SCALAR(rex_window->height);
		}

		// Read device states
		Rex_IO_ReadDevices();

		// Handle keyboard input
		if (KEY_DOWN(KEY_Q))
			running = REX_FALSE;

		// Rotate cube based on time elapsed
		BrMatrix34PostRotateY(&cube->t.t.mat, BR_ANGLE_DEG(BR_SCALAR(50) * BR_SCALAR(frame_elapsed_ticks)));

		// Render a frame
		Rex_ExternalWindow_RenderZb(rex_window, world, camera, REX_RGB_GRY, REX_DEPTH_BUFFER_CLEAR);

		// Flip buffer
		Rex_ExternalWindow_DoubleBuffer(rex_window);

		// Get end-of-frame time
		frame_end_ticks = SDL_GetTicks64();

		// Calculate elapsed frame time in ticks and seconds
		frame_elapsed_ticks = (frame_end_ticks - frame_start_ticks) / 1000.0f;
		frame_elapsed_seconds = 1.0f / frame_elapsed_ticks;
	}

	// Destroy Rex Engine windows
	Rex_WindowExternal_Remove(rex_window);

	// Shutdown Rex Engine
	Rex_Shutdown();
}
