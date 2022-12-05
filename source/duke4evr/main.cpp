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

//
// Types
//

// 3-point integer vector
typedef struct
{
	int32_t x, y, z;
} vec3i_t;

// 3-point scalar vector
typedef struct
{
	scalar_t x, y, z;
} vec3s_t;

//
// Globals
//

// player
struct player
{
	// Scalar vectors
	vec3s_t origin; 	// x, y, z
	vec3s_t movedir;	// x, y, z
	vec3i_t angle;		// pitch, yaw, roll

	// Speed keys
	int movespeedkey;
	int anglespeedkey;
} player;

// math tables
struct math
{
	scalar_t cos[360];
	scalar_t sin[360];
	scalar_t tan[360];
} math;

//
// Drawing functions
//

void RenderWall(Picture::pic_t *dst, int x1, int x2, int yb1, int yb2, int yt1, int yt2, uint8_t color)
{
	int x, y;

	// Distance deltas
	int dyb = yb2 - yb1;
	int dyt = yt2 - yt1;

	int dx = x2 - x1;
	if (dx == 0) dx = 1;

	int xs = x1;

	Picture::DrawLine(dst, x1, yb1, x2, yb2, color);
	Picture::DrawLine(dst, x1, yt1, x2, yt2, color);

	return;

	for (x = x1; x < x2; x++)
	{
		int y1 = dyb * (x - xs) / dx + yb1;
		int y2 = dyt * (x - xs) / dx + yt1;

		// fill in the wall with vertical lines
		//Picture::DrawVerticalLine(dst, x, y1, y2, color);

		// just draw the top and bottom lines
		Picture::DrawPixel(dst, x, y1, color);
		Picture::DrawPixel(dst, x, y2, color);
	}
}

