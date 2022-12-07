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
	scalar_t x, y;
} vec2s_t;

typedef struct
{
	scalar_t x, y, z;
} vec3s_t;

typedef struct
{
	int32_t x, y;
} vec2i_t;

typedef struct
{
	int32_t x, y, z;
} vec3i_t;

typedef struct
{
	int32_t x, y;
	int32_t w, h;
} rect_t;

typedef vec2s_t vertex_t;

typedef struct
{
	int16_t floor_height;
	int16_t ceiling_height;
	int16_t wall_start_id;
	int16_t num_walls;
	uint8_t floor_color;
	uint8_t ceiling_color;
} sector_t;

typedef struct
{
	uint16_t vertex_0_id;
	uint16_t vertex_1_id;
	uint8_t color;
	uint8_t r, g, b;
} wall_t;

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
// Macros
//

#define VEC2S(a, b)					((vec2s_t){SCALAR((a)), SCALAR((b))})
#define VEC2I(a, b)					((vec2i_t){(a), (b)})

#define VERTEX(a, b)				(VEC2S((a), (b)))

#define RECT(x, y, w, h)			((rect_t){(x), (y), (w), (h)})

//
// Globals
//

#define MAX_VERTICES 32
#define MAX_WALLS 32
#define MAX_SECTORS 32

