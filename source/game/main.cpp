// ========================================================
//
// FILE:			/source/game/main.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Testbed program entry point
//
// LAST EDITED:		December 17th, 2022
//
// ========================================================

// Game header
#include "game.hpp"

#define CYCLES 30

//
// Globals
//

Rex::Actor3D *actor3d_root;
Rex::Actor3D *actor3d_camera;

Rex::Actor2D *actor2d_root;
Rex::Actor2D *actor2d_mouse;
Rex::Actor2D *actor2d_window;
Rex::Actor2D *actor2d_button_exit;
Rex::Actor2D *actor2d_button_return;

// Heightmap generator
void Heightmap_Generate(Voxel::VoxelModel *model)
{
	rex_int x, y;

	for (y = 0; y < model->dimensions.y; y++)
	{
		for (x = 0; x < model->dimensions.x; x++)
		{
			rex_int d1 = ((x & 31) - 16);
			rex_int d2 = ((y & 31) - 16);
			rex_int d = (15 * 15) - (d1 * d1) - (d2 * d2);
			rex_int sphere_hei;
			rex_uint8 sphere_col;

			rex_uint8 ground_hei;
			rex_uint8 ground_col;

			Voxel::VoxelSlab ground_slab;
			Voxel::VoxelSlab sphere_slab;

			ground_hei = 1;
			ground_col = (cos(x * 0.2f) + sin(y * 0.3f)) * 3 + 88;

			ground_slab.voxels_drawn = ground_hei;
			ground_slab.color_side = ground_col;
			ground_slab.color_top = ground_col;
			ground_slab.color_bottom = ground_col;

			if (d > 0 && ((x ^ y) & 32))
			{
				// we're drawing a sphere above the ground
				sphere_hei = REX_SQRT(d);
				sphere_col = (x + y) * 0.5f;

				sphere_slab.voxels_drawn = sphere_hei;
				sphere_slab.voxels_skipped = (256 - sphere_hei) - 32;
				sphere_slab.color_side = sphere_col;
				sphere_slab.color_top = sphere_col;
				sphere_slab.color_bottom = sphere_col;

				// top half of sphere
				model->AddSlabToColumn(x, y, sphere_slab);

				// bottom half of sphere
				sphere_slab.voxels_skipped = 0;
				model->AddSlabToColumn(x, y, sphere_slab);

				// ground
				ground_slab.voxels_skipped = 31 - sphere_hei;
				model->AddSlabToColumn(x, y, ground_slab);
			}
			else
			{
				// we're just drawing the ground
				ground_slab.voxels_skipped = 256 - ground_hei;
				model->AddSlabToColumn(x, y, ground_slab);
			}
		}
	}
}

#ifdef BLARGH

// Heightmap loader
void Heightmap_Load(string filename_color, string filename_height, rex_int size_x, rex_int size_y)
{
	// Variables
	rex_int x, y;

	FILE *hei = fopen(filename_height.c_str(), "rb");
	FILE *col = fopen(filename_color.c_str(), "rb");

	for (y = 0; y < size_y; y++)
	{
		for (x = 0; x < size_x; x++)
		{
			Voxel::Slab slab;

			rex_uint8 height = fgetc(hei);
			rex_uint8 color = fgetc(col);

			slab.drawn = height + 1;
			slab.skipped = 256 - height;

			slab.color_top = color;
			slab.color_bottom = color;
			slab.color_side = color;

			world->AddSlab(x, y, slab);
		}
	}

	fclose(hei);
	fclose(col);
}

