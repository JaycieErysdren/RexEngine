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
// LAST EDITED:		December 16th, 2022
//
// ========================================================

// Duke4Ever header
#include "duke4evr.hpp"

#define CYCLES 30

//
// Types
//

typedef struct
{
	rex_scalar cos[360];
	rex_scalar sin[360];
	rex_scalar tan[360];
} math_t;

typedef struct
{
	rex_vec3s origin;		// X, Y, Z
	rex_vec3s velocity;	// X, Y, Z
	rex_vec3i angles;		// Pitch, Yaw, Roll (degrees)
	rex_int32 movespeedkey;	// Movement speed multiplier
	rex_int32 anglespeedkey;	// Turn speed multiplier
	rex_int32 sector_id;		// Current sector the player is in
	rex_int32 fov;			// Field of view (degrees)
} player_t;

//
// Globals
//

player_t player;
math_t math;
char console_buffer[256];

Rex::Surface pic_wall;
bool texturemapping = false;

//
// Raycaster globals
//

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

void RenderRays(Rex::Surface *dst, rex_rect area)
{
	// Draw bounds
	rex_int32 draw_w = area.x2 - area.x1;
	rex_int32 draw_h = area.y2 - area.y1;

	// The positions of the pixels we'll be drawing
	rex_int32 x;

	// Current sin, cos and tan of player's yaw
	rex_scalar sn = math.sin[player.angles.y];
	rex_scalar cs = math.cos[player.angles.y];

	// Ray sweep loop
	for (x = area.x1; x < area.x2; x++)
	{
		rex_vec2s raydir;
		rex_vec2s delta_dist;
		rex_vec2s side_dist;
		rex_int32 map_x = RexScalarToInteger(player.origin.x);
		rex_int32 map_y = RexScalarToInteger(player.origin.y);
		rex_int32 step_x, step_y;
		bool hit = false, side = false;
		bool oob = false;

		// calculate ray direction
		raydir.x = REX_MUL(REX_DIV(REX_SCALAR(2.0f), REX_SCALAR(draw_w)), REX_SCALAR(x)) - REX_SCALAR(1.0f);
		raydir.y = REX_SCALAR(1.0f);

		// rotate around 0,0 by player.angles.y
		rex_vec2s temp = raydir;

		raydir.x = REX_MUL(-temp.x, cs) - REX_MUL(-temp.y, sn);
		raydir.y = REX_MUL(temp.x, sn) + REX_MUL(temp.y, cs);

		// prevent div by 0
		delta_dist.x = (raydir.x == 0) ? REX_SCALAR_MAX : ABS(REX_DIV(REX_SCALAR(1.0f), raydir.x));
		delta_dist.y = (raydir.y == 0) ? REX_SCALAR_MAX : ABS(REX_DIV(REX_SCALAR(1.0f), raydir.y));

		// calculate step and initial side_dist
		if (raydir.x < 0)
		{
			step_x = -1;
			side_dist.x = REX_MUL((player.origin.x - REX_SCALAR(map_x)), delta_dist.x);
		}
		else
		{
			step_x = 1;
			side_dist.x = REX_MUL((REX_SCALAR(map_x) + REX_SCALAR(1) - player.origin.x), delta_dist.x);
		}

		if (raydir.y < 0)
		{
			step_y = -1;
			side_dist.y = REX_MUL((player.origin.y - REX_SCALAR(map_y)), delta_dist.y);
		}
		else
		{
			step_y = 1;
			side_dist.y = REX_MUL((REX_SCALAR(map_y) + REX_SCALAR(1) - player.origin.y), delta_dist.y);
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

		rex_scalar perp_wall_dist;

		if (side == false) perp_wall_dist = (side_dist.x - delta_dist.x);
		else perp_wall_dist = (side_dist.y - delta_dist.y);

		if (perp_wall_dist != 0)
		{
			//Calculate height of line to draw on screen
			rex_int32 line_height = RexScalarToInteger(REX_DIV(REX_SCALAR(draw_h), perp_wall_dist));

			//calculate lowest and highest pixel to fill in current stripe
			rex_int32 drawStart = -line_height / 2 + draw_h / 2;
			if (drawStart < 0) drawStart = 0;
			rex_int32 drawEnd = line_height / 2 + draw_h / 2;
			if (drawEnd >= draw_h) drawEnd = draw_h;

			//texturing calculations
			if (texturemapping == true)
			{
				rex_int32 tex_num = map[map_y][map_x] - 1; //1 subtracted from it so that texture 0 can be used!

				//calculate value of wallX
				rex_scalar wall_x; //where exactly the wall was hit
				if (side == false) wall_x = player.origin.y + REX_MUL(perp_wall_dist, raydir.y);
				else wall_x = player.origin.x + REX_MUL(perp_wall_dist, raydir.x);

				wall_x -= REX_FLOOR(wall_x);

				//x coordinate on the texture
				rex_int32 tex_x = RexScalarToInteger(REX_MUL(wall_x, REX_SCALAR(TEXTURE_X)));
				if (side == false && raydir.x > 0) tex_x = TEXTURE_X - tex_x - 1;
				if (side == true && raydir.y < 0) tex_x = TEXTURE_X - tex_x - 1;

				// How much to increase the texture coordinate per screen pixel
				rex_scalar step = REX_SAFEDIV(REX_MUL(REX_SCALAR(1.0f), REX_SCALAR(TEXTURE_X)), REX_SCALAR(line_height));

				// Starting texture coordinate
				rex_scalar texcoord = REX_MUL(REX_SCALAR(drawStart - (draw_h / 2) + (line_height / 2)), step);

				// it turns out this isn't really any faster...
				//Rex::SurfaceBlit8(dst, x, drawStart, x + 1, drawEnd, &pic_wall, tex_x, 0, 1, pic_wall.height, Rex::SurfaceCOPY);

				for (rex_int32 y = drawStart; y < drawEnd; y++)
				{
					// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
					rex_int32 tex_y = RexScalarToInteger(texcoord) & (TEXTURE_Y - 1);
					texcoord += step;
					uint8_t color = Rex::SurfaceGetPixel(&pic_wall, tex_x, tex_y);

					// Lookup in colormap for brightness
					if (side == true) color = Rex::ColormapLookup(color, RexScalarToInteger(REX_MUL(perp_wall_dist, REX_SCALAR(2))) - 2);
					else color = Rex::ColormapLookup(color, RexScalarToInteger(REX_MUL(perp_wall_dist, REX_SCALAR(2))));

					Rex::SurfaceDrawPixel(dst, x, y, color);
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
				if (side == true) color = Rex::ColormapLookup(color, RexScalarToInteger(REX_MUL(perp_wall_dist, REX_SCALAR(2))) - 4);
				else color = Rex::ColormapLookup(color, RexScalarToInteger(REX_MUL(perp_wall_dist, REX_SCALAR(2))));

				//draw the pixels of the stripe as a vertical line
				Rex::SurfaceDrawVerticalLine(dst, x, drawStart, drawEnd, color);
			}
		}
	}
}

void DrawMap(Rex::Surface *dst, rex_int32 x, rex_int32 y, rex_int32 cell_width, rex_int32 cell_height)
{
	// Draw a map
	for (rex_int32 my = 0; my < MAP_Y; my++)
	{
		for (rex_int32 mx = 0; mx < MAP_X; mx++)
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

			Rex::SurfaceDrawRectangle(dst, x + (mx * cell_width), y + (my * cell_height), cell_width, cell_height, color, true);
		}
	}

	// Draw the player on the map
	Rex::SurfaceDrawPixel(dst, x + RexScalarToInteger(REX_MUL((player.origin.x), REX_SCALAR(cell_width))), y + RexScalarToInteger(REX_MUL((player.origin.y), REX_SCALAR(cell_height))), 254);
}

void GenerateTextures()
{
	// generate a texture
	for(rex_int32 x = 0; x < TEXTURE_X; x++)
	{
		for(rex_int32 y = 0; y < TEXTURE_Y; y++)
		{
			textures[0][TEXTURE_X * y + x] = 32 - ((y + 1) / 2);
		}
	}
}

//
// Player functions
//

void PlayerInit()
{
	player.origin.x = REX_SCALAR(8.5f);
	player.origin.y = REX_SCALAR(2.5f);
	player.origin.z = REX_SCALAR(0.5f);

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
	static rex_int32 mx_prev, my_prev;
	rex_int32 delta_mx, delta_my;
	rex_int32 mb, mx, my;
	Rex::MouseRead(&mb, &mx, &my);

	delta_mx = mx_prev - mx;
	delta_my = my_prev - my;

	// Mouse look
	if (mb == 1 && delta_mx != 0) player.angles.y += delta_mx;
	if (mb == 2 && delta_my != 0) player.angles.x += delta_my;

	// Reset pitch
	if (mb == 3)
		player.angles.x = 0;

	// Keyboard look
	{
		// Rotate leftwards
		if (Rex::KeyTest(REX_KB_LEFT)) player.angles.y += player.anglespeedkey;

		// Rotate rightwards
		if (Rex::KeyTest(REX_KB_RIGHT)) player.angles.y -= player.anglespeedkey;

		// Look upwards
		if (Rex::KeyTest(REX_KB_UP)) player.angles.x += player.anglespeedkey;

		// Look downwards
		if (Rex::KeyTest(REX_KB_DOWN)) player.angles.x -= player.anglespeedkey;
	}

	// Pitch angle sanity checks
	if (player.angles.x >= 30) player.angles.x = 30;
	if (player.angles.x <= -30) player.angles.x = -30;

	// Yaw angle sanity checks
	if (player.angles.y < 0) player.angles.y += 360;
	if (player.angles.y > 359) player.angles.y -= 360;

	// Check if sprinting
	if (Rex::KeyTest(REX_KB_LSHIFT))
		player.movespeedkey = 6;
	else
		player.movespeedkey = 4;

	// Set velocity
	player.velocity.x = REX_MUL(math.sin[player.angles.y], REX_SCALAR(0.1f)) * player.movespeedkey;
	player.velocity.y = REX_MUL(math.cos[player.angles.y], REX_SCALAR(0.1f)) * player.movespeedkey;
	player.velocity.z = REX_SCALAR(1.0f) * player.movespeedkey;

	// Move forwards
	if (Rex::KeyTest(REX_KB_W))
	{
		player.origin.x += player.velocity.x;
		player.origin.y += player.velocity.y;
	}

	// Move backwards
	if (Rex::KeyTest(REX_KB_S))
	{
		player.origin.x -= player.velocity.x;
		player.origin.y -= player.velocity.y;
	}

	// Move leftwards
	if (Rex::KeyTest(REX_KB_A))
	{
		player.origin.x += player.velocity.y;
		player.origin.y -= player.velocity.x;
	}

	// Move rightwards
	if (Rex::KeyTest(REX_KB_D))
	{
		player.origin.x -= player.velocity.y;
		player.origin.y += player.velocity.x;
	}

	// Move upwards
	if (Rex::KeyTest(REX_KB_Q))
		player.origin.z += player.velocity.z;

	// Move downwards
	if (Rex::KeyTest(REX_KB_E))
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
	Portrend::AddVertex(0, REX_SCALAR(-256), REX_SCALAR(0));
	Portrend::AddVertex(1, REX_SCALAR(-128), REX_SCALAR(256));
	Portrend::AddVertex(2, REX_SCALAR(128), REX_SCALAR(256));
	Portrend::AddVertex(3, REX_SCALAR(256), REX_SCALAR(0));
	Portrend::AddVertex(4, REX_SCALAR(128), REX_SCALAR(-256));
	Portrend::AddVertex(5, REX_SCALAR(-128), REX_SCALAR(-256));
	Portrend::AddVertex(6, REX_SCALAR(256), REX_SCALAR(512));
	Portrend::AddVertex(7, REX_SCALAR(512), REX_SCALAR(128));

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
	AddVertex(0, REX_SCALAR(-256), REX_SCALAR(256));
	AddVertex(1, REX_SCALAR(0), REX_SCALAR(256));
	AddVertex(2, REX_SCALAR(256), REX_SCALAR(256));
	AddVertex(3, REX_SCALAR(256), REX_SCALAR(0));
	AddVertex(4, REX_SCALAR(256), REX_SCALAR(-256));
	AddVertex(5, REX_SCALAR(0), REX_SCALAR(-256));
	AddVertex(6, REX_SCALAR(-256), REX_SCALAR(-256));
	AddVertex(7, REX_SCALAR(-256), REX_SCALAR(0));
	AddVertex(8, REX_SCALAR(0), REX_SCALAR(0));

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
	rex_int32 i;

	// Picture buffers
	Rex::Surface pic_font;
	Rex::Surface pic_bbuffer;
	//Rex::Surface pic_gun;

	// Cycles
	int64_t frame_start, frame_end;
	rex_int32 cycles, c;

	// Generate math table
	for (i = 0; i < 360; i++)
	{
		math.sin[i] = REX_SCALAR(sin(i / 180.0f * PI));
		math.cos[i] = REX_SCALAR(cos(i / 180.0f * PI));
		math.tan[i] = REX_SCALAR(tan(i / 180.0f * PI));
	}

	// Initialize player data
	PlayerInit();

	// Initialize texture data
	GenerateTextures();

	// Initialize Rex Engine
	Rex::Initialize();

	// Initialize Graphics
	if (Rex::InitializeGraphics(320, 200, 8) == false) return EXIT_FAILURE;
	Rex::SetGraphicsPalette("gfx/duke3d.pal");
	Rex::VidInfo vidinfo = Rex::GetVidInfo();

	// Initialize colormap
	Rex::ColormapLoad("gfx/duke3d.tab");

	// Create pictures
	Rex::SurfaceLoadBMP(&pic_font, "gfx/font8x8.bmp");
	//Rex::SurfaceLoadBMP(&pic_gun, "gfx/gun.bmp");
	Rex::SurfaceLoadBMP(&pic_wall, "tex_bmp/duke3d/wall001.bmp");
	Rex::SurfaceCreate(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);

	// Start counting time
	frame_end = Rex::GetTicks64();

	// Main loop
	while (!Rex::KeyTest(REX_KB_ESCAPE))
	{
		// Get start of frame time
		frame_start = Rex::GetTicks64();

		#if (REX_COMPILER == COMPILER_DJGPP)
			cycles = CYCLES * (frame_start - frame_end) / UCLOCKS_PER_SEC;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			cycles = CYCLES * (frame_start - frame_end) / CLOCKS_PER_SEC;
		#endif

		// Cycles
		for (c = 0; c < cycles; c++)
		{
			//
			// Input handling
			//

			PlayerController();

			// Prrex_int32 some player info
			sprintf(console_buffer, "x: %d y: %d z %d", RexScalarToInteger(player.origin.x), RexScalarToInteger(player.origin.y), RexScalarToInteger(player.origin.z));
			Rex::ConsoleAddText(0, 0, console_buffer);
			sprintf(console_buffer, "pitch: %d yaw: %d roll %d", player.angles.x, player.angles.y, player.angles.z);
			Rex::ConsoleAddText(0, 1, console_buffer);
		}

		//
		// Rendering
		//

		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 0);

		// Raycaster rendering
		{
			// watcom...
			rex_rect screen_area = {0, 0, vidinfo.width, vidinfo.height};
			RenderRays(&pic_bbuffer, screen_area);
		}

		// HUD elements
		{
			//Rex::SurfaceDraw8(&pic_bbuffer, &pic_gun, vidinfo.width / 2 + 64, vidinfo.height - 64, Rex::SurfaceCOLORKEY);

			DrawMap(&pic_bbuffer, vidinfo.width - (2 * MAP_X) - 1, 0, 2, 2);
		}

		// Render the console text
		//Rex::SurfaceDrawRectangle(&pic_bbuffer, 0, 0, 256, 16, 0, true);
		Rex::ConsoleRender(&pic_bbuffer, &pic_font, 8);

		// Flip the rendering buffers
		Rex::SurfaceToFrontBuffer(&pic_bbuffer);

		// Get end of frame time
		#if (REX_COMPILER == COMPILER_DJGPP)
			frame_end = frame_end + cycles * UCLOCKS_PER_SEC / CYCLES;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			frame_end = frame_end + cycles * CLOCKS_PER_SEC / CYCLES;
		#endif
	}

	// Shutdown Graphics
	Rex::ShutdownGraphics();

	// Shutdown Rex
	Rex::Shutdown();

	// Cleanup memory
	Rex::SurfaceDestroy(&pic_font);
	Rex::SurfaceDestroy(&pic_bbuffer);
	//Rex::SurfaceDestroy(&pic_gun);
	Rex::SurfaceDestroy(&pic_wall);

	// Exit gracefully
	return EXIT_SUCCESS;
}