//
// Main entry point
//

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

	// Player variables
	player.origin.x = SCALAR(70.0f);
	player.origin.y = SCALAR(-110.0f);
	player.origin.z = SCALAR(20.0f);
	player.angle.x = 0;
	player.angle.y = 0;
	player.angle.z = 0;
	player.anglespeedkey = 4;
	player.movespeedkey = 4;

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

			// Mouse read
			int16_t mb, mx, my;
			mb = DOS::MouseRead(&mx, &my);

			// Rotate leftwards
			if (DOS::KeyTest(KB_LTARROW))
			{
				player.angle.y -= player.anglespeedkey;
				if (player.angle.y < 0) player.angle.y += 360;
			}

			// Rotate rightwards
			if (DOS::KeyTest(KB_RTARROW))
			{
				player.angle.y += player.anglespeedkey;
				if (player.angle.y > 359) player.angle.y -= 360;
			}

			// Look upwards
			if (DOS::KeyTest(KB_UPARROW))
			{
				player.angle.x += player.anglespeedkey;
				if (player.angle.x >= 180) player.angle.x = 180;
			}

			// Look downwards
			if (DOS::KeyTest(KB_DNARROW))
			{
				player.angle.x -= player.anglespeedkey;
				if (player.angle.x <= -180) player.angle.x = -180;
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
				player.origin.x -= player.movedir.y;
				player.origin.y += player.movedir.x;
			}

			// Move rightwards
			if (DOS::KeyTest(KB_D))
			{
				player.origin.x += player.movedir.y;
				player.origin.y -= player.movedir.x;
			}

			// Move upwards
			if (DOS::KeyTest(KB_Q))
				player.origin.z += player.movedir.z;

			// Move downwards
			if (DOS::KeyTest(KB_E))
				player.origin.z -= player.movedir.z;
		}

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Render a world
		{
			scalar_t wv[4][3]; // World space vertices (x, y, z)
			scalar_t pv1[4][3]; // Player space vertices (part 1) (x, y, z)
			scalar_t pv2[4][3]; // Player space vertices (part 2) (x, y, z)
			int32_t sv[4][2]; // Screen space vertices (x, y)

			// Current cos and sin of player yaw
			scalar_t cs = math.cos[player.angle.y];
			scalar_t sn = math.sin[player.angle.y];

			// World space vertices
			wv[0][0] = SCALAR(40);
			wv[0][1] = SCALAR(10);
			wv[0][2] = SCALAR(0);

			wv[1][0] = SCALAR(40);
			wv[1][1] = SCALAR(290);
			wv[1][2] = SCALAR(0);

			wv[2][0] = SCALAR(40);
			wv[2][1] = SCALAR(10);
			wv[2][2] = SCALAR(64);

			wv[3][0] = SCALAR(40);
			wv[3][1] = SCALAR(290);
			wv[3][2] = SCALAR(64);

			// Player space vertices (part 1)
			pv1[0][0] = wv[0][0] - player.origin.x;
			pv1[0][1] = wv[0][1] - player.origin.y;
			pv1[0][2] = wv[0][2] - player.origin.z;

			pv1[1][0] = wv[1][0] - player.origin.x;
			pv1[1][1] = wv[1][1] - player.origin.y;
			pv1[1][2] = wv[1][2] - player.origin.z;

			pv1[2][0] = wv[2][0] - player.origin.x;
			pv1[2][1] = wv[2][1] - player.origin.y;
			pv1[2][2] = wv[2][2] - player.origin.z;

			pv1[3][0] = wv[3][0] - player.origin.x;
			pv1[3][1] = wv[3][1] - player.origin.y;
			pv1[3][2] = wv[3][2] - player.origin.z;

			// Player space vertices (part 2)
			pv2[0][0] = MUL(pv1[0][0], cs) - MUL(pv1[0][1], sn);
			pv2[0][1] = MUL(pv1[0][1], cs) + MUL(pv1[0][0], sn);
			pv2[0][2] = 0 - pv1[0][2] + DIV(MUL(SCALAR(player.angle.x), pv2[0][1]), SCALAR(32));

			pv2[1][0] = MUL(pv1[1][0], cs) - MUL(pv1[1][1], sn);
			pv2[1][1] = MUL(pv1[1][1], cs) + MUL(pv1[1][0], sn);
			pv2[1][2] = 0 - pv1[1][2] + DIV(MUL(SCALAR(player.angle.x), pv2[1][1]), SCALAR(32));

			pv2[2][0] = MUL(pv1[2][0], cs) - MUL(pv1[2][1], sn);
			pv2[2][1] = MUL(pv1[2][1], cs) + MUL(pv1[2][0], sn);
			pv2[2][2] = 0 - pv1[2][2] + DIV(MUL(SCALAR(player.angle.x), pv2[2][1]), SCALAR(32));

			pv2[3][0] = MUL(pv1[3][0], cs) - MUL(pv1[3][1], sn);
			pv2[3][1] = MUL(pv1[3][1], cs) + MUL(pv1[3][0], sn);
			pv2[3][2] = 0 - pv1[3][2] + DIV(MUL(SCALAR(player.angle.x), pv2[3][1]), SCALAR(32));

			// Screen space vertices (with divide-by-zero checking)
			if (pv2[0][1] != 0 && pv2[1][1] != 0 && pv2[2][1] != 0 && pv2[3][1] != 0)
			{
				sv[0][0] = ScalarToInteger(DIV(MUL(pv2[0][0], SCALAR(200)), pv2[0][1])) + (SCREEN_WIDTH / 2);
				sv[0][1] = ScalarToInteger(DIV(MUL(pv2[0][2], SCALAR(200)), pv2[0][1])) + (SCREEN_HEIGHT / 2);

				sv[1][0] = ScalarToInteger(DIV(MUL(pv2[1][0], SCALAR(200)), pv2[1][1])) + (SCREEN_WIDTH / 2);
				sv[1][1] = ScalarToInteger(DIV(MUL(pv2[1][2], SCALAR(200)), pv2[1][1])) + (SCREEN_HEIGHT / 2);

				sv[2][0] = ScalarToInteger(DIV(MUL(pv2[2][0], SCALAR(200)), pv2[2][1])) + (SCREEN_WIDTH / 2);
				sv[2][1] = ScalarToInteger(DIV(MUL(pv2[2][2], SCALAR(200)), pv2[2][1])) + (SCREEN_HEIGHT / 2);

				sv[3][0] = ScalarToInteger(DIV(MUL(pv2[3][0], SCALAR(200)), pv2[3][1])) + (SCREEN_WIDTH / 2);
				sv[3][1] = ScalarToInteger(DIV(MUL(pv2[3][2], SCALAR(200)), pv2[3][1])) + (SCREEN_HEIGHT / 2);

				// RenderWall
				RenderWall(&pic_bbuffer, sv[0][0], sv[1][0], sv[0][1], sv[1][1], sv[2][1], sv[3][1], 159);

				// Dots
				//Picture::DrawPixel(&pic_bbuffer, sv[0][0], sv[0][1], 159);
				//Picture::DrawPixel(&pic_bbuffer, sv[1][0], sv[1][1], 159);
			}

			// Print some console info
			sprintf(console_buffer, "px: %d py: %d pz: %d", ScalarToInteger(player.origin.x), ScalarToInteger(player.origin.y), ScalarToInteger(player.origin.z));
			Console::AddText(0, 0, console_buffer);
			sprintf(console_buffer, "sv0: %d sv1: %d", sv[0][0], sv[0][1]);
			Console::AddText(0, 1, console_buffer);
			sprintf(console_buffer, "sv0: %d sv1: %d", sv[1][0], sv[1][1]);
			Console::AddText(0, 2, console_buffer);
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
