// ========================================================
//
// FILE:			/source/testbed/main.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Testbed program entry point
//
// LAST EDITED:		December 15th, 2022
//
// ========================================================

// Testbed header
#include "testbed.hpp"

#define CYCLES 30

//
// Globals
//

// Voxel world
Voxel::World *world;

// Camera
Rex::Camera camera;

// Math tables
Rex::MathTable *mathtable;

// Console buffer
char console_buffer[256];

// Heightmap generator
void Heightmap_Generate()
{
	rex_int x, y;

	for (y = 0; y < 128; y++)
	{
		for (x = 0; x < 128; x++)
		{
			rex_int d1 = ((x & 31) - 16);
			rex_int d2 = ((y & 31) - 16);
			rex_int d = (15 * 15) - (d1 * d1) - (d2 * d2);
			rex_int sphere_hei;
			rex_uint8 sphere_col;

			rex_int ground_hei;
			rex_color ground_col;

			Voxel::Slab ground_slab;
			Voxel::Slab sphere_slab;

			ground_hei = 1;
			ground_col = (cos(x * 0.2f) + sin(y * 0.3f)) * 3 + 88;

			ground_slab.drawn = ground_hei;
			ground_slab.color_side = ground_col;
			ground_slab.color_top = ground_col;
			ground_slab.color_bottom = ground_col;

			if (d > 0 && ((x ^ y) & 32))
			{
				// we're drawing a sphere above the ground
				sphere_hei = REX_SQRT(d);
				sphere_col = (x + y) * 0.5f;

				sphere_slab.drawn = sphere_hei;
				sphere_slab.skipped = (256 - sphere_hei) - 32;
				sphere_slab.color_side = sphere_col;
				sphere_slab.color_top = sphere_col;
				sphere_slab.color_bottom = sphere_col;

				// top half of sphere
				world->AddSlab(x, y, sphere_slab);

				// bottom half of sphere
				sphere_slab.skipped = 0;
				world->AddSlab(x, y, sphere_slab);

				// ground
				ground_slab.skipped = 31 - sphere_hei;
				world->AddSlab(x, y, ground_slab);
			}
			else
			{
				// we're just drawing the ground
				ground_slab.skipped = 256 - ground_hei;
				world->AddSlab(x, y, ground_slab);
			}
		}
	}
}

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

			slab.color_top = color - 2;
			slab.color_bottom = color - 2;
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
	rex_int file_len, map_w, map_h, mapdata, mapdata_len, x, y;
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

			world->AddSlab(x, y, slab);
		}
	}
}

// Initialize testbed
void Initialize()
{
	printf("loading...\n");

	Rex::SetGraphicsPalette("gfx/mindgrdn.pal");
	Rex::ColormapLoad("gfx/mindgrdn.tab");

	// allocate world
	world = new Voxel::World("Map", 1024, 1024, 256);

	// add an actor
	world->AddActor(REX_SCALAR(16), REX_SCALAR(32), REX_SCALAR(4));

	Heightmap_Generate();
	//Heightmap_Load("voxel/m1c_mg.dat", "voxel/m1h.dat", 1024, 1024);
	//Tilemap_Load("maps/casino.tmj");

	// Initialize math table
	mathtable = new Rex::MathTable;

	// Initialize camera info
	camera.draw_distance = REX_SCALAR(128);

	camera.origin.x = REX_SCALAR(0);
	camera.origin.y = REX_SCALAR(0);
	camera.origin.z = REX_SCALAR(4);

	camera.angles.x = 0;
	camera.angles.y = 0;
	camera.angles.z = 0;
}

void Shutdown()
{
	if (world) delete world;
	if (mathtable) delete mathtable;
}

//
// Misc
//

