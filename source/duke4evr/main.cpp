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
// LAST EDITED:		December 1st, 2022
//
// ========================================================

// Duke4Ever header
#include "duke4evr.hpp"

#define MAP_W 24
#define MAP_H 24
#define MAP_SIZE 8
#define PLAYER_SIZE 2
#define PLAYER_SPEED FIX32(0.133333f)
#define CYCLES 30

int map[MAP_W][MAP_H] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Main entry point
int main(int argc, char *argv[])
{
	// Picture buffers
	Picture::pic_t pic_font;
	Picture::pic_t pic_bbuffer;
	Picture::pic_t pic_fbuffer;
	Picture::pic_t pic_cursor;
	Picture::pic_t pic_shotgun;
	
	// Console buffer
	char console_buffer[256];

	// Cycles
	int64_t frame_start, frame_end;
	int cycles, c;

	// Player inforamtion
	fix32_t px = FIX32(12.0f), py = FIX32(16.0f);
	uint8_t pa = 255;
	fix32_t pdx = FIX32(-1.0f), pdy = FIX32(0.0f);
	fix32_t plane_x = FIX32(0.0f), plane_y = FIX32(0.66f);

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
	Picture::LoadBMP(&pic_cursor, "gfx/cursor.bmp");
	Picture::LoadBMP(&pic_shotgun, "gfx/shot001a.bmp");

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

			if (DOS::KeyTest(KB_A))
			{
				pa += 4;
			}

			if (DOS::KeyTest(KB_D))
			{
				pa -= 4;
			}

			pdx = cosFixed(pa);
			pdy = sinFixed(pa);

			if (DOS::KeyTest(KB_W))
			{
				px += pdx;
				py -= pdy;
			}

			if (DOS::KeyTest(KB_S))
			{
				px -= pdx;
				py += pdy;
			}
		}

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Render world
		{


		}

		// Render shotgun
		Picture::Draw8(&pic_bbuffer, &pic_shotgun, 168, 116, Picture::COLORKEY);

		// Map overlay
		//if (DOS::KeyTest(KB_F))
		{
			// Draw map
			{
				for (int y = 0; y < MAP_H; y++)
				{
					for (int x = 0; x < MAP_W; x++)
					{
						uint8_t color;

						switch (map[y][x])
						{
							case 1: color = 47; break;
							case 2: color = 127; break;
							case 3: color = 149; break;
							case 4: color = 191; break;
							case 5: color = 255; break;
							default: color = 0; break;
						}

						Picture::DrawRectangle(&pic_bbuffer, x * MAP_SIZE, y * MAP_SIZE, MAP_SIZE, MAP_SIZE, color, true);
					}
				}
			}

			// Draw player
			{
				Picture::DrawRectangle(&pic_bbuffer, FIX2INT32(px) - 1, FIX2INT32(py) - 1, 2, 2, 159, true);
			}
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
	Picture::Destroy(&pic_shotgun);

	// Exit gracefully
	return EXIT_SUCCESS;
}
