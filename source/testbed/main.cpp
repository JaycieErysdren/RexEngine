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
// LAST EDITED:		December 8th, 2022
//
// ========================================================

// Testbed header
#include "testbed.hpp"

#define CYCLES 30

//
// Types
//

typedef struct
{
	scalar_t cos[360];
	scalar_t sin[360];
	scalar_t tan[360];
} math_t;

typedef struct
{
	vec3s_t origin;				// X, Y, Z
	vec3i_t angles;				// Pitch, yaw, roll
	int draw_distance;			// Draw distance
} camera_t;

//
// Globals
//

#define MAP_X 1024
#define MAP_Y 1024

uint8_t *heightmap;
uint8_t *colormap;
vec3s_t pos;
vec3i_t ang;

camera_t camera;

math_t math;
char console_buffer[256];

//
// Voxels
//

#ifdef FUCKYOUKEN

void VoxelPalette()
{
	outp(0x3c8, 0);

	int z;

	// grayscale
	for (z = 0; z < 64; z++)
	{
		outp(0x3c9, z);
		outp(0x3c9, z);
		outp(0x3c9, z);
	}

	// yellow
	for (z = 0; z < 64; z++)
	{
		outp(0x3c9, z);
		outp(0x3c9, z);
		outp(0x3c9, z / 2);
	}

	// purple
	for (z = 0; z < 64; z++)
	{
		outp(0x3c9, z);
		outp(0x3c9, z / 2);
		outp(0x3c9, z);
	}
}

void VoxelGenerate()
{
	for (int y = 0; y < MAP_Y; y++)
	{
		for (int x = 0; x < MAP_X; x++)
		{
			int x1 = ((x & 31) - 16);
			int y1 = ((y & 31) - 16);
			int d = (15 * 15) - (x1 * x1) - (y1 * y1);

			if (d > 0 && ((x ^ y) & 32))
			{
				heightmap[y][x] = (0 + sqrt(d));
				colormap[y][x] = ((x + y) * 0.5f) + 64;
			}
			else
			{
				heightmap[y][x] = (0);
				colormap[y][x] = ((cos(x * 0.2f) + sin(y * 0.3f)) * 3 + 88) + 64;
			}
		}
	}
}

#endif

void VoxelLoadMap()
{
	int y, x;
	VESA::SetPalette("voxel/m1.pal");

	// Load heightmap
	FILE *hei = fopen("voxel/m1h.dat", "rb");

	heightmap = (uint8_t *)calloc(MAP_X * MAP_Y, sizeof(uint8_t));

	fread(heightmap, sizeof(uint8_t), MAP_X * MAP_Y, hei);

	fclose(hei);

	// Load colormap
	FILE *col = fopen("voxel/m1c.dat", "rb");

	colormap = (uint8_t *)calloc(MAP_X * MAP_Y, sizeof(uint8_t));

	fread(colormap, sizeof(uint8_t), MAP_X * MAP_Y, col);

	fclose(col);
}

void VoxelInit()
{
	// Set Ken's palette
	//VoxelPalette();

	// Generate height and color map
	//VoxelGenerate();

	// Load the map from heightmap and colors
	VoxelLoadMap();

	// Position (scalar units)
	camera.origin.x = SCALAR(32);
	camera.origin.y = SCALAR(32);
	camera.origin.z = SCALAR(96);

	// Angle (degrees)
	camera.angles.x = 0; // pitch
	camera.angles.y = 0; // yaw
	camera.angles.z = 0; // roll`

	// Draw distance (scalar units)
	camera.draw_distance = 128;
}

