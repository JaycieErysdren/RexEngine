// ========================================================
//
// FILE:			/source/portrend.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		PortalRenderer namespace implementation
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

// Rex3D header
#include "rex3d.hpp"

//
//
// ignore me plz
//
//

/* Define window size */
#define W 320
#define H 200
/* Define various vision related constants */
#define EyeHeight  6	// Camera height from floor when standing
#define DuckHeight 2.5  // And when crouching
#define HeadMargin 1	// How much room there is above camera before the head hits the ceiling
#define KneeHeight 2	// How tall obstacles the player can simply walk over without jumping
#define hfov (0.73f*H)  // Affects the horizontal field of vision
#define vfov (.2f*H)	// Affects the vertical field of vision

typedef struct
{
	float x, y;
} xy;

typedef struct
{
	float floor, ceil;
	xy *vertex; // Each vertex has an x and y coordinate
	int8_t *neighbors;		   // Each edge may have a corresponding neighboring sector
	uint32_t npoints;				 // How many vertexes there are
} sector;

/* Sectors: Floor and ceiling height; list of edge vertices and neighbors */
static sector *sectors = NULL;
static uint32_t NumSectors = 0;

/* Player: location */
static struct player
{
	struct xyz { float x,y,z; } where,	  // Current position
	velocity;   // Current motion vector
	float angle, anglesin, anglecos, yaw;   // Looking towards (and sin() and cos() thereof)
	int32_t sector;						// Which sector the player is currently in
} player;

// Utility functions. Because C doesn't have templates,
// we use the slightly less safe preprocessor macros to
// implement these functions that work with multiple types.
#define min(a,b)			 (((a) < (b)) ? (a) : (b)) // min: Choose smaller of two scalars.
#define max(a,b)			 (((a) > (b)) ? (a) : (b)) // max: Choose greater of two scalars.
#define clamp(a, mi,ma)	  min(max(a,mi),ma)		 // clamp: Clamp value into set range.
#define vxs(x0,y0, x1,y1)	((x0)*(y1) - (x1)*(y0))   // vxs: Vector cross product
// Overlap:  Determine whether the two number ranges overlap.
#define Overlap(a0,a1,b0,b1) (min(a0,a1) <= max(b0,b1) && min(b0,b1) <= max(a0,a1))
// IntersectBox: Determine whether two 2D-boxes intersect.
#define IntersectBox(x0,y0, x1,y1, x2,y2, x3,y3) (Overlap(x0,x1,x2,x3) && Overlap(y0,y1,y2,y3))
// PointSide: Determine which side of a line the point is on. Return value: <0, =0 or >0.
#define PointSide(px,py, x0,y0, x1,y1) vxs((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0))
// Intersect: Calculate the point of intersection between two lines.
#define Intersect(x1,y1, x2,y2, x3,y3, x4,y4) ((xy){ \
	vxs(vxs(x1,y1, x2,y2), (x1)-(x2), vxs(x3,y3, x4,y4), (x3)-(x4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)), \
	vxs(vxs(x1,y1, x2,y2), (y1)-(y2), vxs(x3,y3, x4,y4), (y3)-(y4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) })

/* vline: Draw a vertical line on screen, with a different color pixel in top & bottom */
void vline(const int x, int y1, int y2, const uint8_t topcolor, const uint8_t middlecolor, const uint8_t bottomcolor)
{
	//int32_t *const restrict pix = surface->pixels;
	int y;

	y1 = clamp(y1, 0, H - 1);
	y2 = clamp(y2, 0, H - 1);

	if (y2 == y1)
	{
		VGA::PlacePixel(x, y1, middlecolor);
	}
	else if (y2 > y1)
	{
		VGA::PlacePixel(x, y1, topcolor);

		for (y = y1 + 1; y < y2; y++)
			VGA::PlacePixel(x, y, topcolor);

		VGA::PlacePixel(x, y2, bottomcolor);
	}
}

//
//
// PortalRenderer namespace
//
//

// PortalRenderer namespace definition (private)
namespace PortalRenderer
{

}

//
// Rendering
//

