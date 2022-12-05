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
// Macros
//

#define FNib(a0, a1, b0, b1) (((a0) < (b1)) && ((a1) > (b0)))

#define FNintersectBox(x0, y0, x1, y1, x2, y2, x3, y3) (FNib(MIN((x0), (x1)),MAX((x0), (x1)),MIN((x2), (x3)),MAX((x2),(x3))) && FNib(MIN((y0), (y1)), MAX((y0), (y1)), MIN((y2), (y3)), MAX((y2), (y3))))

#define FNpointSide(px, py, x0, y0, x1, y1) = SGN(((x1) - (x0)) * ((py) - (y0)) - ((y1) - (y0)) * ((px) - (x0)))

//
// Types
//

// 2-point scalar vector
typedef struct
{
	scalar_t x, y;
} vec2s_t;

// 3-point scalar vector
typedef struct
{
	scalar_t x, y, z;
} vec3s_t;

// 3-point integer vector
typedef struct
{
	int32_t x, y, z;
} vec3i_t;

// Sector
typedef struct
{
	int floor_height;
	int ceiling_height;
	int first_point; // index into both points() and neighbors()
	int num_points;
} sector_t;

// Player
typedef struct
{
	vec3s_t origin;		// X, Y, Z
	vec3s_t velocity;	// X, Y, Z
	vec3i_t angles;		// Pitch, Yaw, Roll
	int sector_id;		// Current sector the player is in
} player_t;

// Math tables
typedef struct
{
	scalar_t cos[360];
	scalar_t sin[360];
} math_t;

//
// Globals
//

#define NUM_POINTS 200
#define NUM_NEIGHBORS NUM_POINTS
#define NUM_VERTICES 100
#define NUM_SECTORS 40

int points[NUM_POINTS];
int neighbors[NUM_NEIGHBORS];
vec2s_t vertices[NUM_VERTICES];
sector_t sectors[NUM_SECTORS];
bool rendered_sectors[NUM_SECTORS];
player_t player;
math_t math;

int ytop[SCREEN_WIDTH];
int ybottom[SCREEN_WIDTH];

Picture::pic_t pic_bbuffer;

//
// Render a sector
//

