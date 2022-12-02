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

#define MAP_X 8
#define MAP_Y 8
#define MAP_SIZE 8
#define PLAYER_SIZE 2
#define PLAYER_SPEED FIX32(0.133333f)

int map[MAP_Y][MAP_X] =
{
	{1,1,1,1,1,1,1,1},
	{1,0,1,0,0,0,0,1},
	{1,0,1,0,0,0,0,1},
	{1,0,1,0,0,0,0,1},
	{1,0,0,0,0,0,0,1},
	{1,0,0,0,0,1,0,1},
	{1,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1},
};

// Main entry point
int main(int argc, char *argv[])
{
	// Variables
	Picture::pic_t pic_font;
	Picture::pic_t pic_bbuffer;
	Picture::pic_t pic_fbuffer;
	Picture::pic_t pic_cursor;
	Picture::pic_t pic_shotgun;
	char console_buffer[256];
	fix32_t px = FIX32(12.0f), py = FIX32(16.0f);
	uint8_t pa = 64;
	fix32_t pdx = FIX32(-1.0f), pdy = FIX32(0.0f);

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

	// Main loop
	while (!DOS::KeyTest(KB_ESC))
	{
		//
		// Input handling
		//

		if (DOS::KeyTest(KB_A))
		{
			pa += 1;
			pdx = cosFixed(pa);
			pdy = sinFixed(pa);
		}

		if (DOS::KeyTest(KB_D))
		{
			pa -= 1;
			pdx = cosFixed(pa);
			pdy = sinFixed(pa);
		}

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

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Raycaster rendering
		{
			// Draw map
			{
				int x, y, xo, yo;
				uint8_t color;

				for (y = 0; y < MAP_Y; y++)
				{
					for (x = 0; x < MAP_X; x++)
					{
						xo = x * MAP_SIZE;
						yo = y * MAP_SIZE;

						if (map[y][x] == 1)
							Picture::DrawRectangle(&pic_bbuffer, xo, yo, MAP_SIZE, MAP_SIZE, 47, true);
						else
							Picture::DrawRectangle(&pic_bbuffer, xo, yo, MAP_SIZE, MAP_SIZE, 0, true);
					}
				}
			}

			// Draw player
			{
				Picture::DrawRectangle(&pic_bbuffer, FIX2INT32(px) - 1, FIX2INT32(py) - 1, 2, 2, 159, true);
			}
		}

		// Render shotgun
		Picture::Draw8(&pic_bbuffer, &pic_shotgun, 168, 116, Picture::COLORKEY);

		// Render the console text
		Console::Render(&pic_bbuffer, &pic_font);

		// Flip the rendering buffers
		Picture::Copy(&pic_fbuffer, &pic_bbuffer);
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