// Render the world through portals
void PortalRenderer::Render(int screen_width, int screen_height, bool enable_textures)
{
	enum { MaxQueue = 32 };  // maximum number of pending portal renders
	struct item { int32_t sectorno,sx1,sx2; } queue[MaxQueue], *head=queue, *tail=queue;
	int32_t ytop[W]={0}, ybottom[W], renderedsectors[NumSectors];
	for(uint32_t x=0; x<W; ++x) ybottom[x] = H-1;
	for(uint32_t n=0; n<NumSectors; ++n) renderedsectors[n] = 0;

	/* Begin whole-screen rendering from where the player is. */
	*head = (struct item) { player.sector, 0, W-1 };
	if(++head == queue+MaxQueue) head = queue;

	do {
	/* Pick a sector & slice from the queue to draw */
	const struct item now = *tail;
	if(++tail == queue+MaxQueue) tail = queue;

	if(renderedsectors[now.sectorno] & 0x21) continue; // Odd = still rendering, 0x20 = give up
	++renderedsectors[now.sectorno];
	const sector *const sect = &sectors[now.sectorno];
	/* Render each wall of this sector that is facing towards player. */
	for(uint32_t s = 0; s < sect->npoints; ++s)
	{
		/* Acquire the x,y coordinates of the two endpoints (vertices) of this edge of the sector */
		const float vx1 = sect->vertex[s+0].x - player.where.x, vy1 = sect->vertex[s+0].y - player.where.y;
		const float vx2 = sect->vertex[s+1].x - player.where.x, vy2 = sect->vertex[s+1].y - player.where.y;
		/* Rotate them around the player's view */
		const float pcos = player.anglecos, psin = player.anglesin;
		float tx1 = vx1 * psin - vy1 * pcos,  tz1 = vx1 * pcos + vy1 * psin;
		float tx2 = vx2 * psin - vy2 * pcos,  tz2 = vx2 * pcos + vy2 * psin;
		/* Is the wall at least partially in front of the player? */
		if(tz1 <= 0 && tz2 <= 0) continue;
		/* If it's partially behind the player, clip it against player's view frustrum */
		if(tz1 <= 0 || tz2 <= 0)
		{
			const float nearz = 1e-4f, farz = 5, nearside = 1e-5f, farside = 20.f;
			// Find an intersection between the wall and the approximate edges of player's view
			const xy i1 = Intersect(tx1, tz1, tx2, tz2, -nearside,nearz, -farside, farz);
			const xy i2 = Intersect(tx1, tz1, tx2, tz2,  nearside,nearz,  farside, farz);
			if(tz1 < nearz) { if(i1.y > 0) { tx1 = i1.x; tz1 = i1.y; } else { tx1 = i2.x; tz1 = i2.y; } }
			if(tz2 < nearz) { if(i1.y > 0) { tx2 = i1.x; tz2 = i1.y; } else { tx2 = i2.x; tz2 = i2.y; } }
		}
		/* Do perspective transformation */
		const float xscale1 = hfov / tz1, yscale1 = vfov / tz1;	const int32_t x1 = W/2 - (int32_t)(tx1 * xscale1);
		const float xscale2 = hfov / tz2, yscale2 = vfov / tz2;	const int32_t x2 = W/2 - (int32_t)(tx2 * xscale2);
		if(x1 >= x2 || x2 < now.sx1 || x1 > now.sx2) continue; // Only render if it's visible
		/* Acquire the floor and ceiling heights, relative to where the player's view is */
		const float yceil  = sect->ceil  - player.where.z;
		const float yfloor = sect->floor - player.where.z;
		/* Check the edge type. neighbor=-1 means wall, other=boundary between two sectors. */
		const int32_t neighbor = sect->neighbors[s];
		float nyceil=0, nyfloor=0;
		if(neighbor >= 0) // Is another sector showing through this portal?
		{
			nyceil  = sectors[neighbor].ceil  - player.where.z;
			nyfloor = sectors[neighbor].floor - player.where.z;
		}
		/* Project our ceiling & floor heights into screen coordinates (Y coordinate) */
		#define Yaw(y,z) (y + z*player.yaw)
		const int32_t y1a  = H/2 - (int32_t)(Yaw(yceil, tz1) * yscale1),  y1b = H/2 - (int32_t)(Yaw(yfloor, tz1) * yscale1);
		const int32_t y2a  = H/2 - (int32_t)(Yaw(yceil, tz2) * yscale2),  y2b = H/2 - (int32_t)(Yaw(yfloor, tz2) * yscale2);
		/* The same for the neighboring sector */
		const int32_t ny1a = H/2 - (int32_t)(Yaw(nyceil, tz1) * yscale1), ny1b = H/2 - (int)(Yaw(nyfloor, tz1) * yscale1);
		const int32_t ny2a = H/2 - (int32_t)(Yaw(nyceil, tz2) * yscale2), ny2b = H/2 - (int)(Yaw(nyfloor, tz2) * yscale2);

		/* Render the wall. */
		const int32_t beginx = max(x1, now.sx1), endx = min(x2, now.sx2);
		for(int32_t x = beginx; x <= endx; ++x)
		{
			/* Calculate the Z coordinate for this point. (Only used for lighting.) */
			const int32_t z = ((x - x1) * (tz2-tz1) / (x2-x1) + tz1) * 8;
			/* Acquire the Y coordinates for our ceiling & floor for this X coordinate. Clamp them. */
			const int32_t ya = (x - x1) * (y2a-y1a) / (x2-x1) + y1a, cya = clamp(ya, ytop[x],ybottom[x]); // top
			const int32_t yb = (x - x1) * (y2b-y1b) / (x2-x1) + y1b, cyb = clamp(yb, ytop[x],ybottom[x]); // bottom
			
			/* Render ceiling: everything above this sector's ceiling height. */
			vline(x, ytop[x], cya - 1, 5, 6, 5);
			/* Render floor: everything below this sector's floor height. */
			vline(x, cyb + 1, ybottom[x], 32, 33, 32);

			/* Is there another sector behind this edge? */
			if(neighbor >= 0)
			{
				/* Same for _their_ floor and ceiling */
				const int32_t nya = (x - x1) * (ny2a-ny1a) / (x2-x1) + ny1a, cnya = clamp(nya, ytop[x],ybottom[x]);
				const int32_t nyb = (x - x1) * (ny2b-ny1b) / (x2-x1) + ny1b, cnyb = clamp(nyb, ytop[x],ybottom[x]);
				/* If our ceiling is higher than their ceiling, render upper wall */
				const uint8_t r1 = 64, r2 = 76;
				vline(x, cya, cnya-1, 0, x == x1 || x == x2 ? 0 : r1, 0); // Between our and their ceiling
				ytop[x] = clamp(max(cya, cnya), ytop[x], H-1);   // Shrink the remaining window below these ceilings
				/* If our floor is lower than their floor, render bottom wall */
				vline(x, cnyb+1, cyb, 0, x == x1 || x == x2 ? 0 : r2, 0); // Between their and our floor
				ybottom[x] = clamp(min(cyb, cnyb), 0, ybottom[x]); // Shrink the remaining window above these floors
			}
			else
			{
				/* There's no neighbor. Render wall from top (cya = ceiling level) to bottom (cyb = floor level). */
				const uint8_t r = 8;
				vline(x, cya, cyb, 0, x == x1 || x == x2 ? 0 : r, 0);
			}
		}
		/* Schedule the neighboring sector for rendering within the window formed by this wall. */
		if(neighbor >= 0 && endx >= beginx && (head+MaxQueue+1-tail)%MaxQueue)
		{
			*head = (struct item) { neighbor, beginx, endx };
			if(++head == queue+MaxQueue) head = queue;
		}
	} // for s in sector's edges
	++renderedsectors[now.sectorno];
	} while(head != tail); // render any other queued sectors
}

