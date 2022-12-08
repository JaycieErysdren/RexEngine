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
// LAST EDITED:		December 7th, 2022
//
// ========================================================

// Duke4Ever header
#include "duke4evr.hpp"

#define CYCLES 30

#define RAYCASTER

//
// Types
//

typedef struct
{
	scalar_t cos[360];
	scalar_t sin[360];
	scalar_t tan[360];
} math_t;

typedef struct
{
	vec3s_t origin;		// X, Y, Z
	vec3s_t velocity;	// X, Y, Z
	vec3i_t angles;		// Pitch, Yaw, Roll (degrees)
	int movespeedkey;	// Movement speed multiplier
	int anglespeedkey;	// Turn speed multiplier
	int sector_id;		// Current sector the player is in
	int fov;			// Field of view (degrees)
} player_t;

//
// Globals
//

player_t player;
math_t math;
char console_buffer[256];

Picture::pic_t pic_wall;
bool texturemapping = false;

//
// Raycaster globals
//

#ifdef RAYCASTER

#define MAP_X 24
#define MAP_Y 24
#define MAP_SIZE 2
#define TEXTURE_X 64
#define TEXTURE_Y 64

uint8_t map[MAP_X][MAP_Y] = {
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
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

uint8_t textures[1][TEXTURE_Y * TEXTURE_X];

//
// Raycast rendering functions
//

void RenderRays(Picture::pic_t *dst, rect_t area)
{
	// Draw bounds
	int draw_w = area.x2 - area.x1;
	int draw_h = area.y2 - area.y1;

	// The positions of the pixels we'll be drawing
	int x, y;

	// Current sin, cos and tan of player's yaw
	scalar_t sn = math.sin[player.angles.y];
	scalar_t cs = math.cos[player.angles.y];
	scalar_t tn = math.tan[player.angles.y];

	// Ray sweep loop
	for (x = area.x1; x < area.x2; x++)
	{
		// The angle of projection
		int angle = (player.angles.y - (player.fov / 2)) + ((player.fov * x) / draw_w);

		// Projection angle sanity check
		if (angle < 0) angle += 360;
		if (angle > 359) angle -= 360;

		vec2s_t raydir;
		vec2s_t raypos;
		vec2s_t delta_dist;
		vec2s_t side_dist;
		int map_x = ScalarToInteger(player.origin.x);
		int map_y = ScalarToInteger(player.origin.y);
		int step_x, step_y;
		bool hit = false, side = false;
		bool oob = false;

		raydir.x = math.sin[angle];
		raydir.y = math.cos[angle];

		delta_dist.x = (raydir.x == 0) ? SCALAR_MAX : ABS(DIV(SCALAR(1.0f), raydir.x));
		delta_dist.y = (raydir.y == 0) ? SCALAR_MAX : ABS(DIV(SCALAR(1.0f), raydir.y));

		//calculate step and initial sideDist
		if (raydir.x < 0)
		{
			step_x = -1;
			side_dist.x = MUL((player.origin.x - SCALAR(map_x)), delta_dist.x);
		}
		else
		{
			step_x = 1;
			side_dist.x = MUL((SCALAR(map_x) + SCALAR(1) - player.origin.x), delta_dist.x);
		}

		if (raydir.y < 0)
		{
			step_y = -1;
			side_dist.y = MUL((player.origin.y - SCALAR(map_y)), delta_dist.y);
		}
		else
		{
			step_y = 1;
			side_dist.y = MUL((SCALAR(map_y) + SCALAR(1) - player.origin.y), delta_dist.y);
		}

		//perform DDA
		while (hit == false && oob == false)
		{
			if (side_dist.x < side_dist.y)
			{
				side_dist.x += delta_dist.x;
				map_x += step_x;
				side = false;
			}
			else
			{
				side_dist.y += delta_dist.y;
				map_y += step_y;
				side = true;
			}

			//Check if ray has hit a wall
			if (map[map_y][map_x] > 0) hit = true;
			
			if (map_y >= MAP_Y || map_y < 0 || map_x >= MAP_X || map_x < 0) oob = true;
		}

		if (oob == true) continue;

		scalar_t perp_wall_dist;

		if (side == false) perp_wall_dist = (side_dist.x - delta_dist.x);
		else perp_wall_dist = (side_dist.y - delta_dist.y);

		if (perp_wall_dist != 0)
		{
			//Calculate height of line to draw on screen
			int line_height = ScalarToInteger(DIV(SCALAR(draw_h), perp_wall_dist));

			//calculate lowest and highest pixel to fill in current stripe
			int drawStart = -line_height / 2 + draw_h / 2;
			if (drawStart < 0) drawStart = 0;
			int drawEnd = line_height / 2 + draw_h / 2;
			if (drawEnd >= draw_h) drawEnd = draw_h;

			//texturing calculations
			if (texturemapping == true)
			{
				int tex_num = map[map_y][map_x] - 1; //1 subtracted from it so that texture 0 can be used!

				//calculate value of wallX
				scalar_t wall_x; //where exactly the wall was hit
				if (side == false) wall_x = player.origin.y + MUL(perp_wall_dist, raydir.y);
				else wall_x = player.origin.x + MUL(perp_wall_dist, raydir.x);

				//x coordinate on the texture
				int tex_x = ScalarToInteger(MUL(wall_x, SCALAR(TEXTURE_X)));
				if (side == false && raydir.x > 0) tex_x = TEXTURE_X - tex_x - 1;
				if (side == true && raydir.y < 0) tex_x = TEXTURE_X - tex_x - 1;

				// How much to increase the texture coordinate per screen pixel
				scalar_t step = DIV(MUL(SCALAR(1.0f), SCALAR(TEXTURE_X)), SCALAR(line_height));

				// Starting texture coordinate
				scalar_t texcoord = MUL(SCALAR(drawStart - (draw_h / 2) + (line_height / 2)), step);

				for (int y = drawStart; y < drawEnd; y++)
				{
					// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
					int tex_y = ScalarToInteger(texcoord) & (TEXTURE_Y - 1);
					texcoord += step;
					//uint8_t color = textures[0][TEXTURE_Y * tex_y + tex_x];
					uint8_t color = Picture::GetPixel(&pic_wall, tex_x, tex_y);

					// Lookup in colormap for brightness
					if (side == true) color = Colormap::Lookup(color, ScalarToInteger(MUL(perp_wall_dist, SCALAR(2))) - 2);
					else color = Colormap::Lookup(color, ScalarToInteger(MUL(perp_wall_dist, SCALAR(2))));

					Picture::DrawPixel(dst, x, y, color);
				}
			}
			else
			{
				//choose wall color
				uint8_t color;
				switch (map[map_y][map_x])
				{
					case 1: color = 31; break;
					case 2: color = 47; break;
					case 3: color = 63; break;
					case 4: color = 79; break;
					case 5: color = 95; break;
					default: color = 0; break;
				}

				// Lookup in colormap for brightness
				if (side == true) color = Colormap::Lookup(color, ScalarToInteger(MUL(perp_wall_dist, SCALAR(2))) - 4);
				else color = Colormap::Lookup(color, ScalarToInteger(MUL(perp_wall_dist, SCALAR(2))));

				//draw the pixels of the stripe as a vertical line
				Picture::DrawVerticalLine(dst, x, drawStart, drawEnd, color);
			}
		}
	}
}

void DrawMap(Picture::pic_t *dst, int x, int y, int cell_width, int cell_height)
{
	// Draw a map
	for (int my = 0; my < MAP_Y; my++)
	{
		for (int mx = 0; mx < MAP_X; mx++)
		{
			uint8_t color;
			switch (map[my][mx])
			{
				case 1: color = 31; break;
				case 2: color = 47; break;
				case 3: color = 63; break;
				case 4: color = 79; break;
				case 5: color = 95; break;
				default: color = 0; break;
			}

			Picture::DrawRectangle(dst, x + (mx * cell_width), y + (my * cell_height), cell_width, cell_height, color, true);
		}
	}

	// Draw the player on the map
	Picture::DrawPixel(dst, x + ScalarToInteger(MUL((player.origin.x), SCALAR(cell_width))), y + ScalarToInteger(MUL((player.origin.y), SCALAR(cell_height))), 254);
}

void GenerateTextures()
{
	// generate a texture
	for(int x = 0; x < TEXTURE_X; x++)
	{
		for(int y = 0; y < TEXTURE_Y; y++)
		{
			textures[0][TEXTURE_X * y + x] = 32 - ((y + 1) / 2);
		}
	}
}

#endif
//
// Player functions
//

void PlayerInit()
{
	#ifdef RAYCASTER
	player.origin.x = SCALAR(8.5f);
	player.origin.y = SCALAR(2.5f);
	player.origin.z = SCALAR(0.5f);
	#endif

	player.angles.x = 0;
	player.angles.y = 0;
	player.angles.z = 0;

	player.sector_id = 0;

	player.anglespeedkey = 4;

	player.fov = 90;
}

void PlayerController()
{
	// Mouse read
	static int16_t mx_prev, my_prev;
	int16_t delta_mx, delta_my;
	int16_t mb, mx, my;
	mb = DOS::MouseRead(&mx, &my);

	delta_mx = mx_prev - mx;
	delta_my = my_prev - my;

	// Mouse look
	if (mb == 1 && delta_mx != 0) player.angles.y -= delta_mx;
	if (mb == 2 && delta_my != 0) player.angles.x += delta_my;

	// Reset pitch
	if (mb == 3)
		player.angles.x = 0;

	// Keyboard look
	{
		// Rotate leftwards
		if (DOS::KeyTest(KB_LTARROW)) player.angles.y -= player.anglespeedkey;

		// Rotate rightwards
		if (DOS::KeyTest(KB_RTARROW)) player.angles.y += player.anglespeedkey;

		// Look upwards
		if (DOS::KeyTest(KB_UPARROW)) player.angles.x += player.anglespeedkey;

		// Look downwards
		if (DOS::KeyTest(KB_DNARROW)) player.angles.x -= player.anglespeedkey;
	}

	// Pitch angle sanity checks
	if (player.angles.x >= 30) player.angles.x = 30;
	if (player.angles.x <= -30) player.angles.x = -30;

	// Yaw angle sanity checks
	if (player.angles.y < 0) player.angles.y += 360;
	if (player.angles.y > 359) player.angles.y -= 360;

	// Check if sprinting
	if (DOS::KeyTest(KB_LTSHIFT))
		player.movespeedkey = 6;
	else
		player.movespeedkey = 4;

	// Set velocity
	#ifdef RAYCASTER
	player.velocity.x = MUL(math.sin[player.angles.y], SCALAR(0.1f)) * player.movespeedkey;
	player.velocity.y = MUL(math.cos[player.angles.y], SCALAR(0.1f)) * player.movespeedkey;
	player.velocity.z = SCALAR(1.0f) * player.movespeedkey;
	#endif

	// Move forwards
	if (DOS::KeyTest(KB_W))
	{
		player.origin.x += player.velocity.x;
		player.origin.y += player.velocity.y;
	}

	// Move backwards
	if (DOS::KeyTest(KB_S))
	{
		player.origin.x -= player.velocity.x;
		player.origin.y -= player.velocity.y;
	}

	// Move leftwards
	if (DOS::KeyTest(KB_A))
	{
		player.origin.x -= player.velocity.y;
		player.origin.y += player.velocity.x;
	}

	// Move rightwards
	if (DOS::KeyTest(KB_D))
	{
		player.origin.x += player.velocity.y;
		player.origin.y -= player.velocity.x;
	}

	// Move upwards
	if (DOS::KeyTest(KB_Q))
		player.origin.z += player.velocity.z;

	// Move downwards
	if (DOS::KeyTest(KB_E))
		player.origin.z -= player.velocity.z;
	
	mx_prev = mx;
	my_prev = my;
}

#ifdef PORTREND

//
// Data functions
//

// Make some world data
void SectorsInit()
{
	// Add some vertices
	Portrend::AddVertex(0, SCALAR(-256), SCALAR(0));
	Portrend::AddVertex(1, SCALAR(-128), SCALAR(256));
	Portrend::AddVertex(2, SCALAR(128), SCALAR(256));
	Portrend::AddVertex(3, SCALAR(256), SCALAR(0));
	Portrend::AddVertex(4, SCALAR(128), SCALAR(-256));
	Portrend::AddVertex(5, SCALAR(-128), SCALAR(-256));
	Portrend::AddVertex(6, SCALAR(256), SCALAR(512));
	Portrend::AddVertex(7, SCALAR(512), SCALAR(128));

	// Add some walls
	Portrend::AddWall(0, 0, 1, 150);
	Portrend::AddWall(1, 1, 2, 159);
	Portrend::AddWall(2, 2, 3, 150);
	Portrend::AddWall(3, 3, 4, 159);
	Portrend::AddWall(4, 4, 5, 150);
	Portrend::AddWall(5, 5, 0, 159);
	Portrend::AddWall(6, 2, 6, 150);
	Portrend::AddWall(7, 6, 7, 159);
	Portrend::AddWall(8, 7, 3, 150);

	// Add some sectors
	int16_t walls0[6] = {0, 1, 2, 3, 4, 5};
	Portrend::AddSector(0, 6, walls0, 0, 256, 143, 143);

	int16_t walls1[4] = {2, 6, 7, 8};
	Portrend::AddSector(1, 4, walls1, 32, 224, 63, 47);

	// Generate the portals
	Portrend::GeneratePortals();

	/* old rooms

	// Add some vertices
	AddVertex(0, SCALAR(-256), SCALAR(256));
	AddVertex(1, SCALAR(0), SCALAR(256));
	AddVertex(2, SCALAR(256), SCALAR(256));
	AddVertex(3, SCALAR(256), SCALAR(0));
	AddVertex(4, SCALAR(256), SCALAR(-256));
	AddVertex(5, SCALAR(0), SCALAR(-256));
	AddVertex(6, SCALAR(-256), SCALAR(-256));
	AddVertex(7, SCALAR(-256), SCALAR(0));
	AddVertex(8, SCALAR(0), SCALAR(0));

	// Add some walls
	AddWall(0, 0, 1, 150); // outer walls
	AddWall(1, 1, 2, 159);
	AddWall(2, 2, 3, 150);
	AddWall(3, 3, 4, 159);
	AddWall(4, 4, 5, 150);
	AddWall(5, 5, 6, 159);
	AddWall(6, 6, 7, 150);
	AddWall(7, 7, 0, 159);
	AddWall(8, 1, 8, 150); // inner walls
	AddWall(9, 8, 3, 159);
	AddWall(10, 8, 5, 150);
	AddWall(11, 8, 7, 159);

	// Add some sectors
	int16_t walls0[4] = {0, 8, 11, 7};
	AddSector(0, 4, walls0, 0, 192, 143, 138);

	int16_t walls1[4] = {1, 2, 9, 8};
	AddSector(1, 4, walls1, 0, 192, 143, 138);

	int16_t walls2[4] = {9, 3, 4, 10};
	AddSector(2, 4, walls2, 0, 192, 143, 138);

	int16_t walls3[4] = {11, 10, 5, 6};
	AddSector(3, 4, walls3, 32, 160, 63, 138);

	// Generate the portals
	GeneratePortals();

	*/
}

#endif

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// General variables
	int i;

	// Picture buffers
	Picture::pic_t pic_font;
	Picture::pic_t pic_fbuffer;
	Picture::pic_t pic_bbuffer;
	Picture::pic_t pic_shotgun;

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

	// Initialize player data
	PlayerInit();

	#ifdef RAYCASTER
	// Initialize texture data
	GenerateTextures();
	#endif

	// Initialize colormap
	Colormap::Load("gfx/duke3d.tab");

	// Initialize DOS
	DOS::Initialize();

	// Initialize VGA
	VGA::Initialize();
	VGA::SetPalette("gfx/duke3d.pal");

	// Create pictures
	Console::Initialize();
	Picture::LoadBMP(&pic_font, "gfx/font8x8.bmp");
	Picture::LoadBMP(&pic_shotgun, "gfx/shot001a.bmp");
	Picture::LoadBMP(&pic_wall, "tex_bmp/duke3d/wall001.bmp");
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

			PlayerController();
			
			// Print some player info
			sprintf(console_buffer, "x: %d y: %d z %d", ScalarToInteger(player.origin.x), ScalarToInteger(player.origin.y), ScalarToInteger(player.origin.z));
			Console::AddText(0, 0, console_buffer);
			sprintf(console_buffer, "pitch: %d yaw: %d roll %d", player.angles.x, player.angles.y, player.angles.z);
			Console::AddText(0, 1, console_buffer);
		}

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 0);

		#ifdef RAYCASTER
		// Raycaster rendering
		{
			RenderRays(&pic_bbuffer, RECT(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
		}
		#endif

		// HUD elements
		{
			//Picture::Draw8(&pic_bbuffer, &pic_shotgun, 200, 116, Picture::COLORKEY);

			#ifdef RAYCASTER
			DrawMap(&pic_bbuffer, SCREEN_WIDTH - (2 * MAP_X) - 1, 0, 2, 2);
			#endif
		}

		// Render the console text
		//Picture::DrawRectangle(&pic_bbuffer, 0, 0, 256, 16, 0, true);
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
	Picture::Destroy(&pic_wall);

	// Exit gracefully
	return EXIT_SUCCESS;
}