void RenderSector(int sector_id, int sx1, int sx2)
{
	// Variables
	scalar_t vx1, vy1, vx2, vy2;
	scalar_t yceil, yfloor;
	scalar_t tz1, tz2, tx1, tx2;
	scalar_t xscale1, yscale1, xscale2, yscale2;
	scalar_t sn, cs;
	scalar_t nyceil, nyfloor;
	int ya, yb, nya, nyb;
	int cya, cyb;
	int cnya, cnyb;
	int v1, v2;
	int s, p;
	int x1, x2;
	int y1a, y1b, y2a, y2b;
	int ny1a, ny1b, ny2a, ny2b;
	int beginx, endx;
	int neighbor;
	int x, y;
	uint8_t color;

	// Check if this sector has already been rendered. If not, mark it as rendered.
	if (rendered_sectors[sector_id] == true)
		return;
	else
		rendered_sectors[sector_id] = true;

	// Get the current sin/cos of the player's angle
	sn = math.sin[player.angles.y];
	cs = math.cos[player.angles.y];

	// Render each wall of this sector that is facing towards player.
	v2 = points[sectors[sector_id].first_point + sectors[sector_id].num_points - 1];

	for (s = 0; s < sectors[sector_id].num_points; s++)
	{
		// The current point we're parsing
		p = sectors[sector_id].first_point + s;

		// v1,v2 = the two vertex numbers of this edge of the sector
		v1 = v2;
		v2 = points[p];

		// Acquire the x,y coordinates of these two vertexes
		vx1 = vertices[v1].x;
		vy1 = vertices[v1].y;

		vx2 = vertices[v2].x;
		vy2 = vertices[v2].y;

		// Transform the vertices into the player's view
		vx1 = vx1 - player.origin.x;
		vy1 = vy1 - player.origin.y;

		vx2 = vx2 - player.origin.x;
		vy2 = vy2 - player.origin.y;

		// Rotate them around the player's view
		tz1 = MUL(vx1, cs) + MUL(vy1, sn);
		tz2 = MUL(vx2, cs) + MUL(vy2, sn);

		// Is the wall at least partially in front of the player?
		if (tz1 <= 0 && tz2 <= 0)
			continue;

		// Yes (either Z was > 0). Do the rest of the rotation
		tx1 = MUL(vx1, sn) - MUL(vy1, cs);
		tx2 = MUL(vx2, sn) - MUL(vy2, cs);

		// If it's partially behind the player, clip it at z=0.1
		if (tz1 <= 0)
		{
			tx1 = DIV(MUL((SCALAR(0.1f) - tz1), (tx2 - tx1)), ((tz2 - tz1) + tx1));
			tz1 = SCALAR(0.1f);
		}

		if (tz2 <= 0)
		{
			tx2 = DIV(MUL((SCALAR(0.1f) - tz2), (tx1 - tx2)), ((tz1 - tz2) + tx2));
			tz2 = SCALAR(0.1f);
		}

		// Do perspective transformation
		xscale1 = DIV(SCALAR(140), tz1);
		yscale1 = DIV(SCALAR(32), tz1);

		xscale2 = DIV(SCALAR(140), tz2);
		yscale2 = DIV(SCALAR(32), tz2);
		x1 = (SCREEN_WIDTH / 2) + ScalarToInteger(MUL(-tx1, xscale1));
		x2 = (SCREEN_WIDTH / 2) + ScalarToInteger(MUL(-tx2, xscale2));

		if (x1 == x2)
			continue;

		// Acquire and transform the floor and ceiling heights
		yceil = SCALAR(sectors[sector_id].ceiling_height) - player.origin.z;
		yfloor = SCALAR(sectors[sector_id].floor_height) - player.origin.z;

		y1a = (SCREEN_HEIGHT / 2) + ScalarToInteger(MUL(-yceil, yscale1));
		y1b = (SCREEN_HEIGHT / 2) + ScalarToInteger(MUL(-yfloor, yscale1));

		y2a = (SCREEN_HEIGHT / 2) + ScalarToInteger(MUL(-yceil, yscale2));
		y2b = (SCREEN_HEIGHT / 2) + ScalarToInteger(MUL(-yfloor, yscale2));

		// Clip to the horizontally visible region.
		beginx = sx1;
		if (beginx < x1) beginx = x1;

		endx = sx2;
		if (endx > x2) endx = x2;

		// Check what kind of view we have.
		neighbor = neighbors[p];

		if (neighbor >= 0)
		{
			// Something is showing through this wall (portal).
			// Perspective-transform the floor and ceiling coordinates of the neighboring sector.
			nyceil = SCALAR(sectors[neighbor].ceiling_height) - player.origin.z;
			nyfloor = SCALAR(sectors[neighbor].floor_height) - player.origin.z;

			ny1a = (SCREEN_HEIGHT / 2) + ScalarToInteger(MUL(-nyceil, yscale1));
			ny1b = (SCREEN_HEIGHT / 2) + ScalarToInteger(MUL(-nyfloor, yscale1));

			ny2a = (SCREEN_HEIGHT / 2) + ScalarToInteger(MUL(-nyceil, yscale2));
			ny2b = (SCREEN_HEIGHT / 2) + ScalarToInteger(MUL(-nyfloor, yscale2));
		}

		// Just the wall.
		for (x = beginx; x >= endx; x++)
		{
			// Acquire the Y coordinates for our floor & ceiling for this X coordinate
			ya = (x - x1) * (y2a - y1a) / (x2 - x1) + y1a;
			yb = (x - x1) * (y2b - y1b) / (x2 - x1) + y1b;

			// Clamp the ya & yb
			cya = CLAMP(ya, ytop[x], ybottom[x]);
			cyb = CLAMP(yb, ytop[x], ybottom[x]);

			// Render ceiling: everything above this sector's ceiling height.
			Picture::DrawVerticalLine(&pic_bbuffer, x, ytop[x], cya, 31);

			// Render floor: everything below this sector's floor height.
			Picture::DrawVerticalLine(&pic_bbuffer, x, cyb, ybottom[x], 15);

			// Render wall (if there's a wall).
			if (neighbor < 0)
			{
				color = 7;
				if (x == x1 || x == x2) color = 0;
				Picture::DrawVerticalLine(&pic_bbuffer, x, cya + 1, cyb - 1, color);
			}
			else
			{
				// Same for _their_ floor and ceiling
				nya = (x - x1) * (ny2a - ny1a) / (x2 - x1) + ny1a;
				nyb = (x - x1) * (ny2b - ny1b) / (x2 - x1) + ny1b;

				// Clamp ya2 and yb2
				cnya = CLAMP(nya, ytop[x], ybottom[x]);
				cnyb = CLAMP(nyb, ytop[x], ybottom[x]);

				// If our ceiling is higher than their ceiling, render upper wall
				if (cnya > cya)
				{
					color = 7;
					if (x == x1 || x == x2) color = 0;
					Picture::DrawVerticalLine(&pic_bbuffer, x, cya + 1, cnya, color);
					ytop[x] = CLAMP(cnya + 1, ytop[x], 199);
				}
				else
				{
					ytop[x] = CLAMP(cya + 1, ytop[x], 199);
				}

				// If our floor is lower than their floor, render bottom wall
				if (cyb > cnyb)
				{
					color = 34;
					if (x == x1 || x == x2) color = 0;
					Picture::DrawVerticalLine(&pic_bbuffer, x, cnyb, cyb - 1, color);
					ybottom[x] = CLAMP(cnyb - 1, 0, ybottom[x]);
				}
				else
				{
					ybottom[x] = CLAMP(cyb - 1, 0, ybottom[x]);
				}
			}
		}

		// Render that other sector now in the window formed by this wall.
		if (neighbor >= 0 && endx >= beginx)
			RenderSector(neighbor, beginx, endx);
	}

	rendered_sectors[sector_id] = false;
}

