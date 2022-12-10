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
	scalar_t draw_distance;		// Draw distance (scalar units)
} camera_t;

//
// Globals
//

scalar_t heightmap[64][64];
uint8_t colormap[64][64];
vec3s_t pos;
vec3i_t ang;

camera_t camera;

math_t math;
char console_buffer[256];

//
// Voxels
//

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
	for (int y = 0; y < 64; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			int x1 = ((x & 31) - 16);
			int y1 = ((y & 31) - 16);
			int d = (15 * 15) - (x1 * x1) - (y1 * y1);

			if (d > 0 && ((x ^ y) & 32))
			{
				heightmap[y][x] = SCALAR(0 + sqrt(d));
				colormap[y][x] = ((x + y) * 0.5f) + 64;
			}
			else
			{
				heightmap[y][x] = SCALAR(0);
				colormap[y][x] = ((cos(x * 0.2f) + sin(y * 0.3f)) * 3 + 88) + 64;
			}
		}
	}
}

void VoxelInit()
{
	// Set Ken's palette
	VoxelPalette();

	// Generate height and color map
	VoxelGenerate();

	// Position (scalar units)
	camera.origin.x = SCALAR(32);
	camera.origin.y = SCALAR(32);
	camera.origin.z = SCALAR(32);

	// Angle (degrees)
	camera.angles.x = 0; // pitch
	camera.angles.y = 0; // yaw
	camera.angles.z = 0; // roll

	// Draw distance (scalar units)
	camera.draw_distance = SCALAR(8);
}

void VoxelRender(Picture::pic_t *dst, rect_t area)
{
	// Drawable area
	int draw_w = area.x2 - area.x1;
	int draw_h = area.y2 - area.y1;

	// sin and cos of player yaw
	scalar_t sn = math.sin[camera.angles.y];
	scalar_t cs = math.cos[camera.angles.y];

	// r160
	scalar_t r160 = DIV(SCALAR(1), SCALAR(draw_w / 2));
	scalar_t r100 = DIV(SCALAR(1), SCALAR(draw_h / 2));

	scalar_t draw_distance = MUL(r160, camera.draw_distance);

	// Horizon
	int horizon = -(draw_h / 4);

	// View direction
	vec3s_t raydir;
	raydir.x = cs - sn;
	raydir.y = sn + cs;
	raydir.z = r100;

	vec3s_t rayinc;
	rayinc.x = MUL(-sn, r160);
	rayinc.y = MUL(cs, r160);
	rayinc.z = -r100;

	// Ray position
	vec3s_t raypos;
	
	// Horizontal scan loop 
	for (int sx = area.x1; sx < area.x2; sx++)
	{
		raypos = camera.origin;
		raydir.z = MUL(SCALAR((draw_h / 2) - horizon), r100);

		int sy = area.y2;

		uint8_t c;
		scalar_t h;

		// Ray marching loop
		for (scalar_t scan = 0; scan < camera.draw_distance; scan += SCALAR(1))
		{
			raypos.x += raydir.x;
			raypos.y += raydir.y;
			raypos.z += raydir.z;

			if (raypos.x > SCALAR(63) || raypos.x < SCALAR(0) || raypos.y > SCALAR(63) || raypos.y < SCALAR(0)) break;

			c = colormap[ScalarToInteger(raypos.y)][ScalarToInteger(raypos.x)];
			h = heightmap[ScalarToInteger(raypos.y)][ScalarToInteger(raypos.x)];

			if (scan > 0)
			{
				for (scalar_t z = raypos.z; z > 0; z -= scan)
				{
					if (h > z)
					{
						Picture::DrawPixel(dst, sx, sy, c);
						sy -= 1;
					}
				}
			}
		}

		raydir.x += rayinc.x;
		raydir.y += rayinc.y;
		raydir.z += rayinc.z;

		//Picture::DrawVerticalLine(dst, sx, area.y1, draw_h / 3, 0);
	}

	bool draw_map = true;

	// Draw maps
	if (draw_map == true)
	{
		for (int y = 0; y < 64; y++)
		{
			for (int x = 0; x < 64; x++)
			{
				// Color map
				Picture::DrawPixel(dst, x, y, colormap[y][x]);

				// Height map
				Picture::DrawPixel(dst, x + 64, y, ScalarToInteger(heightmap[y][x]));
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

			Console::AddText(0, 8, "colors");
			Console::AddText(8, 8, "height");

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