void PortalRenderer::LoadData()
{
	FILE *fp = fopen("mapclear.txt", "r");
	if (fp == NULL) { perror("mapclear.txt"); exit(1); }
	char Buf[256], word[256], *ptr;
	xy *vert = NULL, v;
	int32_t n, m, NumVertices = 0;
	
	while(fgets(Buf, sizeof(Buf), fp))
	{
		switch(sscanf(ptr = Buf, "%32s%n", word, &n) == 1 ? word[0] : '\0')
		{
			case 'v': // vertex
			{
				for(sscanf(ptr += n, "%f%n", &v.y, &n); sscanf(ptr += n, "%f%n", &v.x, &n) == 1; )
					{ vert = (xy *)realloc(vert, ++NumVertices * sizeof(*vert)); vert[NumVertices-1] = v; }
				break;
			}
			case 's': // sector
			{
				sectors = (sector *)realloc(sectors, ++NumSectors * sizeof(*sectors));
				sector *const sect = &sectors[NumSectors-1];
				int32_t *num = NULL;
				sscanf(ptr += n, "%f%f%n", &sect->floor,&sect->ceil, &n);
				for(m=0; sscanf(ptr += n, "%32s%n", word, &n) == 1 && word[0] != '#'; )
					{ num = (int32_t *)realloc(num, ++m * sizeof(*num)); num[m-1] = word[0]=='x' ? -1 : atoi(word); }
				sect->npoints   = m /= 2;
				sect->neighbors = (int8_t *)malloc( (m  ) * sizeof(*sect->neighbors) );
				sect->vertex	= (xy *)malloc( (m+1) * sizeof(*sect->vertex)	);
				for(n=0; n<m; ++n) sect->neighbors[n] = num[m + n];
				for(n=0; n<m; ++n) sect->vertex[n+1]  = vert[num[n]]; // TODO: Range checking
				sect->vertex[0] = sect->vertex[m]; // Ensure the vertexes form a loop
				free(num), num=NULL;
				break;
			}
			case 'p': // player
			{
				float angle;
				sscanf(ptr += n, "%f %f %f %d", &v.x, &v.y, &angle,&n);
				player = (struct player) { {v.x, v.y, 0}, {0,0,0}, angle,0,0,0, n }; // TODO: Range checking
				player.where.z = sectors[player.sector].floor + EyeHeight;
				break;
			}
		}
	}
	fclose(fp);
	free(vert), vert=NULL;
}

void PortalRenderer::UnloadData()
{
	for(uint32_t a=0; a<NumSectors; ++a) free(sectors[a].vertex), sectors[a].vertex=NULL;
	for(uint32_t a=0; a<NumSectors; ++a) free(sectors[a].neighbors), sectors[a].neighbors=NULL;
	free(sectors);
	sectors	= NULL;
	NumSectors = 0;
}