void VoxelRender(Picture::pic_t *dst, rect_t area)
{
	// Drawable area
	int draw_w = area.x2 - area.x1;
	int draw_h = area.y2 - area.y1;

	// sin and cos of player yaw
	scalar_t sn = math.sin[camera.angles.y];
	scalar_t cs = math.cos[camera.angles.y];

	// Rendering position
	vec3s_t p = camera.origin;

	// Horizon line
	int horizon = 64;

	// Line height scale
	int height_scale = 128;

	// Render back to front
	for (int z = camera.draw_distance; z > 0; z--)
	{
		vec2s_t pleft, pright;

		scalar_t z_s = SCALAR(z);

		pleft.x = (MUL(-cs, z_s) - MUL(sn, z_s)) + p.x;
		pleft.y = (MUL(sn, z_s) - MUL(cs, z_s)) + p.y;

		pright.x = (MUL(cs, z_s) - MUL(sn, z_s)) + p.x;
		pright.y = (MUL(-sn, z_s) - MUL(cs, z_s)) + p.y;

		scalar_t dx = DIV(pright.x - pleft.x, SCALAR(draw_w));
		scalar_t dy = DIV(pright.y - pleft.y, SCALAR(draw_w));

		for (int sx = 0; sx < draw_w; sx++)
		{
			vec2i_t pi;
			pi.x = ScalarToInteger(pleft.x);
			pi.y = ScalarToInteger(pleft.y);

			if (pi.x > (MAP_X - 1)) pi.x -= MAP_X;
			if (pi.x < 0) pi.x += MAP_X;

			if (pi.y > (MAP_Y - 1)) pi.y -= MAP_Y;
			if (pi.y < 0) pi.y += MAP_Y;

			scalar_t height_scale_s = SCALAR(height_scale);
			scalar_t h = SCALAR(heightmap[(pi.y * MAP_Y) + pi.x]);
			scalar_t horizon_s = SCALAR(horizon);

			int line_height = ScalarToInteger(MUL(DIV(p.z - h, z_s), height_scale_s) + horizon_s);

			if (line_height > draw_h) continue;

			line_height = CLAMP(line_height, area.y1, area.y2);

			Picture::DrawVerticalLine(dst, sx, line_height, draw_h, colormap[(pi.y * MAP_Y) + pi.x]);

			pleft.x += dx;
			pleft.y += dy;
		}
	}

	#ifdef FUCKFUCKFUCK

	// Current color and height
	uint8_t c;
	int32_t h;

	// Curerent Ray position
	vec3s_t raypos;
	vec3i_t raypos_i;

	// Ray starting direction
	vec3s_t raydir;

	// Ray horizontal increment value
	vec2s_t rayinc;
	rayinc.x = DIV(SCALAR(2.0f), SCALAR(draw_w));
	rayinc.y = DIV(SCALAR(2.0f), SCALAR(draw_h));

	// screen pixel coordinates
	vec2i_t sc;

	for (sc.x = area.x1; sc.x < area.x2; sc.x++)
	{
		// Ray starting position
		raypos = camera.origin;

		// Calculate ray starting direction
		raydir.x = MUL(rayinc.x, SCALAR(sc.x)) - SCALAR(1.0f);
		raydir.y = SCALAR(1.0f);

		// rotate around 0,0 by player.angles.y
		vec3s_t temp = raydir;

		raydir.x = MUL(temp.x, cs) - MUL(temp.y, sn);
		raydir.y = MUL(temp.x, sn) + MUL(temp.y, cs);

		// min and max draw distance
		for (scalar_t scan = SCALAR(1); scan < camera.draw_distance; scan += SCALAR(1))
		{
			// march the ray
			raypos.x += raydir.x;
			raypos.y += raydir.y;
			raypos.z += raydir.z;

			// round it
			raypos_i.x = ScalarToInteger(raypos.x);
			raypos_i.y = ScalarToInteger(raypos.y);
			raypos_i.z = ScalarToInteger(raypos.z);

			// distance
			scalar_t dist = SCALAR(sqrt(pow(ScalarToFloat(camera.origin.x - raypos.x), 2) + pow(ScalarToFloat(camera.origin.y - raypos.y), 2)));

			// dont cast past the world boundaries
			if (raypos_i.x > 63 || raypos_i.x < 0 || raypos_i.y > 63 || raypos_i.y < 0) break;

			// get current color and height
			c = colormap[raypos_i.y][raypos_i.x];
			h = heightmap[raypos_i.y][raypos_i.x];

			// project it
			sc.y = ScalarToInteger(DIV(MUL((SCALAR(h) - camera.origin.z), SCALAR(-100)), scan * 2)) + (draw_w / 2);

			// draw it
			Picture::DrawPixel(dst, sc.x, sc.y, c);
		}
	}

	// Draw map
	bool draw_map = true;

	if (draw_map == true)
	{
		for (int y = 0; y < 64; y++)
		{
			for (int x = 0; x < 64; x++)
			{
				// Color map
				Picture::DrawPixel(dst, x, y, colormap[y][x]);

				// Height map
				Picture::DrawPixel(dst, x + 64, y, heightmap[y][x]);
			}
		}

		// Camera ray direction
		vec2i_t ray_screen_pos;
		ray_screen_pos.x = ScalarToInteger(camera.origin.x + MUL(raydir.x, SCALAR(8)));
		ray_screen_pos.y = ScalarToInteger(camera.origin.y + MUL(raydir.y, SCALAR(8)));

		// Camera ray position
		Picture::DrawLine(dst, ScalarToInteger(camera.origin.x), ScalarToInteger(camera.origin.y), ray_screen_pos.x, ray_screen_pos.y, 63);
		Picture::DrawLine(dst, ScalarToInteger(camera.origin.x) + 64, ScalarToInteger(camera.origin.y), ray_screen_pos.x + 64, ray_screen_pos.y, 63);

		// Camera position
		Picture::DrawPixel(dst, ScalarToInteger(camera.origin.x), ScalarToInteger(camera.origin.y), 31);
		Picture::DrawPixel(dst, ScalarToInteger(camera.origin.x) + 64, ScalarToInteger(camera.origin.y), 31);
	}

	#endif

	// Idly rotate the camera
	camera.angles.y += 1;
	if (camera.angles.y > 359) camera.angles.y -= 360;
	if (camera.angles.y < 0) camera.angles.y += 360;
}


//
// Misc
//

