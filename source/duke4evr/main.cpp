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

// Painful globals
Picture::pic_t pic_bbuffer;

#define CYCLES 30

#define LEVEL_W 20
#define LEVEL_H 15
#define LEVEL_SIZE 8

int8_t level[LEVEL_W * LEVEL_H] = // here 1 means wall, 0 floor
{
//	0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, // 0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, // 1
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, // 2
	1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, // 3
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, // 4
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, // 5
	1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, // 7
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, // 8
	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
	0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, // 10
	0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, // 11
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, // 12
	0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, // 13
	0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0  // 14
};

// Main entry point
int main(int argc, char *argv[])
{
	// raycastlib stuff
	RCL::Initialize(2, 2, 0, LEVEL_W, LEVEL_H, level);

	// Picture buffers
	Picture::pic_t pic_font;
	Picture::pic_t pic_fbuffer;
	Picture::pic_t pic_cursor;
	Picture::pic_t pic_shotgun;
	
	// Console buffer
	char console_buffer[256];

	// Cycles
	int64_t frame_start, frame_end;
	int cycles, c;

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
			#ifdef FUCK
			//
			// Input handling
			//
			RCL_Vector2D angle = RCL_angleToDirection(camera.direction);

			int step = 1;
			int step2 = 5;

			angle.x /= 10;
			angle.y /= 10;

			if (DOS::KeyTest(KB_D))
				camera.direction += step2;
			
			if (DOS::KeyTest(KB_A))
				camera.direction -= step2;

			if (DOS::KeyTest(KB_W))
			{
				camera.position.x += step * angle.x;
				camera.position.y += step * angle.y;
			}

			if (DOS::KeyTest(KB_S))
			{
				camera.position.x -= step * angle.x;
				camera.position.y -= step * angle.y;
			}
			#endif
		}

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Render world
		{
			RCL::Render(&pic_bbuffer, 1, 40);
		}

		// Render shotgun
		Picture::Draw8(&pic_bbuffer, &pic_shotgun, 168, 116, Picture::COLORKEY);

		// Map overlay
		if (DOS::KeyTest(KB_F))
		{
			// Draw level map
			{
				for (int y = 0; y < LEVEL_H; y++)
				{
					for (int x = 0; x < LEVEL_W; x++)
					{
						uint8_t color;

						switch (level[y * LEVEL_W + x])
						{
							case 1: color = 47; break;
							case 2: color = 127; break;
							case 3: color = 149; break;
							case 4: color = 191; break;
							case 5: color = 255; break;
							default: color = 0; break;
						}

						Picture::DrawRectangle(&pic_bbuffer, x * LEVEL_SIZE, y * LEVEL_SIZE, LEVEL_SIZE, LEVEL_SIZE, color, true);
					}
				}
			}

			// Draw player
			{
				#ifdef FUCK
				Picture::DrawRectangle(&pic_bbuffer, FIX2INT32(camera.position.x) - 1, FIX2INT32(camera.position.y) - 1, 2, 2, 159, true);
				#endif
			}
		}

		// Console
		//sprintf(console_buffer, "cx: %d cy: %d", FIX2INT32(camera.position.x), FIX2INT32(camera.position.y));
		//Console::AddText(0, 0, console_buffer);

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