// Tilemap loader
void Tilemap_Load(string filename)
{
	// variables
	cJSON *json, *json_array, *json_arrayitem, *json_child, *json_mapdata;
	FILE *file;
	rex_int file_len, map_w, map_h, mapdata, mapdata_len, x, y, x1, y1;
	char *buffer;

	// open file
	file = fopen(filename.c_str(), "rt");
	if (file == NULL) return;

	// seek to end
	fseek(file, 0L, SEEK_END);

	// get file length
	file_len = ftell(file);

	// seek to start
	fseek(file, 0L, SEEK_SET);

	// allocate char buffer
	buffer = (char *)calloc(1, file_len);
	if (buffer == NULL) return;

	// read file into buffer
	fread(buffer, sizeof(char), file_len, file);

	// close file
	fclose(file);

	// parse json string
	json = cJSON_ParseWithLength(buffer, file_len);
	if (json == NULL) return;

	// get layer array
	json_array = cJSON_GetObjectItem(json, "layers");
	if (json_array == NULL) return;

	// get first item
	json_arrayitem = cJSON_GetArrayItem(json_array, 0);
	if (json_arrayitem == NULL) return;

	// get width
	json_child = cJSON_GetObjectItem(json_arrayitem, "width");
	if (json_child == NULL) return;

	map_w = json_child->valueint;

	// get height
	json_child = cJSON_GetObjectItem(json_arrayitem, "height");
	if (json_child == NULL) return;

	map_h = json_child->valueint;

	// get map data
	json_mapdata = cJSON_GetObjectItem(json_arrayitem, "data");
	if (json_mapdata == NULL) return;

	// get length of map data array
	mapdata_len = cJSON_GetArraySize(json_mapdata);
	if (mapdata_len < 1) return;
	if (mapdata_len != map_w * map_h) return;

	// create slabs for map data array
	for (y = 0; y < map_h; y++)
	{
		for (x = 0; x < map_w; x++)
		{
			json_child = cJSON_GetArrayItem(json_mapdata, (y * map_h) + x);
			if (json_child == NULL) return;

			mapdata = json_child->valueint;

			Voxel::Slab slab;

			slab.color_side = mapdata;
			slab.color_top = mapdata;
			slab.color_bottom = mapdata;

			slab.drawn = CLAMP(mapdata, 0, 1);
			slab.skipped = 256 - CLAMP(mapdata, 0, 1);

			for (y1 = 0; y1 < 4; y1++)
			{
				for (x1 = 0; x1 < 4; x1++)
				{
					world->AddSlab((x * 4) + x1, (y * 4) + y1, slab);
				}
			}
		}
	}
}

#endif

// Initialize testbed
void Initialize()
{
	Rex::Log("gamed.log", "loading...");

	Rex::SetGraphicsPalette("gfx/duke3d.pal");
	Rex::ColormapLoad("gfx/duke3d.tab");

	// 2D Actors
	actor2d_root = Rex::AddActor2D(NULL, Rex::ACTOR2D_NONE);

	actor2d_window = Rex::AddActor2D(actor2d_root, Rex::ACTOR2D_WINDOW, "gfx/bg2.bmp");
	actor2d_button_exit = Rex::AddActor2D(actor2d_window, Rex::ACTOR2D_BUTTON, "gfx/btn_exit.bmp");
	actor2d_button_return = Rex::AddActor2D(actor2d_window, Rex::ACTOR2D_BUTTON, "gfx/btn_clck.bmp");

	actor2d_mouse = Rex::AddActor2D(actor2d_root, Rex::ACTOR2D_CURSOR, "gfx/cursor.bmp");
	actor2d_mouse->color_blit_mode = Rex::COLORKEY;

	// 3D actors
	actor3d_root = Rex::AddActor3D(NULL, Rex::ACTOR3D_MODEL);
	actor3d_camera = Rex::AddActor3D(actor3d_root, Rex::ACTOR3D_CAMERA);

	actor3d_root->model = Voxel::AddVoxelModel(64, 64, 256);
	actor3d_root->identifier = "ORBB FIELD";

	Heightmap_Generate((Voxel::VoxelModel *)actor3d_root->model);

	// Initialize camera info
	actor3d_camera->draw_distance = REX_SCALAR(128);

	actor3d_camera->origin.x = REX_SCALAR(0);
	actor3d_camera->origin.y = REX_SCALAR(0);
	actor3d_camera->origin.z = REX_SCALAR(4);

	actor3d_camera->angles.x = 0;
	actor3d_camera->angles.y = 0;
	actor3d_camera->angles.z = 0;
}

