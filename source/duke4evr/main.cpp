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
// LAST EDITED:		November 29th, 2022
//
// ========================================================

// Duke4Ever header
#include "duke4evr.hpp"

// Main entry point
int main(int argc, char *argv[])
{
	// Variables
	int i, x, y;
	int mb, mx, my;

	#ifdef UGH
	fix32_t posx, posy, posz, ang;
	int sx, sy;
	int board[64][64];
	uint8_t textures[4][64][64];
	#endif

	// Initialize DOS
	DOS::Initialize();

	// Initialize VGA
	VGA::Initialize();
	VGA::SetPalette("duke3d.pal");

	#ifdef UGH
	// Generate textures
	for (y = 0; y < 64; y++)
	{
		for (x = 0; x < 64; x++)
		{
			textures[0][x][y] = (x + y) / 2 + 0;
			textures[1][x][y] = ((x ^ y) * .875 + rand() * 64 * .125) * .5 + 64;
			textures[2][x][y] = ((x | y) * .875 + rand() * 64 * .125) * .5 + 128;
			textures[3][x][y] = (x * x + y * y) / 128 + 192;
		}
	}

	// Generate board
	for (i = 0; i < 64; i++)
	{
		board[i][0] = (4 * rand()) + 1;
		board[0][i] = (4 * rand()) + 1;
		board[i][63] = (4 * rand()) + 1;
		board[63][i] = (4 * rand()) + 1;
	}
	for (i = 0; i < 1024; i++)
	{
		board[(62 * rand()) + 1][(62 * rand()) + 1] = (4 * rand()) + 1;
	}

	// Initialize raycaster values
	posx = FIX32(x) + FIX32(.5);
	posy = FIX32(y) + FIX32(.5);
	posz = FIX32(0);
	ang = FIX32(PI * 2 * rand());

	#endif

	// Main loop
	while (!DOS::KeyTest(KB_ESC))
	{
		//
		// Input handling
		//

		// Read mouse
		mb = DOS::MouseRead(&mx, &my);

		// Clear back buffer
		VGA::Clear(79);

		#ifdef UGH

		// Floor (causes page fault?)
		VGA::DrawRectangleFilled(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 111);

		// Walls
		fix32_t cosang = COS32(ang);
		fix32_t sinang = SIN32(ang);
		fix32_t vxinc = sinang * -2 / SCREEN_WIDTH;
		fix32_t vx = cosang + sinang + vxinc * FIX32(0.5);
		fix32_t vyinc = cosang * 2 / SCREEN_WIDTH;
		fix32_t vy = sinang - cosang + vyinc * FIX32(0.5);

		for (sx = 0; sx < SCREEN_WIDTH; sx++)
		{
			int32_t xscan = FIX2INT32(posx);
			fix32_t xdir = Math::signum(vx);
			fix32_t incx = ABS(vx);

			int32_t yscan = FIX2INT32(posy);
			fix32_t ydir = Math::signum(vy);
			fix32_t incy = ABS(vy);

			fix32_t xtemp = posx - FIX32(xscan);
			if (xdir > 0) xtemp = 1 - xtemp;
			fix32_t ytemp = posy - FIX32(yscan);
			if (ydir > 0) ytemp = 1 - ytemp;
			fix32_t d = xtemp * incy - ytemp * incx;
		}

		#endif

		// Flip buffers
		VGA::Flip();
	}

	// Shutdown VGA
	VGA::Shutdown();

	// Shutdown DOS
	DOS::Shutdown();

	// Exit gracefully
	return EXIT_SUCCESS;
}
