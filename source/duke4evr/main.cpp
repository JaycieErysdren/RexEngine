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

#define MAP_X 24
#define MAP_Y 24

int8_t map[MAP_Y][MAP_X]=
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
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

typedef struct
{
	int32_t x, y, z;
} vec3i_t;

typedef struct
{
	scalar_t x, y, z;
} vec3s_t;

struct player
{
	// Scalar vectors
	vec3s_t origin; 	// x, y, z
	vec3s_t movedir;	// x, y, z
	vec3s_t plane;		// x, y, z
	vec3i_t angle;		// pitch, yaw, roll

	// Speed keys
	int movespeedkey;
	int anglespeedkey;
} player;

struct math
{
	scalar_t cos[360];
	scalar_t sin[360];
	scalar_t tan[360];
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

	// Player variables
	player.origin.x = SCALAR(22.0f);
	player.origin.y = SCALAR(12.0f);
	player.origin.z = SCALAR(0.0f);
	player.movedir.x = SCALAR(-1.0f);
	player.movedir.y = SCALAR(0.0f);
	player.movedir.z = SCALAR(0.0f);
	player.plane.x = SCALAR(1.0f);
	player.plane.y = SCALAR(0.66f);
	player.plane.z = SCALAR(0.0f);
	player.angle.x = 0;
	player.angle.y = 0;
	player.angle.z = 0;

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

			#define ROT_SPEED 0.1f
			#define MOVE_SPEED 1

			//move forward if no wall in front of you
			if(DOS::KeyTest(KB_UPARROW))
			{
				if (map[ScalarToInteger(player.origin.x + player.movedir.x * MOVE_SPEED)][ScalarToInteger(player.origin.y)] == 0)
					player.origin.x += player.movedir.x * MOVE_SPEED;

				if (map[ScalarToInteger(player.origin.x)][ScalarToInteger(player.origin.y + player.movedir.y * MOVE_SPEED)] == 0)
					player.origin.y += player.movedir.y * MOVE_SPEED;
			}

			//move backwards if no wall behind you
			if(DOS::KeyTest(KB_DNARROW))
			{
				if (map[ScalarToInteger(player.origin.x - player.movedir.x * MOVE_SPEED)][ScalarToInteger(player.origin.y)] == 0)
					player.origin.x -= player.movedir.x * MOVE_SPEED;

				if (map[ScalarToInteger(player.origin.x)][ScalarToInteger(player.origin.y - player.movedir.y * MOVE_SPEED)] == 0)
					player.origin.y -= player.movedir.y * MOVE_SPEED;
			}

			//rotate to the right
			if(DOS::KeyTest(KB_RTARROW))
			{
				//both camera direction and camera plane must be rotated
				scalar_t oldDirX = player.movedir.x;
				player.movedir.x = MUL(player.movedir.x, SCALAR(cos(-ROT_SPEED))) - MUL(player.movedir.y, SCALAR(sin(-ROT_SPEED)));
				player.movedir.y = MUL(oldDirX, SCALAR(sin(-ROT_SPEED))) + MUL(player.movedir.y, SCALAR(cos(-ROT_SPEED)));
				scalar_t oldPlaneX = player.plane.x;
				player.plane.x = MUL(player.plane.x, SCALAR(cos(-ROT_SPEED))) - MUL(player.plane.y, SCALAR(sin(-ROT_SPEED)));
				player.plane.y = MUL(oldPlaneX, SCALAR(sin(-ROT_SPEED))) + MUL(player.plane.y, SCALAR(cos(-ROT_SPEED)));
			}