void Shutdown()
{
	// Tell log we're shutting down
	Rex::Log("gamed.log", "shutting down...");

	// Free world hierarchy
	Rex::Log("gamed.log", "freeing 3D world hierarchy");
	Rex::FreeActor3D(actor3d_root);

	// Free screen hierarchy
	Rex::Log("gamed.log", "freeing 2D world hierarchy");
	Rex::FreeActor2D(actor2d_root);
}

//
// Misc
//

// Camera controller
void CameraController(rex_int32 mb, rex_vec2i mp)
{
	// Mouse read
	static rex_int32 mx_prev, my_prev;
	rex_int32 delta_mx, delta_my;

	delta_mx = mx_prev - mp.x;
	delta_my = my_prev - mp.y;

	// Mouse look
	if (mb == 1 && delta_mx != 0) actor3d_camera->angles.y += delta_mx;
	if (mb == 2 && delta_my != 0) actor3d_camera->angles.x -= delta_my;

	// Reset pitch
	if (mb == 3)
		actor3d_camera->angles.x = 0;

	actor3d_camera->anglespeedkey = 2;

	// Keyboard look
	{
		// Rotate leftwards
		if (Rex::KeyTest(REX_SC_LEFT)) actor3d_camera->angles.y += actor3d_camera->anglespeedkey;

		// Rotate rightwards
		if (Rex::KeyTest(REX_SC_RIGHT)) actor3d_camera->angles.y -= actor3d_camera->anglespeedkey;

		// Look upwards
		if (Rex::KeyTest(REX_SC_UP)) actor3d_camera->angles.x += actor3d_camera->anglespeedkey;

		// Look downwards
		if (Rex::KeyTest(REX_SC_DOWN)) actor3d_camera->angles.x -= actor3d_camera->anglespeedkey;
	}

	// Pitch angle sanity checks
	if (actor3d_camera->angles.x < -90) actor3d_camera->angles.x = -90;
	if (actor3d_camera->angles.x > 90) actor3d_camera->angles.x = 90;

	// Yaw angle sanity checks
	if (actor3d_camera->angles.y < 0) actor3d_camera->angles.y += 360;
	if (actor3d_camera->angles.y > 359) actor3d_camera->angles.y -= 360;

	// Check if sprinting
	if (Rex::KeyTest(REX_SC_LSHIFT))
		actor3d_camera->movespeedkey = 2;
	else
		actor3d_camera->movespeedkey = 1;

	// Set velocity
	actor3d_camera->velocity.x = Rex::math_table->sin[actor3d_camera->angles.y] * actor3d_camera->movespeedkey;
	actor3d_camera->velocity.y = Rex::math_table->cos[actor3d_camera->angles.y] * actor3d_camera->movespeedkey;
	actor3d_camera->velocity.z = REX_SCALAR(1.0f) * actor3d_camera->movespeedkey;

	// Move forwards
	if (Rex::KeyTest(REX_SC_W))
	{
		actor3d_camera->origin.x += actor3d_camera->velocity.x;
		actor3d_camera->origin.y += actor3d_camera->velocity.y;
	}

	// Move backwards
	if (Rex::KeyTest(REX_SC_S))
	{
		actor3d_camera->origin.x -= actor3d_camera->velocity.x;
		actor3d_camera->origin.y -= actor3d_camera->velocity.y;
	}

	// Move leftwards
	if (Rex::KeyTest(REX_SC_A))
	{
		actor3d_camera->origin.x += actor3d_camera->velocity.y;
		actor3d_camera->origin.y -= actor3d_camera->velocity.x;
	}

	// Move rightwards
	if (Rex::KeyTest(REX_SC_D))
	{
		actor3d_camera->origin.x -= actor3d_camera->velocity.y;
		actor3d_camera->origin.y += actor3d_camera->velocity.x;
	}

	// Move upwards
	if (Rex::KeyTest(REX_SC_Q))
		actor3d_camera->origin.z += actor3d_camera->velocity.z;

	// Move downwards
	if (Rex::KeyTest(REX_SC_E))
		actor3d_camera->origin.z -= actor3d_camera->velocity.z;

	mx_prev = mp.x;
	my_prev = mp.y;
}

