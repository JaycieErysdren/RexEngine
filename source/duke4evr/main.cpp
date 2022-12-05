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

// 2-point integer vector
typedef struct
{
	int32_t x, y;
} vec2i_t;

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

	// clip x
	if (x1 < 1) x1 = 1;
	if (x2 < 1) x2 = 1;
	if (x1 > dst->width - 1) x1 = dst->width - 1;
	if (x2 > dst->width - 1) x2 = dst->width - 1;

	for (x = x1; x < x2; x++)
	{
		int y1 = dyb * (x - xs) / dx + yb1;
		int y2 = dyt * (x - xs) / dx + yt1;

		// clip y
		if (y1 < 1) y1 = 1;
		if (y2 < 1) y2 = 1;
		if (y1 > dst->height - 1) y1 = dst->height - 1;
		if (y2 > dst->height - 1) y2 = dst->height - 1;

		Picture::DrawVerticalLine(dst, x, y1, y2, color);
	}
}

void ClipWall(scalar_t *x1, scalar_t *y1, scalar_t *z1, scalar_t x2, scalar_t y2, scalar_t z2)
{
	scalar_t da = *y1;
	scalar_t db = y2;
	scalar_t d = da - db;
	if (d == SCALAR(0)) d = SCALAR(1);
	scalar_t s = DIV(da, d);
	*x1 = *x1 + MUL(s, x2 - (*x1));
	*y1 = *y1 + MUL(s, y2 - (*y1));
	*z1 = *z1 + MUL(s, z2 - (*z1));

	if (*y1 == SCALAR(0)) *y1 = SCALAR(1);
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
			vec3s_t wv[4]; // World space vertices
			vec3s_t pv1[4]; // Player space vertices (part 1)
			vec3s_t pv2[4]; // Player space vertices (part 2)
			vec2i_t sv[4]; // Screen space vertices

			// Current cos and sin of player yaw
			scalar_t cs = math.cos[player.angle.y];
			scalar_t sn = math.sin[player.angle.y];

			// World space vertices
			wv[0].x = SCALAR(40);
			wv[0].y = SCALAR(10);
			wv[0].z = SCALAR(0);

			wv[1].x = SCALAR(40);
			wv[1].y = SCALAR(290);
			wv[1].z = SCALAR(0);

			wv[2].x = SCALAR(40);
			wv[2].y = SCALAR(10);
			wv[2].z = SCALAR(64);

			wv[3].x = SCALAR(40);
			wv[3].y = SCALAR(290);
			wv[3].z = SCALAR(64);

			// Player space vertices (part 1)
			pv1[0].x = wv[0].x - player.origin.x;
			pv1[0].y = wv[0].y - player.origin.y;
			pv1[0].z = wv[0].z - player.origin.z;

			pv1[1].x = wv[1].x - player.origin.x;
			pv1[1].y = wv[1].y - player.origin.y;
			pv1[1].z = wv[1].z - player.origin.z;

			pv1[2].x = wv[2].x - player.origin.x;
			pv1[2].y = wv[2].y - player.origin.y;
			pv1[2].z = wv[2].z - player.origin.z;

			pv1[3].x = wv[3].x - player.origin.x;
			pv1[3].y = wv[3].y - player.origin.y;
			pv1[3].z = wv[3].z - player.origin.z;

			// Player space vertices (part 2)
			pv2[0].x = MUL(pv1[0].x, cs) - MUL(pv1[0].y, sn);
			pv2[0].y = MUL(pv1[0].y, cs) + MUL(pv1[0].x, sn);
			pv2[0].z = 0 - pv1[0].z + DIV(MUL(SCALAR(player.angle.x), pv2[0].y), SCALAR(32));

			pv2[1].x = MUL(pv1[1].x, cs) - MUL(pv1[1].y, sn);
			pv2[1].y = MUL(pv1[1].y, cs) + MUL(pv1[1].x, sn);
			pv2[1].z = 0 - pv1[1].z + DIV(MUL(SCALAR(player.angle.x), pv2[1].y), SCALAR(32));

			pv2[2].x = MUL(pv1[2].x, cs) - MUL(pv1[2].y, sn);
			pv2[2].y = MUL(pv1[2].y, cs) + MUL(pv1[2].x, sn);
			pv2[2].z = 0 - pv1[2].z + DIV(MUL(SCALAR(player.angle.x), pv2[2].y), SCALAR(32));

			pv2[3].x = MUL(pv1[3].x, cs) - MUL(pv1[3].y, sn);
			pv2[3].y = MUL(pv1[3].y, cs) + MUL(pv1[3].x, sn);
			pv2[3].z = 0 - pv1[3].z + DIV(MUL(SCALAR(player.angle.x), pv2[3].y), SCALAR(32));

			// Screen space vertices (with divide-by-zero checking)
			{
				if (pv2[0].y <= SCALAR(0))
				{
					ClipWall(&pv2[0].x, &pv2[0].y, &pv2[0].z, pv2[1].x, pv2[1].y, pv2[1].z);
					ClipWall(&pv2[2].x, &pv2[2].y, &pv2[2].z, pv2[3].x, pv2[3].y, pv2[3].z);
				}

				if (pv2[1].y <= SCALAR(0))
				{
					ClipWall(&pv2[1].x, &pv2[1].y, &pv2[1].z, pv2[0].x, pv2[0].y, pv2[0].z);
					ClipWall(&pv2[3].x, &pv2[3].y, &pv2[3].z, pv2[2].x, pv2[2].y, pv2[2].z);
				}

				sv[0].x = ScalarToInteger(DIV(MUL(pv2[0].x, SCALAR(200)), pv2[0].y)) + (SCREEN_WIDTH / 2);
				sv[0].y = ScalarToInteger(DIV(MUL(pv2[0].z, SCALAR(200)), pv2[0].y)) + (SCREEN_HEIGHT / 2);

				sv[1].x = ScalarToInteger(DIV(MUL(pv2[1].x, SCALAR(200)), pv2[1].y)) + (SCREEN_WIDTH / 2);
				sv[1].y = ScalarToInteger(DIV(MUL(pv2[1].z, SCALAR(200)), pv2[1].y)) + (SCREEN_HEIGHT / 2);

				sv[2].x = ScalarToInteger(DIV(MUL(pv2[2].x, SCALAR(200)), pv2[2].y)) + (SCREEN_WIDTH / 2);
				sv[2].y = ScalarToInteger(DIV(MUL(pv2[2].z, SCALAR(200)), pv2[2].y)) + (SCREEN_HEIGHT / 2);

				sv[3].x = ScalarToInteger(DIV(MUL(pv2[3].x, SCALAR(200)), pv2[3].y)) + (SCREEN_WIDTH / 2);
				sv[3].y = ScalarToInteger(DIV(MUL(pv2[3].z, SCALAR(200)), pv2[3].y)) + (SCREEN_HEIGHT / 2);

				// RenderWall
				RenderWall(&pic_bbuffer, sv[0].x, sv[1].x, sv[0].y, sv[1].y, sv[2].y, sv[3].y, 159);
			}

			// Print some console info
			sprintf(console_buffer, "px: %d py: %d pz: %d", ScalarToInteger(player.origin.x), ScalarToInteger(player.origin.y), ScalarToInteger(player.origin.z));
			Console::AddText(0, 0, console_buffer);
			sprintf(console_buffer, "sv0: %d sv1: %d", sv[0].x, sv[0].y);
			Console::AddText(0, 1, console_buffer);
			sprintf(console_buffer, "sv0: %d sv1: %d", sv[1].x, sv[1].y);
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
