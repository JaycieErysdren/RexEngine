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

#define mapX 8
#define mapY 8
#define mapS 8
#define playerS 2
#define playerSpeed FIX32(0.05f)

int map[] =
{
	1,1,1,1,1,1,1,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,1,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,
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
	fix32_t px = FIX32(12.0f), py = FIX32(16.0f), pa = 0;
	fix32_t pdx, pdy;

	// Wall picture
	Picture::pic_t pic_wall;
	void *pic_wall_pixels = new uint8_t [mapS * mapS];
	memset(pic_wall_pixels, 47, mapS * mapS);
	Picture::Create(&pic_wall, mapS, mapS, 8, 0, pic_wall_pixels);

	// Floor picture
	Picture::pic_t pic_floor;
	void *pic_floor_pixels = new uint8_t [mapS * mapS];
	memset(pic_floor_pixels, 0, mapS * mapS);
	Picture::Create(&pic_floor, mapS, mapS, 8, 0, pic_floor_pixels);

	// Player picture
	Picture::pic_t pic_player;
	void *pic_player_pixels = new uint8_t [4];
	memset(pic_player_pixels, 159, 4);
	Picture::Create(&pic_player, 2, 2, 8, 0, pic_player_pixels);

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

		// Get the mouse cursor input
		#ifdef MOUSE_CURSOR
		int16_t mx, my, mb;
		mb = DOS::MouseRead(&mx, &my);
		if (mx >= SCREEN_WIDTH - pic_cursor.width) mx = SCREEN_WIDTH - pic_cursor.width;
		if (my >= SCREEN_HEIGHT - pic_cursor.height) my = SCREEN_HEIGHT - pic_cursor.height;
		#endif

		if (DOS::KeyTest(KB_A))
			px -= FIX32(0.1f);

		if (DOS::KeyTest(KB_D))
			px += FIX32(0.1f);

		if (DOS::KeyTest(KB_W))
			py -= FIX32(0.1f);

		if (DOS::KeyTest(KB_S))
			py += FIX32(0.1f);

		#ifdef BAD_PLAYER_CODE

		// Reset velocity at the start of each frame
		pdx = pdy = 0;

		if (DOS::KeyTest(KB_A))
		{
			pa -= FIX32(0.1f);
		}

		if (DOS::KeyTest(KB_D))
		{
			pa += FIX32(0.1f);
		}

		if (DOS::KeyTest(KB_W))
		{
			pdx += COS32(pa);
			pdy -= SIN32(pa);
		}

		if (DOS::KeyTest(KB_S))
		{
			pdx -= COS32(pa);
			pdy += SIN32(pa);
		}

		// Limit player angle
		if (pa < 0) pa = INT2FIX32(360);
		if (pa > INT2FIX32(360)) pa = INT2FIX32(0);

		// add velocity to player position
		px += pdx;
		py += pdy;

		#endif

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Raycaster rendering
		{
			// Draw world
			{
				int32_t r, mx, my, mp, dof;
				fix32_t rx, ry, ra, xo, yo;
			}

			// Draw map
			{
				int x, y, xo, yo;
				uint8_t color;

				for (y = 0; y < mapY; y++)
				{
					for (x = 0; x < mapX; x++)
					{
						xo = x * mapS;
						yo = y * mapS;

						if (map[y * mapX + x] == 1)
							Picture::Blit8(&pic_bbuffer, xo, yo, xo + mapS, yo + mapS, &pic_wall, 0, 0, pic_wall.width, pic_wall.height, Picture::COPY);
						else
							Picture::Blit8(&pic_bbuffer, xo, yo, xo + mapS, yo + mapS, &pic_floor, 0, 0, pic_floor.width, pic_floor.height, Picture::COPY);
					}
				}
			}

			// Draw player
			{
				Picture::Blit8(&pic_bbuffer, FIX2INT32(px) - 1, FIX2INT32(py) - 1, FIX2INT32(px) + 1, FIX2INT32(py) + 1, &pic_player, 0, 0, 1, 1, Picture::COPY);
			}
		}

		// does this work
		//Picture::DrawRectangle(&pic_bbuffer, 32, 32, 32, 32, 159, true);
		//Picture::DrawRectangle(&pic_bbuffer, 64, 64, 32, 32, 159, false);

		// Render shotgun
		Picture::Draw8(&pic_bbuffer, &pic_shotgun, 168, 116, Picture::COLORKEY);

		// Add some console text
		sprintf(console_buffer, "px: %d py: %d", FIX2INT32(px), FIX2INT32(py));
		Console::AddText(8, 0, console_buffer);
		sprintf(console_buffer, "pa: %d pdx: %d pdy: %d", FIX2INT32(pa), FIX2INT32(pdx), FIX2INT32(pdy));
		Console::AddText(8, 1, console_buffer);
		sprintf(console_buffer, "sin pa: %d cos pa: %d", FIX2INT32(SIN32(pa)), FIX2INT32(COS32(pa)));
		Console::AddText(8, 2, console_buffer);

		#ifdef MOUSE_CURSOR
		// Add some console text
		sprintf(console_buffer, "Mouse X: %d Mouse Y: %d Mouse B: %d", mx, my, mb);
		Console::AddText(0, 0, console_buffer);

		// Render the mouse cursor
		Picture::Draw8(&pic_bbuffer, &pic_cursor, mx, my, Picture::COLORKEY);
		#endif

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
	Picture::Destroy(&pic_wall);
	Picture::Destroy(&pic_floor);
	Picture::Destroy(&pic_player);

	// Exit gracefully
	return EXIT_SUCCESS;
}