// Camera controller
void CameraController()
{
	// Mouse read
	static rex_int32 mx_prev, my_prev;
	rex_int32 delta_mx, delta_my;
	rex_int32 mb, mx, my;
	Rex::MouseRead(&mb, &mx, &my);

	delta_mx = mx_prev - mx;
	delta_my = my_prev - my;

	// Mouse look
	if (mb == 1 && delta_mx != 0) camera.angles.y += delta_mx;
	if (mb == 2 && delta_my != 0) camera.angles.x -= delta_my;

	// Reset pitch
	if (mb == 3)
		camera.angles.x = 0;

	camera.anglespeedkey = 2;

	// Keyboard look
	{
		// Rotate leftwards
		if (Rex::KeyTest(REX_KB_LEFT)) camera.angles.y += camera.anglespeedkey;

		// Rotate rightwards
		if (Rex::KeyTest(REX_KB_RIGHT)) camera.angles.y -= camera.anglespeedkey;

		// Look upwards
		if (Rex::KeyTest(REX_KB_UP)) camera.angles.x += camera.anglespeedkey;

		// Look downwards
		if (Rex::KeyTest(REX_KB_DOWN)) camera.angles.x -= camera.anglespeedkey;
	}

	// Pitch angle sanity checks
	if (camera.angles.x < -90) camera.angles.x = -90;
	if (camera.angles.x > 90) camera.angles.x = 90;

	// Yaw angle sanity checks
	if (camera.angles.y < 0) camera.angles.y += 360;
	if (camera.angles.y > 359) camera.angles.y -= 360;

	// Check if sprinting
	if (Rex::KeyTest(REX_KB_LSHIFT))
		camera.movespeedkey = 2;
	else
		camera.movespeedkey = 1;

	// Set velocity
	camera.velocity.x = mathtable->sin[camera.angles.y] * camera.movespeedkey;
	camera.velocity.y = mathtable->cos[camera.angles.y] * camera.movespeedkey;
	camera.velocity.z = REX_SCALAR(1.0f) * camera.movespeedkey;

	// Move forwards
	if (Rex::KeyTest(REX_KB_W))
	{
		camera.origin.x += camera.velocity.x;
		camera.origin.y += camera.velocity.y;
	}

	// Move backwards
	if (Rex::KeyTest(REX_KB_S))
	{
		camera.origin.x -= camera.velocity.x;
		camera.origin.y -= camera.velocity.y;
	}

	// Move leftwards
	if (Rex::KeyTest(REX_KB_A))
	{
		camera.origin.x += camera.velocity.y;
		camera.origin.y -= camera.velocity.x;
	}

	// Move rightwards
	if (Rex::KeyTest(REX_KB_D))
	{
		camera.origin.x -= camera.velocity.y;
		camera.origin.y += camera.velocity.x;
	}

	// Move upwards
	if (Rex::KeyTest(REX_KB_Q))
		camera.origin.z += camera.velocity.z;

	// Move downwards
	if (Rex::KeyTest(REX_KB_E))
		camera.origin.z -= camera.velocity.z;

	mx_prev = mx;
	my_prev = my;
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
	// Cycle variables
	rex_int64 frame_start, frame_end;
	rex_int32 cycles, c;

	// Picture buffers
	Rex::Surface pic_font;
	Rex::Surface pic_bbuffer;
	//Rex::Surface pic_background;
	Rex::Surface pic_cursor;

	// Initialize Rex Engine
	Rex::Initialize();

	// Initialize Graphics
	if (Rex::InitializeGraphics(320, 200, 8) == false) return EXIT_FAILURE;
	Rex::VidInfo vidinfo = Rex::GetVidInfo();

	// Set palette & colormap
	Rex::SetGraphicsPalette("gfx/mindgrdn.pal");
	Rex::ColormapLoad("gfx/mindgrdn.tab");

	// Create picture buffers
	Rex::SurfaceLoadBMP(&pic_font, "gfx/font8x8.bmp");
	Rex::SurfaceLoadBMP(&pic_cursor, "local/cursor.bmp");
	Rex::SurfaceCreate(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);

	Voxel::Initialize(vidinfo.width, vidinfo.height);
	Initialize();

	// Start counting time
	frame_end = Rex::GetTicks64();

	// Main loop
	while (!Rex::KeyTest(REX_KB_ESCAPE))
	{
		// Get start of frame time
		frame_start = Rex::GetTicks64();

		#if (REX_COMPILER == COMPILER_DJGPP)
			cycles = CYCLES * (frame_start - frame_end) / UCLOCKS_PER_SEC;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			cycles = CYCLES * (frame_start - frame_end) / CLOCKS_PER_SEC;
		#endif

		// Cycles
		for (c = 0; c < cycles; c++)
		{
			//
			// User inputs
			//

			CameraController();
		}

		//
		// Rendering
		//

		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 241);

		// Voxels
		{
			// watcom...
			rex_rect screen_area = {0, 0, pic_bbuffer.width, pic_bbuffer.height};

			Voxel::Render(&pic_bbuffer, camera, world, REX_SCALAR(160));
		}

		sprintf(console_buffer, "x: %d y: %d z %d", RexScalarToInteger(camera.origin.x), RexScalarToInteger(camera.origin.y), RexScalarToInteger(camera.origin.z));
		Rex::ConsoleAddText(0, 0, console_buffer);
		sprintf(console_buffer, "pitch: %d yaw: %d roll %d", camera.angles.x, camera.angles.y, camera.angles.z);
		Rex::ConsoleAddText(0, 1, console_buffer);

		// Render the console text
		Rex::ConsoleRender(&pic_bbuffer, &pic_font, 8);

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
	//Rex::SurfaceDestroy(&pic_background);
	Rex::SurfaceDestroy(&pic_cursor);

	// Exit gracefully
	return EXIT_SUCCESS;
}