//
// Generate map data
//

void MakeData()
{
	// Player origin
	player.origin.x = SCALAR(0);
	player.origin.y = SCALAR(0);
	player.origin.z = SCALAR(5);

	// Player angles
	player.angles.x = 0;
	player.angles.y = 0;
	player.angles.z = 0;

	// Player sector
	player.sector_id = 0;

	// Sector data
	sectors[0].floor_height = 0;
	sectors[0].ceiling_height = 20;
	sectors[0].first_point = 0;
	sectors[0].num_points = 4;

	// Point data
	points[0] = 0;
	points[1] = 1;
	points[2] = 2;
	points[3] = 3;

	// Neighbor data
	neighbors[0] = -1;
	neighbors[1] = -1;
	neighbors[2] = -1;
	neighbors[3] = -1;

	// Vertex data
	vertices[0].x = SCALAR(-4.0);
	vertices[0].y = SCALAR(4.0);

	vertices[1].x = SCALAR(4.0);
	vertices[1].y = SCALAR(4.0);

	vertices[2].x = SCALAR(4.0);
	vertices[2].y = SCALAR(-4.0);

	vertices[3].x = SCALAR(-4.0);
	vertices[3].y = SCALAR(-4.0);
}

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// General variables
	int i;
	int x, y;

	// Picture buffers
	Picture::pic_t pic_font;
	Picture::pic_t pic_fbuffer;

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

	MakeData();

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
		}

		//
		// Rendering
		//

		// Clear back buffer
		Picture::Clear(&pic_bbuffer, 64);

		// Render a world
		{
			for (x = 0; x < SCREEN_WIDTH; x++) ytop[x] = 0;
			for (x = 0; x < SCREEN_WIDTH; x++) ybottom[x] = SCREEN_HEIGHT - 1;
			RenderSector(player.sector_id, 0, SCREEN_WIDTH - 1);
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