// Mouse helper function
void ReadMouse(int16_t *buttons, vec2i_t *pos, int16_t speedlimit, rect_t area)
{
	int16_t mx, my, mb, dmx, dmy;
	int16_t halfx = 160, halfy = 100;

	*buttons = DOS::MouseRead(&mx, &my);
	
	dmx = mx - halfx;
	dmy = my - halfy;

	pos->x += (dmx < 0) ? (dmx < -speedlimit ? -speedlimit : dmx) : (dmx > speedlimit ? speedlimit : dmx);
	pos->y += (dmy < 0) ? (dmy < -speedlimit ? -speedlimit : dmy) : (dmy > speedlimit ? speedlimit : dmy);

	// Clip mouse to area
	if (pos->x < area.x1) pos->x = area.x1;
	if (pos->x > area.x2) pos->x = area.x2;
	if (pos->y < area.y1) pos->y = area.y1;
	if (pos->y > area.y2) pos->y = area.y2;

	// 160x100 is the the middle of the 320x200 screen
	// (use this regardless of actual screen resolution)
	DOS::MouseSet(halfx, halfy);
}

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// General variables
	int i;

	// Mouse variables
	vec2i_t mouse_pos;
	int16_t mouse_buttons;
	int16_t mouse_speed;
	rect_t mouse_area;

	// Cycle variables
	int64_t frame_start, frame_end;
	int cycles, c;

	// Picture buffers
	Picture::pic_t pic_font;
	Picture::pic_t pic_bbuffer;
	//Picture::pic_t pic_background;
	Picture::pic_t pic_cursor;

	// Initialize DOS
	DOS::Initialize();

	// Initialize VESA
	if (VESA::Initialize(320, 200, 8) == false) return EXIT_FAILURE;
	VESA::VidInfo vidinfo = VESA::GetVidInfo();

	// Create picture buffers
	Console::Initialize();
	Picture::InitializeFrontBuffer();
	Picture::LoadBMP(&pic_font, "gfx/font8x8.bmp");
	Picture::LoadBMP(&pic_cursor, "local/cursor.bmp");
	//Picture::LoadBMP(&pic_background, "local/forest.bmp");
	Picture::Create(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);

	// Setup mouse variables
	mouse_area = RECT(0, 0, pic_bbuffer.width - pic_cursor.width, pic_bbuffer.height - pic_cursor.height);
	mouse_speed = pic_bbuffer.width / 20;

	// Generate math tables
	for (i = 0; i < 360; i++)
	{
		math.sin[i] = SCALAR(sin(i / 180.0f * M_PI));
		math.cos[i] = SCALAR(cos(i / 180.0f * M_PI));
		math.tan[i] = SCALAR(tan(i / 180.0f * M_PI));
	}

	// Initialize voxel stuff
	VoxelInit();

	// Start counting time
	frame_end = DOS::TimerGet64();

	// Main loop
	while (!DOS::KeyTest(KB_ESC))
	{
		// Get start of frame time
		frame_start = DOS::TimerGet64();
		cycles = CYCLES * (frame_start - frame_end) / UCLOCKS_PER_SEC;

		// Cycles
		for (c = 0; c < cycles; c++)
		{
			//
			// User inputs
			//

			//ReadMouse(&mouse_buttons, &mouse_pos, mouse_speed, mouse_area);

			//
			// Rendering
			//

			// Clear back buffer
			Picture::Clear(&pic_bbuffer, 0);

			// Voxel renderer
			VoxelRender(&pic_bbuffer, RECT(0, 0, pic_bbuffer.width, pic_bbuffer.height));

			// Mouse render
			{
				// Blit the background
				//Picture::Blit8(&pic_bbuffer, 0, 0, pic_bbuffer.width, pic_bbuffer.height, &pic_background, 0, 0, pic_background.width, pic_background.height, Picture::COPY);

				// Blit the mouse
				//Picture::Blit8(&pic_bbuffer, mouse_pos.x, mouse_pos.y, mouse_pos.x + pic_cursor.width, mouse_pos.y + pic_cursor.height, &pic_cursor, 0, 0, pic_cursor.width, pic_cursor.height, Picture::COLORKEY);

				//sprintf(console_buffer, "mx: %d my: %d", mouse_pos.x, mouse_pos.y);
				//Console::AddText(0, 0, console_buffer);
			}

			// Render the console text
			Console::Render(&pic_bbuffer, &pic_font, 8);

			// Flip the rendering buffers
			Picture::CopyToFrontBuffer(&pic_bbuffer);
		}

		// Get end of frame time
		frame_end += cycles * UCLOCKS_PER_SEC / CYCLES;
	}

	// Shutdown VESA
	VESA::Shutdown();

	// Shutdown DOS
	DOS::Shutdown();

	// Cleanup memory
	Console::Shutdown();
	Picture::Destroy(&pic_font);
	Picture::Destroy(&pic_bbuffer);
	//Picture::Destroy(&pic_background);
	Picture::Destroy(&pic_cursor);
	Picture::ShutdownFrontBuffer();

	// Exit gracefully
	return EXIT_SUCCESS;
}
