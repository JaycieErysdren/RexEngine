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
	// Picture buffers
	Picture::pic_t pic_font;
	Picture::pic_t pic_fbuffer;
	Picture::pic_t pic_bbuffer;

	// Fuck my life
	scalar_t vx1 = SCALAR(70);
	scalar_t vy1 = SCALAR(20);
	scalar_t vx2 = SCALAR(70);
	scalar_t vy2 = SCALAR(70);

	scalar_t px = SCALAR(50);
	scalar_t py = SCALAR(50);
	int32_t angle = 0;

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

			if (DOS::KeyTest(KB_A)) angle -= 4;
			if (DOS::KeyTest(KB_D)) angle += 4;

			if (DOS::KeyTest(KB_W))
			{
				px += FloatToScalar(cos(angle / 180.0f * M_PI));
				py += FloatToScalar(sin(angle / 180.0f * M_PI));
			}

			if (DOS::KeyTest(KB_S))
			{
				px -= FloatToScalar(cos(angle / 180.0f * M_PI));
				py -= FloatToScalar(sin(angle / 180.0f * M_PI));
			}
		}

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Render a world
		{
			int32_t svx1 = ScalarToInteger(vx1);
			int32_t svy1 = ScalarToInteger(vy1);
			int32_t svx2 = ScalarToInteger(vx2);
			int32_t svy2 = ScalarToInteger(vy2);

			int32_t spx = ScalarToInteger(px);
			int32_t spy = ScalarToInteger(py);

			int32_t spxl = ScalarToInteger(FloatToScalar(cos(angle / 180.0f * M_PI) * 10) + px);
			int32_t spyl = ScalarToInteger(FloatToScalar(sin(angle / 180.0f * M_PI) * 10) + py);

			// Draw the player
			Picture::DrawLine(&pic_bbuffer, spx, spy, spxl, spyl, 150);
			Picture::DrawPixel(&pic_bbuffer, spx, spy, 159);

			// Draw the wall
			Picture::DrawLine(&pic_bbuffer, svx1, svy1, svx2, svy2, 31);

			sprintf(console_buffer, "a: %d spxl: %d spyl %d", angle, spxl, spyl);
			Console::AddText(0, 0, console_buffer);
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
