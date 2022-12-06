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

typedef vec2s_t vertex_t;

typedef struct
{
	int16_t floor_height;
	int16_t ceiling_height;
	int16_t wall_start_id;
	int16_t num_walls;
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
	vec3i_t angles;		// Pitch, Yaw, Roll
	int movespeedkey;
	int anglespeedkey;
	int sector_id;		// Current sector the player is in
} player_t;

//
// Macros
//

#define VEC2S(a, b)					((vec2s_t){SCALAR((a)), SCALAR((b))})
#define VEC2I(a, b)					((vec2i_t){(a), (b)})

#define VERTEX(a, b)				(VEC2S((a), (b)))

//
// Globals
//

#define MAX_VERTICES 512
#define MAX_WALLS 512
#define MAX_SECTORS 512

vertex_t vertices[MAX_VERTICES];
wall_t walls[MAX_WALLS];
sector_t sectors[MAX_SECTORS];

player_t player;
math_t math;

//
// Rendering functions
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
	if (x1 < 0) x1 = 0;
	if (x2 < 0) x2 = 0;
	if (x1 > dst->width) x1 = dst->width;
	if (x2 > dst->width) x2 = dst->width;

	for (x = x1; x < x2; x++)
	{
		int y1 = dyb * (x - xs) / dx + yb1;
		int y2 = dyt * (x - xs) / dx + yt1;

		// clip y
		if (y1 < 0) y1 = 0;
		if (y2 < 0) y2 = 0;
		if (y1 > dst->height) y1 = dst->height;
		if (y2 > dst->height) y2 = dst->height;

		Picture::DrawVerticalLine(dst, x, y1, y2, color);
	}
}

void ClipWall(scalar_t *x1, scalar_t *y1, scalar_t *z1, scalar_t x2, scalar_t y2, scalar_t z2)
{
	scalar_t da = *y1;
	scalar_t db = y2;
	scalar_t d = da - db;
	if (d == 0) d = SCALAR(1);
	scalar_t s = DIV(da, d);
	*x1 = *x1 + MUL(s, x2 - (*x1));
	*y1 = *y1 + MUL(s, y2 - (*y1)); if (*y1 == 0) *y1 = SCALAR(1);
	*z1 = *z1 + MUL(s, z2 - (*z1));
}

void RenderSector(Picture::pic_t *dst, int sector_id, int visible_x_start, int visible_x_end)
{
	// General variables
	int i, w;

	// Current sin, cos and tan of player angle
	scalar_t cs = math.cos[player.angles.y];
	scalar_t sn = math.sin[player.angles.y];
	scalar_t tn = math.tan[player.angles.y];

	// Current sector
	sector_t sector = sectors[sector_id];

	// Parse through the walls and render them
	for (w = sector.wall_start_id; w < sector.wall_start_id + sector.num_walls; w++)
	{
		// Current wall
		wall_t wall = walls[w];

		// Current vertices
		vec3s_t v[2];			// Initial vertex values
		vec3s_t pv[4];			// Player-space coordinates
		vec2i_t sv[4];			// Perspective transformed coordinates

		// Transform the vertices into the player's view
		v[0].x = vertices[wall.vertex_0_id].x - player.origin.x;
		v[0].y = vertices[wall.vertex_0_id].y - player.origin.y;
		v[0].z = SCALAR(sector.floor_height) - player.origin.z;

		v[1].x = vertices[wall.vertex_1_id].x - player.origin.x;
		v[1].y = vertices[wall.vertex_1_id].y - player.origin.y;
		v[1].z = SCALAR(sector.floor_height) - player.origin.z;

		// Rotate the values around the player's view
		pv[0].x = MUL(v[0].x, cs) - MUL(v[0].y, sn);
		pv[0].y = MUL(v[0].y, cs) + MUL(v[0].x, sn);
		pv[0].z = v[0].z + DIV(MUL(SCALAR(player.angles.x), pv[0].y), SCALAR(32));

		pv[1].x = MUL(v[1].x, cs) - MUL(v[1].y, sn);
		pv[1].y = MUL(v[1].y, cs) + MUL(v[1].x, sn);
		pv[1].z = v[1].z + DIV(MUL(SCALAR(player.angles.x), pv[1].y), SCALAR(32));

		pv[2].x = pv[0].x;
		pv[2].y = pv[0].y;
		pv[2].z = pv[0].z + SCALAR(sector.ceiling_height);

		pv[3].x = pv[1].x;
		pv[3].y = pv[1].y;
		pv[3].z = pv[1].z + SCALAR(sector.ceiling_height);

		// Don't even bother if both points are behind the player
		if (pv[0].y <= 0 && pv[1].y <= 0) continue;

		// Clip the vertices if they're partially behind the player
		if (pv[0].y <= 0)
		{
			ClipWall(&pv[0].x, &pv[0].y, &pv[0].z, pv[1].x, pv[1].y, pv[1].z);
			ClipWall(&pv[2].x, &pv[2].y, &pv[2].z, pv[3].x, pv[3].y, pv[3].z);
		}

		if (pv[1].y <= 0)
		{
			ClipWall(&pv[1].x, &pv[1].y, &pv[1].z, pv[0].x, pv[0].y, pv[0].z);
			ClipWall(&pv[3].x, &pv[3].y, &pv[3].z, pv[2].x, pv[2].y, pv[2].z);
		}

		// Screen space vertices
		sv[0].x = ScalarToInteger(DIV(MUL(pv[0].x, SCALAR(200)), pv[0].y)) + (dst->width / 2);
		sv[0].y = ScalarToInteger(DIV(MUL(pv[0].z, SCALAR(200)), pv[0].y)) + (dst->height / 2);

		sv[1].x = ScalarToInteger(DIV(MUL(pv[1].x, SCALAR(200)), pv[1].y)) + (dst->width / 2);
		sv[1].y = ScalarToInteger(DIV(MUL(pv[1].z, SCALAR(200)), pv[1].y)) + (dst->height / 2);

		sv[2].x = ScalarToInteger(DIV(MUL(pv[2].x, SCALAR(200)), pv[2].y)) + (dst->width / 2);
		sv[2].y = ScalarToInteger(DIV(MUL(pv[2].z, SCALAR(200)), pv[2].y)) + (dst->height / 2);

		sv[3].x = ScalarToInteger(DIV(MUL(pv[3].x, SCALAR(200)), pv[3].y)) + (dst->width / 2);
		sv[3].y = ScalarToInteger(DIV(MUL(pv[3].z, SCALAR(200)), pv[3].y)) + (dst->height / 2);

		// Render the wall (filled)
		RenderWall(dst, sv[0].x, sv[1].x, sv[0].y, sv[1].y, sv[2].y, sv[3].y, wall.color);
	}
}

