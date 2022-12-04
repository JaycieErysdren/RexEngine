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
// LAST EDITED:		December 3rd, 2022
//
// ========================================================

// Duke4Ever header
#include "duke4evr.hpp"

#define CYCLES 30

void ClipBehindPlayer(int *x1, int *y1, int *z1, int x2, int y2, int z2)
{
	int da = *y1;
	int db = y2;
	int d = da - db;
	if (d == 0) d = 1;
	int s = da / (da - db);

	*x1 = (*x1) + s * (x2 - (*x1));
	*y1 = (*y1) + s * (y2 - (*y1));
	*z1 = (*z1) + s * (z2 - (*z1));

	// present divide by zero
	if (*y1 == 0) *y1 = 1;
}

void DrawWall(Picture::pic_t *dst, int x1, int x2, int b1, int b2, int t1, int t2, uint8_t color)
{
	int x, y;
	int dyb = b2 - b1;
	int dyt = t2 - t1;
	int dx = x2 - x1;
	if (dx == 0) dx = 1;
	int xs = x1;

	for (x = x1; x < x2; x++)
	{
		int y1 = dyb * (x - xs) / dx + b1;
		int y2 = dyt * (x - xs) / dx + t1;

		for (y = y1; y < y2; y++)
		{
			Picture::DrawPixel(dst, x, y, color);
		}
	}
}

typedef struct
{
	int x, y, z;
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
	}

	// Set player start pos
	player.origin.x = SCALAR(70.0f);
	player.origin.y = SCALAR(-110.0f);
	player.origin.z = SCALAR(20.0f);
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

		// Render world as a psuedo-3D projection
		{
			int32_t wx[4] = {0, 0, 0, 0};
			int32_t wy[4] = {0, 0, 0, 0};
			int32_t wz[4] = {0, 0, 0, 0};

			scalar_t cs = math.cos[player.angle.y];
			scalar_t sn = math.sin[player.angle.y];

			scalar_t x1 = SCALAR(40.0f) - player.origin.x;
			scalar_t y1 = SCALAR(10.0f) - player.origin.y;

			scalar_t x2 = SCALAR(40.0f) - player.origin.x;
			scalar_t y2 = SCALAR(290.0f) - player.origin.y;

			// World X
			wx[0] = ScalarToInteger(ScalarMul(x1, cs) - ScalarMul(y1, sn));
			wx[1] = ScalarToInteger(ScalarMul(x2, cs) - ScalarMul(y2, sn));

			// World Y
			wy[0] = ScalarToInteger(ScalarMul(y1, cs) + ScalarMul(x1, sn)) + 1;
			wy[1] = ScalarToInteger(ScalarMul(y2, cs) + ScalarMul(x2, sn)) + 1;

			// World Z
			wz[0] = ScalarToInteger(0 - player.origin.z);
			wz[1] = ScalarToInteger(0 - player.origin.z);

			// Screen X
			wx[0] = wx[0] * SCREEN_HEIGHT / wy[0] + (SCREEN_WIDTH / 2);
			wx[1] = wx[1] * SCREEN_HEIGHT / wy[1] + (SCREEN_WIDTH / 2);

			// Screen Y
			wy[0] = wz[0] * SCREEN_HEIGHT / wy[0] + (SCREEN_HEIGHT / 2);
			wy[1] = wz[1] * SCREEN_HEIGHT / wy[1] + (SCREEN_HEIGHT / 2);

			sprintf(console_buffer, "wx0: %d wy0: %d wz0: %d", wx[0], wy[0], wz[0]);
			Console::AddText(0, 4, console_buffer);
			sprintf(console_buffer, "wx1: %d wy1: %d wz1: %d", wx[1], wy[1], wz[1]);
			Console::AddText(0, 5, console_buffer);

			Picture::DrawLine(&pic_bbuffer, wx[0], wy[0], wx[1], wy[1], 159);
		}

		// Console text
		sprintf(console_buffer, "x: %d y: %d z: %d", ScalarToInteger(player.origin.x), ScalarToInteger(player.origin.y), ScalarToInteger(player.origin.z));
		Console::AddText(0, 0, console_buffer);
		sprintf(console_buffer, "pitch: %d yaw: %d roll: %d", player.angle.x, player.angle.y, player.angle.z);
		Console::AddText(0, 1, console_buffer);
		sprintf(console_buffer, "mdx: %d mdy: %d mdz: %d", ScalarToInteger(player.movedir.x), ScalarToInteger(player.movedir.y), ScalarToInteger(player.movedir.z));
		Console::AddText(0, 2, console_buffer);

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
