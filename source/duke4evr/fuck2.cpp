// ========================================================
//
// FILE:			/source/duke4evr/main.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Duke4Ever program entry point
//
// LAST EDITED:		December 4th, 2022
//
// ========================================================

// Duke4Ever header
#include "duke4evr.hpp"

#define CYCLES 30

#define MAP_W 8
#define MAP_H 8
#define MAP_S 16

int8_t map[MAP_H][MAP_W] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

typedef struct
{
	int32_t x, y, z;
} vec3i_t;

typedef struct
{
	scalar_t x, y, z;
} vec3s_t;

struct player
{
	// Scalar vectors
	vec3s_t origin;
	vec3s_t movedir;

	// Integer vectors
	vec3i_t angle; // pitch, yaw, roll

	// Integers
	int movespeedkey;
	int anglespeedkey;
} player;

struct math
{
	scalar_t cos[360];
	scalar_t sin[360];
	scalar_t tan[360];
} math;

// Main entry point
int main(int argc, char *argv[])
{
	// General variables
	int i;

	// Picture buffers
	Picture::pic_t pic_font;
	Picture::pic_t pic_fbuffer;
	Picture::pic_t pic_bbuffer;

	// Console buffer
	char console_buffer[256];

	// Cycles
	int64_t frame_start, frame_end;
	int cycles, c;

	// Generate math table
	for (i = 0; i < 360; i++)
	{
		math.sin[i] = SCALAR(sin(i / 180.0f * M_PI));
		math.cos[i] = SCALAR(cos(i / 180.0f * M_PI));
		math.tan[i] = SCALAR(tan(i / 180.0f * M_PI));
	}

	// Player starting variables
	player.origin.x = SCALAR(24.0f);
	player.origin.y = SCALAR(24.0f);
	player.origin.z = SCALAR(0.0f);
	player.angle.x = 0;
	player.angle.y = 0;
	player.angle.z = 0;
	player.movespeedkey = 4;
	player.anglespeedkey = 4;

	// Initialize DOS
	DOS::Initialize();

	// Initialize VGA
	VGA::Initialize();
	VGA::SetPalette("gfx/duke3d.pal");

	// Create pictures
	Console::Initialize();
	Picture::LoadBMP(&pic_font, "gfx/font8x8.bmp");
	Picture::Create(&pic_fbuffer, SCREEN_WIDTH, SCREEN_HEIGHT, 8, 0, (void *)VGA_VIDMEM_PTR);
	Picture::Create(&pic_bbuffer, SCREEN_WIDTH, SCREEN_HEIGHT, 8, 0, 0);

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
			// Input handling
			//

			// Rotate leftwards
			if (DOS::KeyTest(KB_LTARROW))
			{
				player.angle.y += player.anglespeedkey;
				if (player.angle.y > 359) player.angle.y -= 360;
			}

			// Rotate rightwards
			if (DOS::KeyTest(KB_RTARROW))
			{
				player.angle.y -= player.anglespeedkey;
				if (player.angle.y < 0) player.angle.y += 360;
			}
	
			// Set movedir
			player.movedir.x = math.sin[player.angle.y] * player.movespeedkey;
			player.movedir.y = math.cos[player.angle.y] * player.movespeedkey;
			player.movedir.z = SCALAR(1.0f) * player.movespeedkey;

			// Move forwards
			if (DOS::KeyTest(KB_W))
			{
				player.origin.x += player.movedir.x;
				player.origin.y += player.movedir.y;
			}

			// Move backwards
			if (DOS::KeyTest(KB_S))
			{
				player.origin.x -= player.movedir.x;
				player.origin.y -= player.movedir.y;
			}

			// Move leftwards
			if (DOS::KeyTest(KB_A))
			{
				player.origin.x += player.movedir.y;
				player.origin.y -= player.movedir.x;
			}

			// Move rightwards
			if (DOS::KeyTest(KB_D))
			{
				player.origin.x -= player.movedir.y;
				player.origin.y += player.movedir.x;
			}
		}

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Render 2D map
		{
			for (int y = 0; y < MAP_H; y++)
			{
				for (int x = 0; x < MAP_W; x++)
				{
					uint8_t color;

					if (map[y][x] == 1) color = 31; else color = 0;

					Picture::DrawRectangle(&pic_bbuffer, x * MAP_S, y * MAP_S, MAP_S, MAP_S, color, true);
				}
			}
		}

		// Render rays
		{
			int r, mx, my, mp, dof, ra;
			scalar_t rx, ry, xo, yo, atan;

			ra = player.angle.y;

			for (r = 0; r < 1; r++)
			{
				dof = 0;
				atan = 0 - math.tan[ra];

				if (ra > 180)
				{
					ry = (ry / 64) * 64;
					rx = (player.origin.y - ry) * atan + player.origin.x;
					yo = -64;
					xo = -yo * atan;
				}

				if (ra < 180)
				{
					ry = ((ry / 64) * 64) + 64;
					rx = (player.origin.y - ry) * atan + player.origin.x;
					yo = 64;
					xo = -yo * atan;
				}

				if (ra == 0 || ra == 180)
				{
					rx = player.origin.x;
					ry = player.origin.y;
					dof = 8;
				}

				while (dof < 8)
				{
					mx = ScalarToInteger(rx / 64);
					my = ScalarToInteger(ry / 64);
					mp = my * MAP_W + mx;

					if (mp < MAP_W * MAP_H && map[my][mx] == 1)
					{
						dof = 8;
					}
					else
					{
						rx += xo;
						ry += yo;
					}
				}

				Picture::DrawLine(&pic_bbuffer, ScalarToInteger(player.origin.x), ScalarToInteger(player.origin.y), ScalarToInteger(rx), ScalarToInteger(ry), 150);
			}
		}

		// Render player
		{
			//Picture::DrawLine(&pic_bbuffer, ScalarToInteger(player.origin.x), ScalarToInteger(player.origin.y), ScalarToInteger(player.origin.x + player.movedir.x * 2), ScalarToInteger(player.origin.y + player.movedir.y * 2), 150);
			Picture::DrawPixel(&pic_bbuffer, ScalarToInteger(player.origin.x), ScalarToInteger(player.origin.y), 159);
		}

		// Render the console text
		Console::Render(&pic_bbuffer, &pic_font);

		// Flip the rendering buffers
		Picture::Copy(&pic_fbuffer, &pic_bbuffer);

		// Get end of frame time
		frame_end = frame_end + cycles * UCLOCKS_PER_SEC / CYCLES;
	}

	// Shutdown VGA
	VGA::Shutdown();

	// Shutdown DOS
	DOS::Shutdown();

	// Cleanup memory
	Console::Shutdown();
	Picture::Destroy(&pic_font);
	Picture::Destroy(&pic_fbuffer);
	Picture::Destroy(&pic_bbuffer);

	// Exit gracefully
	return EXIT_SUCCESS;
}