// Mouse helper function
void ReadMouse(rex_int32 *buttons, rex_vec2i *pos, rex_int32 speedlimit, rex_rect area)
{
	rex_int32 mb, mx, my, dmx, dmy;
	rex_int32 halfx = 160, halfy = 100;

	Rex::MouseRead(&mb, &mx, &my);

	dmx = mx - halfx;
	dmy = my - halfy;

	pos->x += (dmx < 0) ? (dmx < -speedlimit ? -speedlimit : dmx) : (dmx > speedlimit ? speedlimit : dmx);
	pos->y += (dmy < 0) ? (dmy < -speedlimit ? -speedlimit : dmy) : (dmy > speedlimit ? speedlimit : dmy);

	// Clip mouse to area
	if (pos->x < area.x1) pos->x = area.x1;
	if (pos->x > area.x2) pos->x = area.x2;
	if (pos->y < area.y1) pos->y = area.y1;
	if (pos->y > area.y2) pos->y = area.y2;

	*buttons = mb;

	// 160x100 is the the middle of the 320x200 screen
	// (use this regardless of actual screen resolution)
	Rex::MouseSet(halfx, halfy);
}

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// General variables
	rex_bool running = true;
	rex_int game_state = 1;

	// Cycle variables
	rex_int64 frame_start, frame_end;
	rex_int32 cycles, c;

	// Picture buffers
	Rex::Surface pic_font;
	Rex::Surface pic_bbuffer;

	// Initialize Rex Engine
	Rex::Initialize();

	// Initialize Graphics
	if (Rex::InitializeGraphics(320, 200, 8) == false) return EXIT_FAILURE;
	Rex::VidInfo vidinfo = Rex::GetVidInfo();

	// Create picture buffers
	Rex::SurfaceLoadBMP(&pic_font, "gfx/font8x8.bmp");
	Rex::SurfaceCreate(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);

	Voxel::Initialize(vidinfo.width, vidinfo.height);
	Initialize();

	// Start counting time
	frame_end = Rex::GetTicks64();

	// Main loop
	while (running == true && !Rex::KeyTest(REX_SC_ESCAPE))
	{
		// Get start of frame time
		frame_start = Rex::GetTicks64();

		#if (REX_COMPILER == COMPILER_DJGPP)
			cycles = CYCLES * (frame_start - frame_end) / UCLOCKS_PER_SEC;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			cycles = CYCLES * (frame_start - frame_end) / CLOCKS_PER_SEC;
		#endif

		rex_int32 mouse_buttons;
		rex_rect mouse_area = {0, 0, pic_bbuffer.width - actor2d_mouse->color.width, pic_bbuffer.height - actor2d_mouse->color.height};
		ReadMouse(&mouse_buttons, &actor2d_mouse->origin, 16, mouse_area);

		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 0);

		if (game_state == 1)
		{
			//
			// User inputs
			//

			// Return to game
			if (mouse_buttons == 1) actor2d_mouse->events |= Rex::ACTOR2D_EVENT01;

			//
			// Return-to-game button
			//

			// Position logic
			actor2d_button_return->origin.x = 96;
			actor2d_button_return->origin.y = 32;
			actor2d_button_return->draw_area.x1 = 0;
			actor2d_button_return->draw_area.y1 = 0;
			actor2d_button_return->draw_area.x2 = 48;
			actor2d_button_return->draw_area.y2 = 24;

			// Mouse hover logic
			if (actor2d_mouse->OriginInside(actor2d_button_return))
			{
				actor2d_button_return->draw_area.x1 = 48;
				actor2d_button_return->draw_area.y1 = 0;
				actor2d_button_return->draw_area.x2 = 96;
				actor2d_button_return->draw_area.y2 = 24;

				if (actor2d_mouse->events && Rex::ACTOR2D_EVENT01)
				{
					actor2d_button_return->draw_area.x1 = 96;
					actor2d_button_return->draw_area.y1 = 0;
					actor2d_button_return->draw_area.x2 = 144;
					actor2d_button_return->draw_area.y2 = 24;

					game_state = 2;
				}
			}

			//
			// Exit button
			//

			// Position logic
			actor2d_button_exit->origin.x = 32;
			actor2d_button_exit->origin.y = 32;
			actor2d_button_exit->draw_area.x1 = 0;
			actor2d_button_exit->draw_area.y1 = 0;
			actor2d_button_exit->draw_area.x2 = 48;
			actor2d_button_exit->draw_area.y2 = 24;

			// Mouse hover logic
			if (actor2d_mouse->OriginInside(actor2d_button_exit))
			{
				actor2d_button_exit->draw_area.x1 = 48;
				actor2d_button_exit->draw_area.y1 = 0;
				actor2d_button_exit->draw_area.x2 = 96;
				actor2d_button_exit->draw_area.y2 = 24;

				if (actor2d_mouse->events && Rex::ACTOR2D_EVENT01)
				{
					actor2d_button_exit->draw_area.x1 = 96;
					actor2d_button_exit->draw_area.y1 = 0;
					actor2d_button_exit->draw_area.x2 = 144;
					actor2d_button_exit->draw_area.y2 = 24;

					running = false;
				}
			}

			//
			// Mouse
			//

			// Clear mouse events
			actor2d_mouse->ClearEvents();

			//
			// Draw 2D actors
			//

			Rex::RenderActor2D(&pic_bbuffer, actor2d_root);
		}
		else if (game_state == 2)
		{
			//
			// User inputs
			//

			// Return to menu
			if (Rex::KeyTest(REX_SC_TILDE)) game_state = 1;

			// Cycles
			for (c = 0; c < cycles; c++)
			{
				// Camera controller
				CameraController(mouse_buttons, actor2d_mouse->origin);
			}

			//
			// Rendering
			//

			// Render 3D scene
			rex_rect screen_area = {0, 0, pic_bbuffer.width, pic_bbuffer.height};
			Voxel::Render(&pic_bbuffer, actor3d_root, actor3d_camera, REX_SCALAR(160));

			// Render onscreen text
			Rex::ConsoleTextF(&pic_bbuffer, &pic_font, 8, 0, 0, "x: %d y: %d z %d", RexScalarToInteger(actor3d_camera->origin.x), RexScalarToInteger(actor3d_camera->origin.y), RexScalarToInteger(actor3d_camera->origin.z));
			Rex::ConsoleTextF(&pic_bbuffer, &pic_font, 8, 0, 1, "pitch: %d yaw: %d roll %d", actor3d_camera->angles.x, actor3d_camera->angles.y, actor3d_camera->angles.z);
		}
		else
		{
			running = false;
		}

		// Flip the rendering buffers
		Rex::SurfaceToFrontBuffer(&pic_bbuffer);

		// Get end of frame time
		#if (REX_COMPILER == COMPILER_DJGPP)
			frame_end = frame_end + cycles * UCLOCKS_PER_SEC / CYCLES;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			frame_end = frame_end + cycles * CLOCKS_PER_SEC / CYCLES;
		#endif
	}

	// Shutdown game features, free up memory
	Shutdown();

	// Shutdown Graphics
	Rex::ShutdownGraphics();

	// Shutdown Rex Engine
	Rex::Shutdown();

	// Cleanup memory
	Rex::SurfaceDestroy(&pic_font);
	Rex::SurfaceDestroy(&pic_bbuffer);

	// Exit gracefully
	return EXIT_SUCCESS;
}