//
// Initialization functions
//

void InitializePlayer()
{
	player.origin.x = SCALAR(64);
	player.origin.y = SCALAR(-64);
	player.origin.z = SCALAR(32);

	player.angles.x = 0;
	player.angles.y = 0;
	player.angles.z = 0;

	player.sector_id = 0;

	player.anglespeedkey = 4;
	player.movespeedkey = 4;
}

void InitializeSectors()
{
	// Vertices
	vertices[0].x = SCALAR(-128);
	vertices[0].y = SCALAR(128);

	vertices[1].x = SCALAR(128);
	vertices[1].y = SCALAR(128);

	vertices[2].x = SCALAR(128);
	vertices[2].y = SCALAR(-128);

	vertices[3].x = SCALAR(-128);
	vertices[3].y = SCALAR(-128);

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
	walls[3].vertex_1_id = 0;
	walls[3].color = 159;

	// Sectors
	sectors[0].wall_start_id = 0;
	sectors[0].num_walls = 4;
	sectors[0].floor_height = 0;
	sectors[0].ceiling_height = 128;
}

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
		math.tan[i] = SCALAR(tan(i / 180.0f * M_PI));
	}

	// Initialize player
	InitializePlayer();
	InitializeSectors();

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
				player.angles.y -= player.anglespeedkey;
				if (player.angles.y < 0) player.angles.y += 360;
			}

			// Rotate rightwards
			if (DOS::KeyTest(KB_RTARROW))
			{
				player.angles.y += player.anglespeedkey;
				if (player.angles.y > 359) player.angles.y -= 360;
			}

			// Look upwards
			if (DOS::KeyTest(KB_UPARROW))
			{
				player.angles.x += player.anglespeedkey;
				if (player.angles.x >= 180) player.angles.x = 180;
			}

			// Look downwards
			if (DOS::KeyTest(KB_DNARROW))
			{
				player.angles.x -= player.anglespeedkey;
				if (player.angles.x <= -180) player.angles.x = -180;
			}
	
			// Set velocity
			player.velocity.x = math.sin[player.angles.y] * player.movespeedkey;
			player.velocity.y = math.cos[player.angles.y] * player.movespeedkey;
			player.velocity.z = SCALAR(1.0f) * player.movespeedkey;

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
		}

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Render a world
		{
			RenderSector(&pic_bbuffer, player.sector_id, 0, SCREEN_WIDTH);
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
