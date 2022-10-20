// ========================================================
//
// FILE:			/source/testgame/testgame.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		TestGame program entry point.
//
// LAST EDITED:		October 20th, 2022
//
// ========================================================

// Include testgame header
#include "testgame.h"

// Main window
rex_window_external *window_main;

// Project defines
#define NUM_WALLS 4

#define WALL_X 64
#define WALL_Y 64

#define BOARD_X 64
#define BOARD_Y 64

// Walls
rex_rgb walls[NUM_WALLS][WALL_X][WALL_Y];

// Board
rex_ubyte board[BOARD_X][BOARD_Y];

// Main function
void main(int argc, char *argv[])
{
	// Define variables
	rex_int x, y, z, i;

	// Startup engine
	Rex_Startup();

	// Add main window
	window_main = Rex_WindowExternal_Add(
		"TestGame",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		rex_desktop_size[0] / 2, rex_desktop_size[1] / 2,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	// Setup GL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	// Set doublebuffer swap interval
	SDL_GL_SetSwapInterval(1);

	// Generate interesting walls
	for (y = 0; y < WALL_Y; y++)
	{
		for (x = 0; x < WALL_X; x++)
		{
			i = 1;
			for (z = 0; z < NUM_WALLS; z++)
			{
				walls[z][x][y] = REX_RGB(x * i, y * i + 1, (x + y) * i + 2);
				i++;
			}
		}
	}

	// Generate random board
	for(i = 0; i < BOARD_X; i++)
	{
		board[0][i] = Rex_Random(0, NUM_WALLS);
		board[BOARD_X - 1][i] = Rex_Random(0, NUM_WALLS);

		board[i][0] = Rex_Random(0, NUM_WALLS);
		board[i][BOARD_Y - 1] = Rex_Random(0, NUM_WALLS);
	}

	for (i = 0; i < 1024; i++)
	{
		board[Rex_Random(0, BOARD_X - 2)][Rex_Random(0, BOARD_Y - 2)] = Rex_Random(0, NUM_WALLS);
	}

	while (board[y][x])
	{
		x = Rex_Random(0, BOARD_X - 2);
		y = Rex_Random(0, BOARD_X - 2);
		Rex_Message("hello");
	}

	rex_vector3f pos;
	rex_float ang;

	rex_float hx, hy;
	rex_int bx, by;
	rex_int sy1, sy2, byi;

	pos[0] = BOARD_X / 2 + 0.5;
	pos[1] = BOARD_Y / 2 + 0.5;
	pos[2] = 0;

	ang = 0;

	// Main loop
	while (REX_TRUE)
	{
		//
		// Logic
		//

		// Update window resolution
		SDL_GetWindowSize(window_main->window, &window_main->width, &window_main->height);

		// Update display angles
		rex_float ang_cos = SDL_cosf(ang);
		rex_float ang_sin = SDL_sinf(ang);

		rex_float vxinc = ang_sin * -2 / window_main->width;
		rex_float vx = ang_cos + ang_sin + vxinc / 2;

		rex_float vyinc = ang_cos * 2 / window_main->width;
		rex_float vy = ang_sin - ang_cos + vyinc / 2;

		rex_int hval = window_main->width / 2;
		rex_int sx, sy;

		for(rex_int sx = 0; sx < window_main->width; sx++)
		{
			//Raytrace in 2D to see what block is hit, and where it gets hit
			rex_int xscan = (rex_int)pos[0];
			rex_int xdir = sgn(vx);
			rex_float incx = fabsf(vx);

			rex_int yscan = (rex_int)pos[1];
			rex_int ydir = sgn(vy);
			rex_float incy = fabsf(vy);

			rex_float xtemp = pos[0] - xscan;
			if (xdir > 0) xtemp = 1 - xtemp;

			rex_float ytemp = pos[1] - yscan;
			if (ydir > 0) ytemp = 1 - ytemp;

			rex_float d = xtemp * incy - ytemp * incx;

			while (REX_TRUE)
			{
				if (d < 0)
				{
					xscan += xdir;

					if (board[yscan][xscan])
					{
						hx = xscan + (xdir < 0);
						hy = pos[1] + vy * (hx - pos[0]) / vx;
						bx = (rex_int)((hy - hy) * 64);
						if (xdir < 0) bx = 63-bx;
						break;
					}

					d += incy;
				}
				else
				{
					yscan += ydir;

					if (board[yscan][xscan])
					{
						hy = yscan + (ydir < 0);
						hx = pos[0] + vx * (hy - pos[1]) / vy;
						bx = (rex_int)((hx - hx) * 64);
						if (ydir > 0) bx = 63-bx;
						break;
					}

					d -= incx;
				}
			}

			rex_float dist = ang_cos * (hx - pos[0]) + ang_sin * (hy-pos[1]);
			
			//Find ceiling & floor borders, and texture mapping equation
			if (dist > 1 / 64)
			{
				sy1 = max(window_main->height / 2 + (-pos[2] - .5)*hval / dist, 0);
				sy2 = min(window_main->height / 2 + (-pos[2] + .5)*hval / dist, window_main->height);
				byi = (rex_int)(dist * 65536 * 64 / hval);
				by = (sy1 + 1 - window_main->height / 2) * byi + (pos[2] + .5) * 65536 * 64 - 1;
				by += ((board[yscan][xscan] - 1) * 4096 + bx * 64) * 65536;
			}
			else
			{
				sy1 = window_main->height / 2;
				sy2 = sy1;
			}
	
			vx += vxinc;
			vy += vyinc;
		}

		//
		// Rendering
		//

		// Clear the screen
		Rex_ExternalWindow_ClearGL(REX_RGBA_BLK, 255);

		// Swap GL buffer
		Rex_ExternalWindow_SwapBuffer(window_main);

		//
		// User I/O
		//

		// Read device states
		Rex_IO_ReadDevices();

		// Handle keyboard input
		if (KEY_DOWN(KEY_Q))
			Rex_Shutdown();

	}

	// free windows
	free(window_main);

	// Shutdown engine
	Rex_Shutdown();
}