			//rotate to the left
			if(DOS::KeyTest(KB_LTARROW))
			{
				//both camera direction and camera plane must be rotated
				scalar_t oldDirX = player.movedir.x;
				player.movedir.x = MUL(player.movedir.x, SCALAR(cos(ROT_SPEED))) - MUL(player.movedir.y, SCALAR(sin(ROT_SPEED)));
				player.movedir.y = MUL(oldDirX, SCALAR(sin(ROT_SPEED))) + MUL(player.movedir.y, SCALAR(cos(ROT_SPEED)));
				scalar_t oldPlaneX = player.plane.x;
				player.plane.x = MUL(player.plane.x, SCALAR(cos(ROT_SPEED))) - MUL(player.plane.y, SCALAR(sin(ROT_SPEED)));
				player.plane.y = MUL(oldPlaneX, SCALAR(sin(ROT_SPEED))) + MUL(player.plane.y, SCALAR(cos(ROT_SPEED)));
			}
		}

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Render a world
		{
			for(int x = 0; x < SCREEN_WIDTH; x++)
			{
				//calculate ray position and direction
				scalar_t cameraX = DIV(SCALAR(2 * x), SCALAR(SCREEN_WIDTH - 1)); //x-coordinate in camera space
				scalar_t rayDirX = player.movedir.x + MUL(player.plane.x, cameraX);
				scalar_t rayDirY = player.movedir.y + MUL(player.plane.y, cameraX);

				//which box of the map we're in
				int mapX = ScalarToInteger(player.origin.x);
				int mapY = ScalarToInteger(player.origin.y);

				//length of ray from current position to next x or y-side
				scalar_t sideDistX;
				scalar_t sideDistY;

				//length of ray from one x or y-side to next x or y-side
				//these are derived as:
				//deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
				//deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
				//which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
				//where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
				//unlike (dirX, dirY) is not 1, however this does not matter, only the
				//ratio between deltaDistX and deltaDistY matters, due to the way the DDA
				//stepping further below works. So the values can be computed as below.
				// Division through zero is prevented, even though technically that's not
				// needed in C++ with IEEE 754 floating point values.
				scalar_t deltaDistX = (rayDirX == 0) ? SCALAR_MIN : ABS(DIV(SCALAR(1.0f), rayDirX));
				scalar_t deltaDistY = (rayDirY == 0) ? SCALAR_MIN : ABS(DIV(SCALAR(1.0f), rayDirY));

				scalar_t perpWallDist;

				//what direction to step in x or y-direction (either +1 or -1)
				int stepX;
				int stepY;

				bool hit = false; //was there a wall hit?
				bool side; //was a NS or a EW wall hit?

				//calculate step and initial sideDist
				if (rayDirX < 0)
				{
					stepX = -1;
					sideDistX = MUL((player.origin.x - SCALAR(mapX)), deltaDistX);
				}
				else
				{
					stepX = 1;
					sideDistX = MUL((SCALAR(mapX) + SCALAR(1) - player.origin.x), deltaDistX);
				}

				if (rayDirY < 0)
				{
					stepY = -1;
					sideDistY = MUL((player.origin.y - SCALAR(mapY)), deltaDistY);
				}
				else
				{
					stepY = 1;
					sideDistY = MUL((SCALAR(mapY) + SCALAR(1) - player.origin.y), deltaDistY);
				}

				//perform DDA
				while (hit == false)
				{
					//jump to next map square, either in x-direction, or in y-direction
					if (sideDistX < sideDistY)
					{
						sideDistX += deltaDistX;
						mapX += stepX;
						side = false;
					}
					else
					{
						sideDistY += deltaDistY;
						mapY += stepY;
						side = true;
					}

					//Check if ray has hit a wall
					if(map[mapX][mapY] > 0) hit = true;
				}

				//Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
				//hit to the camera plane. Euclidean to center camera point would give fisheye effect!
				//This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
				//for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
				//because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
				//steps, but we subtract deltaDist once because one step more into the wall was taken above.
				if (side == false)
					perpWallDist = (sideDistX - deltaDistX);
				else
					perpWallDist = (sideDistY - deltaDistY);

				if (perpWallDist != 0)
				{
					//Calculate height of line to draw on screen
					int lineHeight = ScalarToInteger(DIV(SCALAR(SCREEN_HEIGHT), perpWallDist));

					//calculate lowest and highest pixel to fill in current stripe
					int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
					if (drawStart < 0) drawStart = 0;
					int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
					if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

					//choose wall color
					uint8_t color;
					switch (map[mapX][mapY])
					{
						case 1:  color = 31; break;
						case 2:  color = 47; break;
						case 3:  color = 63; break;
						case 4:  color = 79; break;
						default: color = 95; break;
					}

					//give x and y sides different brightness
					if(side == true) {color -= 4;}

					//draw the pixels of the stripe as a vertical line
					Picture::DrawVerticalLine(&pic_bbuffer, x, drawStart, drawEnd, color);
				}
				sprintf(console_buffer, "px: %d py: %d pz: %d", (player.plane.x), (player.plane.y), (player.plane.z));
				Console::AddText(0, 0, console_buffer);
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

	// Exit gracefully
	return EXIT_SUCCESS;
}