vertex_t vertices[MAX_VERTICES];
wall_t walls[MAX_WALLS];
sector_t sectors[MAX_SECTORS];

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
	int draw_x0 = area.x;
	int draw_y0 = area.y;
	int draw_x1 = area.x + area.w;
	int draw_y1 = area.y + area.h;

	// The positions of the pixels we'll be drawing
	int x, y;

	// Current sin, cos and tan of player's yaw
	scalar_t sn = math.sin[player.angles.y];
	scalar_t cs = math.cos[player.angles.y];
	scalar_t tn = math.tan[player.angles.y];

	// Ray sweep loop
	for (x = draw_x0; x < draw_x1; x++)
	{
		// The angle of projection
		int angle = (player.angles.y - (player.fov / 2)) + ((player.fov * x) / area.w);

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
			int line_height = ScalarToInteger(DIV(SCALAR(area.h), perp_wall_dist));

			//calculate lowest and highest pixel to fill in current stripe
			int drawStart = -line_height / 2 + area.h / 2;
			if (drawStart < 0) drawStart = 0;
			int drawEnd = line_height / 2 + area.h / 2;
			if (drawEnd >= area.h) drawEnd = area.h;

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
				scalar_t texcoord = MUL(SCALAR(drawStart - (area.h / 2) + (line_height / 2)), step);

				for (int y = drawStart; y < drawEnd; y++)
				{
					// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
					int tex_y = ScalarToInteger(texcoord) & (TEXTURE_Y - 1);
					texcoord += step;
					//uint8_t color = textures[0][TEXTURE_Y * tex_y + tex_x];
					uint8_t color = Picture::GetPixel(&pic_wall, tex_x, tex_y);

					// Lookup in colormap for brightness
					if (side == true) color = Colormap::Lookup(color, ScalarToInteger(MUL(perp_wall_dist, SCALAR(0.25f))) - 2);
					else color = Colormap::Lookup(color, ScalarToInteger(MUL(perp_wall_dist, SCALAR(0.25f))));

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

#ifdef PORTREND

//
// Sector rendering functions
//

void ClipWall(scalar_t *x1, scalar_t *y1, scalar_t *z1, scalar_t x2, scalar_t y2, scalar_t z2)
{
	scalar_t da = *y1;
	scalar_t db = y2;
	scalar_t d = da - db;
	if (ScalarToInteger(d) == 0) d = SCALAR(1);
	scalar_t s = DIV(da, d);
	*x1 = *x1 + MUL(s, x2 - (*x1));
	*y1 = *y1 + MUL(s, y2 - (*y1)); if (ScalarToInteger(*y1) == 0) *y1 = SCALAR(16);
	*z1 = *z1 + MUL(s, z2 - (*z1));
}

void RenderSector(Picture::pic_t *dst, rect_t dst_area, int sector_id)
{
	// General variables
	int i, w;

	// Current sin, cos and tan of player angle
	scalar_t cs = math.cos[player.angles.y];
	scalar_t sn = math.sin[player.angles.y];
	scalar_t tn = math.tan[player.angles.y];

	// Current sector
	sector_t sector = sectors[sector_id];

	// Draw bounds
	int draw_x0 = dst_area.x;
	int draw_y0 = dst_area.y;
	int draw_x1 = dst_area.x + dst_area.w;
	int draw_y1 = dst_area.y + dst_area.h;

	// Parse through the walls and render them
	for (w = sector.wall_start_id; w < sector.wall_start_id + sector.num_walls; w++)
	{
		// Current wall
		wall_t wall = walls[w];

		// Variables
		vec3s_t v[2];			// Initial vertex values
		vec3s_t pv[4];			// Player-space coordinates
		vec2i_t sv[4];			// Perspective transformed coordinates

		// Transform the vertices into the player's view
		v[0].x = vertices[wall.vertex_0_id].x - player.origin.x;
		v[0].y = vertices[wall.vertex_0_id].y - player.origin.y;
		v[0].z = SCALAR(sector.floor_height) + player.origin.z;

		v[1].x = vertices[wall.vertex_1_id].x - player.origin.x;
		v[1].y = vertices[wall.vertex_1_id].y - player.origin.y;
		v[1].z = SCALAR(sector.floor_height) + player.origin.z;

		// Rotate the y values around the player's view
		pv[0].y = MUL(v[0].x, sn) + MUL(v[0].y, cs);
		pv[1].y = MUL(v[1].x, sn) + MUL(v[1].y, cs);

		// Don't even bother if both y points are behind the player
		if (pv[0].y < SCALAR(1) && pv[1].y < SCALAR(1)) continue;

		// Rotate the x and z values around the player's view
		pv[0].x = MUL(v[0].x, cs) - MUL(v[0].y, sn);
		pv[0].z = v[0].z + DIV(MUL(SCALAR(player.angles.x), pv[0].y), SCALAR(32));

		pv[1].x = MUL(v[1].x, cs) - MUL(v[1].y, sn);
		pv[1].z = v[1].z + DIV(MUL(SCALAR(player.angles.x), pv[1].y), SCALAR(32));

		// Clip the vertices if they're partially behind the player
		if (pv[0].y < SCALAR(1)) ClipWall(&pv[0].x, &pv[0].y, &pv[0].z, pv[1].x, pv[1].y, pv[1].z);
		if (pv[1].y < SCALAR(1)) ClipWall(&pv[1].x, &pv[1].y, &pv[1].z, pv[0].x, pv[0].y, pv[0].z);

		// Project the top-of-wall vertices
		pv[2].x = pv[0].x;
		pv[2].y = pv[0].y;
		pv[2].z = pv[0].z - SCALAR(sector.ceiling_height);

		pv[3].x = pv[1].x;
		pv[3].y = pv[1].y;
		pv[3].z = pv[1].z - SCALAR(sector.ceiling_height);

		//
		// Screen space vertices (perspective transform)
		//

		scalar_t xscale1 = DIV(SCALAR(140), pv[0].y);
		scalar_t yscale1 = DIV(SCALAR(32), pv[0].y);

		scalar_t xscale2 = DIV(SCALAR(140), pv[1].y);
		scalar_t yscale2 = DIV(SCALAR(32), pv[1].y);

		sv[0].x = ScalarToInteger(DIV(MUL(pv[0].x, SCALAR(150)), pv[0].y)) + (dst->width / 2);
		sv[0].y = ScalarToInteger(DIV(MUL(pv[0].z, SCALAR(150)), pv[0].y)) + (dst->height / 2);

		sv[1].x = ScalarToInteger(DIV(MUL(pv[1].x, SCALAR(150)), pv[1].y)) + (dst->width / 2);
		sv[1].y = ScalarToInteger(DIV(MUL(pv[1].z, SCALAR(150)), pv[1].y)) + (dst->height / 2);

		if (sv[0].x == sv[1].x) continue;

		// the x values will always be the same, so save some cycles

		sv[2].x = sv[0].x;
		sv[2].y = ScalarToInteger(DIV(MUL(pv[2].z, SCALAR(150)), pv[2].y)) + (dst->height / 2);

		sv[3].x = sv[1].x;
		sv[3].y = ScalarToInteger(DIV(MUL(pv[3].z, SCALAR(150)), pv[3].y)) + (dst->height / 2);

		//
		// Draw the wall
		//

		// Pixel positions
		int x, y1, y2;

		// Distance deltas
		int delta_bottom_y = sv[1].y - sv[0].y;
		int delta_top_y = sv[3].y - sv[2].y;

		int delta_x = sv[1].x - sv[0].x;
		if (delta_x == 0) delta_x = 1;

		int x0 = sv[0].x;

		// Clip x values
		if (sv[0].x < draw_x0) sv[0].x = draw_x0;
		if (sv[1].x < draw_x0) sv[1].x = draw_x0;
		if (sv[0].x > draw_x1) sv[0].x = draw_x1;
		if (sv[1].x > draw_x1) sv[1].x = draw_x1;

		// Vertical line loop
		for (x = sv[0].x; x < sv[1].x; x++)
		{
			y1 = delta_bottom_y * (x - x0) / delta_x + sv[0].y;
			y2 = delta_top_y * (x - x0) / delta_x + sv[2].y;

			// Clip y values
			if (y1 < draw_y0) y1 = draw_y0;
			if (y2 < draw_y0) y2 = draw_y0;
			if (y1 > draw_y1) y1 = draw_y1;
			if (y2 > draw_y1) y2 = draw_y1;

			// Draw the wall column
			Picture::DrawVerticalLine(dst, x, y1, y2, wall.color);

			// Draw the ceiling column (lazy)
			if (player.origin.z < SCALAR(sector.ceiling_height) && y2 < draw_y1)
			{
				Picture::DrawVerticalLine(dst, x, y2, draw_y0, sector.ceiling_color);
				Picture::DrawPixel(dst, x, y2, 0); // trim
			}

			// Draw the floor column (lazy)
			if (player.origin.z > SCALAR(sector.floor_height) && y1 > draw_y0)
			{
				Picture::DrawVerticalLine(dst, x, y1, draw_y1, sector.floor_color);
				Picture::DrawPixel(dst, x, y1, 0); // trim
			}
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

	#ifdef PORTREND
	player.origin.x = SCALAR(0);
	player.origin.y = SCALAR(0);
	player.origin.z = SCALAR(64);
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

	#ifdef PORTREND
	player.velocity.x = math.sin[player.angles.y] * player.movespeedkey;
	player.velocity.y = math.cos[player.angles.y] * player.movespeedkey;
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
// Sector functions
//

void SectorsInit()
{
	// Vertices
	vertices[0].x = SCALAR(-256);
	vertices[0].y = SCALAR(0);

	vertices[1].x = SCALAR(-128);
	vertices[1].y = SCALAR(256);

	vertices[2].x = SCALAR(128);
	vertices[2].y = SCALAR(256);

	vertices[3].x = SCALAR(256);
	vertices[3].y = SCALAR(0);

	vertices[4].x = SCALAR(128);
	vertices[4].y = SCALAR(-256);

	vertices[5].x = SCALAR(-128);
	vertices[5].y = SCALAR(-256);

	vertices[6].x = SCALAR(256);
	vertices[6].y = SCALAR(512);

	vertices[7].x = SCALAR(512);
	vertices[7].y = SCALAR(128);

	// Walls
	walls[0].vertex_0_id = 0;
	walls[0].vertex_1_id = 1;
	walls[0].color = 150;

	walls[1].vertex_0_id = 1;
	walls[1].vertex_1_id = 2;
	walls[1].color = 159;

	walls[2].vertex_0_id = 2;
	walls[2].vertex_1_id = 3;
	walls[2].color = 150;

	walls[3].vertex_0_id = 3;
	walls[3].vertex_1_id = 4;
	walls[3].color = 159;

	walls[4].vertex_0_id = 4;
	walls[4].vertex_1_id = 5;
	walls[4].color = 150;

	walls[5].vertex_0_id = 5;
	walls[5].vertex_1_id = 0;
	walls[5].color = 159;

	walls[6].vertex_0_id = 2;
	walls[6].vertex_1_id = 6;
	walls[6].color = 159;

	walls[7].vertex_0_id = 6;
	walls[7].vertex_1_id = 7;
	walls[7].color = 150;

	walls[8].vertex_0_id = 7;
	walls[8].vertex_1_id = 3;
	walls[8].color = 159;

	// Sectors
	sectors[0].wall_start_id = 0;
	sectors[0].num_walls = 6;
	sectors[0].floor_height = 0;
	sectors[0].ceiling_height = 256;
	sectors[0].ceiling_color = 143;
	sectors[0].floor_color = 143;

	sectors[1].wall_start_id = 6;
	sectors[1].num_walls = 3;
	sectors[1].floor_height = 0;
	sectors[1].ceiling_height = 256;
	sectors[1].ceiling_color = 143;
	sectors[1].floor_color = 143;
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

	#ifdef PORTREND
	// Initialize sector data
	SectorsInit();
	#endif

	#ifdef RAYCASTER
	// Initialize texture data
	GenerateTextures();

	// Initialize colormap
	Colormap::Load("gfx/duke3d.tab");
	#endif

	// Initialize DOS
	DOS::Initialize();

	// Initialize VGA
	VGA::Initialize();
	VGA::SetPalette("gfx/duke3d.pal");

	// Create pictures
	Console::Initialize();
	Picture::LoadBMP(&pic_font, "gfx/font8x8.bmp");
	Picture::LoadBMP(&pic_shotgun, "gfx/shot001a.bmp");
	Picture::LoadBMP(&pic_wall, "tex_bmp/quake/city2_3.bmp");
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
		Picture::Clear(&pic_bbuffer, 64);

		#ifdef RAYCASTER
		// Raycaster rendering
		{
			RenderRays(&pic_bbuffer, RECT(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
		}
		#endif

		#ifdef PORTREND
		// Sector rendering
		{
			RenderSector(&pic_bbuffer, RECT(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), player.sector_id);
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
