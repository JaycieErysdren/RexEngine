// ========================================================
//
// FILE:			/source/rend/portrend.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Portrend namespace implementation
//
// LAST EDITED:		December 7th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Portrend namespace
//
//

// Portrend namespace definition (private)
namespace Portrend
{

//
//
// Text definitions & macros
//
//

//
// Engine limits
//

#define MAX_VERTICES			2048
#define MAX_WALLS				1024
#define MAX_SECTORS				512

//
// Types
//

// Sector
typedef struct
{
	int16_t floor_height;
	int16_t ceiling_height;
	int16_t num_walls;
	int16_t *walls;
	uint8_t floor_color;
	uint8_t ceiling_color;
} sector_t;

// Wall
typedef struct
{
	uint16_t vertex_0_id;
	uint16_t vertex_1_id;
	uint8_t color;
	uint8_t r, g, b;
} wall_t;

// Portal
typedef struct
{
	uint16_t sector0;
	uint16_t sector1;
} portal_t;

//
//
// Global Variables
//
//

vertex_t *vertices[MAX_VERTICES];
wall_t *walls[MAX_WALLS];
portal_t *portals[MAX_WALLS];
sector_t *sectors[MAX_SECTORS];
bool rendered_sectors[MAX_SECTORS];

//
//
// Functions
//
//

//
// Data construction functions
//

// Free vertex
void FreeVertex(int id)
{
	if (vertices[id] != NULL) free(vertices[id]);
}

// Free wall
void FreeWall(int id)
{
	if (walls[id] != NULL) free(walls[id]);
}

// Free portal
void FreePortal(int id)
{
	if (portals[id] != NULL) free(portals[id]);
}

// Free sector
void FreeSector(int id)
{
	//if (sectors[id]->walls != NULL) free(sectors[id]->walls);
	if (sectors[id] != NULL) free(sectors[id]);
}

// Add new vertex
void AddVertex(int id, scalar_t x, scalar_t y)
{
	FreeVertex(id);

	vertices[id] = (vertex_t *)calloc(1, sizeof(wall_t));
	vertices[id]->x = x;
	vertices[id]->y = y;
}

// Add new wall
void AddWall(int id, uint16_t v0, uint16_t v1, uint8_t color)
{
	FreeWall(id);

	walls[id] = (wall_t *)calloc(1, sizeof(wall_t));
	walls[id]->vertex_0_id = v0;
	walls[id]->vertex_1_id = v1;
	walls[id]->color = color;
}

// Add new portal
void AddPortal(int id, uint16_t sector0, uint16_t sector1)
{
	FreePortal(id);

	portals[id] = (portal_t *)calloc(1, sizeof(portal_t));
	portals[id]->sector0 = sector0;
	portals[id]->sector1 = sector1;
}

// Add new sector
void AddSector(int id, int16_t n_walls, int16_t *walls, int16_t f_height, int16_t c_height, uint8_t f_color, uint8_t c_color)
{
	FreeSector(id);

	sectors[id] = (sector_t *)calloc(1, sizeof(sector_t));
	sectors[id]->num_walls = n_walls;
	sectors[id]->floor_height = f_height;
	sectors[id]->ceiling_height = c_height;
	sectors[id]->ceiling_color = c_color;
	sectors[id]->floor_color = f_color;

	// Add in the wall array
	sectors[id]->walls = (int16_t *)calloc(n_walls, sizeof(int16_t));

	for (int i = 0; i < n_walls; i++)
	{
		sectors[id]->walls[i] = walls[i];
	}
}

// Cleanup world data
void SectorsShutdown()
{
	int i;

	for (i = 0; i < MAX_SECTORS; i++) FreeSector(i);
	for (i = 0; i < MAX_VERTICES; i++) FreeVertex(i);
	for (i = 0; i < MAX_WALLS; i++) FreeWall(i);
	for (i = 0; i < MAX_WALLS; i++) FreePortal(i);
}

// Generate portals from a given list of sectors and walls
void GeneratePortals()
{
	// loopers & wall id
	int i, x, w;

	// testing array (initialized to -1)
	int tester[MAX_WALLS];

	for (i = 0; i < MAX_WALLS; i++)
	{
		tester[i] = -1;
	}

	// loop through all sectors
	for (i = 0; i < MAX_SECTORS; i++)
	{
		// check if the sector exists
		if (sectors[i] != NULL && sectors[i]->walls != NULL)
		{
			// loop through all the walls in the sector
			for (x = 0; x < sectors[i]->num_walls; x++)
			{
				// get the id of this wall
				w = sectors[i]->walls[x];

				// if the wall id already has an attached sector id, create a portal
				// otherwise, add the current sector id to the array
				if (tester[w] != -1 && tester[w] != i)
				{
					AddPortal(w, i, tester[w]);
					tester[w] = -1;
				}
				else
				{
					tester[w] = i;
				}
			}
		}
	}
}

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

void RenderWall(Picture::pic_t *dst, rect_t dst_area, sector_t *sector, wall_t *wall, int x1, int x2, int yb1, int yb2, int yt1, int yt2, bool bfloor, bool bceiling, bool bwall)
{
	// Pixel positions
	int x, y1, y2;

	// Distance deltas
	int delta_bottom_y = yb2 - yb1;
	int delta_top_y = yt2 - yt1;

	int delta_x = x2 - x1;
	if (delta_x == 0) delta_x = 1;

	int x1_preclip = x1;

	// Clip x values
	if (x1 < dst_area.x1) x1 = dst_area.x1;
	if (x2 < dst_area.x1) x2 = dst_area.x1;
	if (x1 > dst_area.x2) x1 = dst_area.x2;
	if (x2 > dst_area.x2) x2 = dst_area.x2;

	// Vertical line loop
	for (x = x1; x < x2; x++)
	{
		y1 = delta_bottom_y * (x - x1_preclip) / delta_x + yb1;
		y2 = delta_top_y * (x - x1_preclip) / delta_x + yt1;

		// Clip y values
		if (y1 < dst_area.y1) y1 = dst_area.y1;
		if (y2 < dst_area.y1) y2 = dst_area.y1;
		if (y1 > dst_area.y2) y1 = dst_area.y2;
		if (y2 > dst_area.y2) y2 = dst_area.y2;

		// Draw the ceiling column (lazy)
		if (bceiling == true && player.origin.z < SCALAR(sector->ceiling_height))
			Picture::DrawVerticalLine(dst, x, y2, dst_area.y1, sector->ceiling_color);

		// Draw the floor column (lazy)
		if (bfloor == true && player.origin.z > SCALAR(sector->floor_height))
			Picture::DrawVerticalLine(dst, x, y1, dst_area.y2, sector->floor_color);

		// Draw the wall column with "toon shading"
		if (bwall == true && x == x1)
			Picture::DrawVerticalLine(dst, x, y1, y2, 0);
		else if (bwall == true)
			Picture::DrawVerticalLine(dst, x, y1, y2, wall->color);

		// Ceiling trim
		Picture::DrawPixel(dst, x, y2, 0);

		// Floor trim
		Picture::DrawPixel(dst, x, y1, 0);
	}
}

void RenderSector(Picture::pic_t *dst, rect_t dst_area, int sector_id, int sector_start_id)
{
	// General variables
	int i, w;

	// Current sin, cos and tan of player angle
	scalar_t cs = math.cos[player.angles.y];
	scalar_t sn = math.sin[player.angles.y];
	scalar_t tn = math.tan[player.angles.y];

	// Current sector
	sector_t *sector = sectors[sector_id];

	// Break out of the rendering loop when rendering sectors
	if (sector_id == sector_start_id) return;

	// If this sector has already been rendered in this frame, return
	if (rendered_sectors[sector_id] == true) return;
	else rendered_sectors[sector_id] = true;

	// Parse through the walls and render them
	for (w = 0; w < sector->num_walls; w++)
	{
		// Current wall
		wall_t *wall = walls[sector->walls[w]];

		// Current portal (if applicable)
		portal_t *portal = portals[sector->walls[w]];
		bool is_portal = false;
		int portal_sector_id;

		// Variables
		vec3s_t v[2];			// Initial vertex values
		vec3s_t pv[4];			// Player-space coordinates
		vec3s_t nv[4];			// Neighbor sector coordinates
		vec2i_t sv[4];			// Perspective transformed coordinates

		// Check for portals (start with a sanity check)
		if (portal != NULL && portal->sector0 != portal->sector1)
		{
			if (portal->sector0 == sector_id)
			{
				is_portal = true;
				portal_sector_id = portal->sector1;
			}
			else if (portal->sector1 == sector_id)
			{
				is_portal = true;
				portal_sector_id = portal->sector0;
			}
		}

		// Transform the vertices into the player's view
		v[0].x = vertices[wall->vertex_0_id]->x - player.origin.x;
		v[0].y = vertices[wall->vertex_0_id]->y - player.origin.y;
		v[0].z = -SCALAR(sector->floor_height) + player.origin.z;

		v[1].x = vertices[wall->vertex_1_id]->x - player.origin.x;
		v[1].y = vertices[wall->vertex_1_id]->y - player.origin.y;
		v[1].z = -SCALAR(sector->floor_height) + player.origin.z;

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
		pv[2].z = pv[0].z - SCALAR(sector->ceiling_height) + SCALAR(sector->floor_height);

		pv[3].x = pv[1].x;
		pv[3].y = pv[1].y;
		pv[3].z = pv[1].z - SCALAR(sector->ceiling_height) + SCALAR(sector->floor_height);

		//
		// Screen space vertices (perspective transform)
		//

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
		// Draw the wall or the neighboring sector
		//

		if (is_portal == true)
		{
			int x1, x2;

			// Clip x values
			if (sv[0].x < dst_area.x1) sv[0].x = dst_area.x1;
			if (sv[1].x < dst_area.x1) sv[1].x = dst_area.x1;
			if (sv[0].x > dst_area.x2) sv[0].x = dst_area.x2;
			if (sv[1].x > dst_area.x2) sv[1].x = dst_area.x2;

			rect_t portal_area = RECT(sv[0].x, dst_area.y1, sv[1].x, dst_area.y2);

			RenderSector(dst, portal_area, portal_sector_id, sector_id);

			// If their floor is higher than hours, draw a wall
			if (sectors[portal_sector_id]->floor_height > sector->floor_height || sectors[portal_sector_id]->ceiling_height < sector->ceiling_height)
			{
				scalar_t neighbor_z0; // bottom z 1
				scalar_t neighbor_z1; // bottom z 2
				scalar_t neighbor_z2; // top z 1
				scalar_t neighbor_z3; // top z 2
				int neighbor_sy0; // bottom y 1
				int neighbor_sy1; // bottom y 2
				int neighbor_sy2; // top y 1
				int neighbor_sy3; // top y 2

				neighbor_z0 = -SCALAR(sectors[portal_sector_id]->floor_height) + player.origin.z;
				neighbor_z1 = -SCALAR(sectors[portal_sector_id]->floor_height) + player.origin.z;

				neighbor_z0 += DIV(MUL(SCALAR(player.angles.x), pv[0].y), SCALAR(32));
				neighbor_z1 += DIV(MUL(SCALAR(player.angles.x), pv[1].y), SCALAR(32));

				neighbor_z2 = neighbor_z0 - SCALAR(sectors[portal_sector_id]->ceiling_height) + SCALAR(sectors[portal_sector_id]->floor_height);
				neighbor_z3 = neighbor_z1 - SCALAR(sectors[portal_sector_id]->ceiling_height) + SCALAR(sectors[portal_sector_id]->floor_height);

				// just do the clipping anyway, whatever

				//scalar_t dummy_x0 = pv[0].x;
				//scalar_t dummy_y0 = pv[0].y;

				//scalar_t dummy_x1 = pv[1].x;
				//scalar_t dummy_y1 = pv[1].y;

				//ClipWall(&dummy_x0, &dummy_y0, &neighbor_z0, pv[1].x, pv[1].y, neighbor_z1);
				//ClipWall(&dummy_x1, &dummy_y0, &neighbor_z1, pv[0].x, pv[0].y, neighbor_z0);

				neighbor_sy0 = ScalarToInteger(DIV(MUL(neighbor_z0, SCALAR(150)), pv[0].y)) + (dst->height / 2);
				neighbor_sy1 = ScalarToInteger(DIV(MUL(neighbor_z1, SCALAR(150)), pv[1].y)) + (dst->height / 2);

				neighbor_sy2 = ScalarToInteger(DIV(MUL(neighbor_z2, SCALAR(150)), pv[2].y)) + (dst->height / 2);
				neighbor_sy3 = ScalarToInteger(DIV(MUL(neighbor_z3, SCALAR(150)), pv[3].y)) + (dst->height / 2);

				// If our floor height is lower than theirs, draw a wall
				if (sectors[portal_sector_id]->floor_height > sector->floor_height)
					RenderWall(dst, dst_area, sector, wall, sv[0].x, sv[1].x, sv[0].y, sv[1].y, neighbor_sy0, neighbor_sy1, true, false, true);

				// If their ceiling is lower than ours, draw a wall
				if (sectors[portal_sector_id]->ceiling_height < sector->ceiling_height)
					RenderWall(dst, dst_area, sector, wall, sv[0].x, sv[1].x, neighbor_sy2, neighbor_sy3, sv[2].y, sv[3].y, false, true, true);
			}
			else
			{
				RenderWall(dst, dst_area, sector, wall, sv[0].x, sv[1].x, sv[0].y, sv[1].y, sv[2].y, sv[3].y, true, true, false);
			}
		}
		else
		{
			RenderWall(dst, dst_area, sector, wall, sv[0].x, sv[1].x, sv[0].y, sv[1].y, sv[2].y, sv[3].y, true, true, true);
		}
	}

	// Now that we're at the end of the frame, mark this sector as un-rendered
	rendered_sectors[sector_id] = false;
}

} // namespace Portrend
