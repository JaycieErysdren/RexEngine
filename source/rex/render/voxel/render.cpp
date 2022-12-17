// ========================================================
//
// FILE:			/source/rex/modules/voxel/render.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Voxel namespace: Render implementation
//
// LAST EDITED:		December 16th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// Voxel renderer header
#include "voxel.hpp"

//
//
// Voxel namespace: Render
//
//

namespace Voxel
{

//
//
// Global variables
//
//

rex_uint8 *zbuffer;
Rex::MathTable *mathtable;

rex_int draw_w;
rex_int draw_h;

//
//
// Functions
//
//

// Initialize renderer
void Initialize(rex_int render_width, rex_int render_height)
{
	mathtable = new Rex::MathTable;

	SetRenderDimensions(render_width, render_height);
}

// Shutdown renderer
void Shutdown()
{
	if (mathtable) delete mathtable;
	if (zbuffer) delete zbuffer;
}

// Render an image to the specified surface
void Render(Rex::Surface *dst, Rex::Camera camera, World *world, rex_scalar pixel_height_scale)
{
	// General variables
	rex_int i;

	// Sin and cos of the camera's yaw
	rex_scalar sn = mathtable->sin[camera.angles.y];
	rex_scalar cs = mathtable->cos[camera.angles.y];

	// Screen coords
	rex_vec2i s;

	// meh
	rex_vec3s p = camera.origin;

	// meh
	rex_int horizon = -camera.angles.x + (draw_h / 2);

	// Draw left to right
	for (s.x = 0; s.x < draw_w; s.x++)
	{
		// variables
		rex_vec2s ray_dir, delta_dist, side_dist;
		rex_vec2i step, map_pos;

		// clear z-buffer
		memset(zbuffer, 255, draw_h);

		// map pos (int)
		map_pos.x = RexScalarToInteger(p.x);
		map_pos.y = RexScalarToInteger(p.y);

		// calculate ray direction
		ray_dir.x = REX_MUL(REX_DIV(REX_SCALAR(2.0f), REX_SCALAR(draw_w)), REX_SCALAR(s.x)) - REX_SCALAR(1.0f);
		ray_dir.y = REX_SCALAR(1.0f);

		// rotate around (0, 0) by camera yaw
		rex_vec2s temp = ray_dir;

		ray_dir.x = REX_MUL(-temp.x, cs) - REX_MUL(-temp.y, sn);
		ray_dir.y = REX_MUL(temp.x, sn) + REX_MUL(temp.y, cs);

		// get delta of ray (prevent div by 0)
		delta_dist.x = (ray_dir.x == 0) ? REX_SCALAR_MIN : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.x));
		delta_dist.y = (ray_dir.y == 0) ? REX_SCALAR_MIN : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.y));

		// calculate step and initial side_dist
		if (ray_dir.x < 0)
		{
			step.x = -1;
			side_dist.x = REX_MUL((p.x - REX_SCALAR(map_pos.x)), delta_dist.x);
		}
		else
		{
			step.x = 1;
			side_dist.x = REX_MUL((REX_SCALAR(map_pos.x) + REX_SCALAR(1) - p.x), delta_dist.x);
		}

		if (ray_dir.y < 0)
		{
			step.y = -1;
			side_dist.y = REX_MUL((p.y - REX_SCALAR(map_pos.y)), delta_dist.y);
		}
		else
		{
			step.y = 1;
			side_dist.y = REX_MUL((REX_SCALAR(map_pos.y) + REX_SCALAR(1) - p.y), delta_dist.y);
		}

		bool casting = true;
		rex_int side;
		rex_scalar dist, dist2;

		// perform DDA to draw voxels
		while (casting == true)
		{
			if (side_dist.x < side_dist.y)
			{
				side_dist.x += delta_dist.x;
				map_pos.x += step.x;
				side = 1;
			}
			else
			{
				side_dist.y += delta_dist.y;
				map_pos.y += step.y;
				side = 2;
			}

			switch (side)
			{
				case 1: dist = side_dist.x - delta_dist.x; dist2 = side_dist.y; break;

				case 2: dist = side_dist.y - delta_dist.y; dist2 = side_dist.x; break;

				default: break;
			}

			// if it goes beyond the draw distance, cut off the ray
			if (dist > camera.draw_distance) break;

			// if out of bounds, keep going in hopes of finding something in-bounds again
			// this allows rendering the map from an out of bounds location
			if (map_pos.x > (world->size.x - 1) || map_pos.x < 0) continue;
			if (map_pos.y > (world->size.y - 1) || map_pos.y < 0) continue;

			// voxel draw loop
			if (dist > REX_SCALAR(1) && dist2 > REX_SCALAR(1))
			{
				Column column = world->GetColumn(map_pos.x, map_pos.y);

				rex_int column_height = 256;

				// draw the slabs from top to bottom
				for (i = 0; i < column.slabs.size(); i++)
				{
					Slab slab = column.slabs[i];

					// z position of the slab
					rex_scalar slab_z = REX_SCALAR(column_height - slab.skipped);

					// height of the slab
					rex_scalar slab_height = REX_SCALAR(slab.drawn);

					// height delta 1 & 2
					rex_scalar height_delta1 = p.z - slab_z;
					rex_scalar height_delta2 = p.z - (slab_z - slab_height);

					// height of the line on screen
					rex_int line_start, line_end;

					// draw the side of the voxel
					{
						// height of the line on screen
						line_start = RexScalarToInteger(REX_MUL(REX_DIV(height_delta1, dist), pixel_height_scale)) + horizon;
						line_end = RexScalarToInteger(REX_MUL(REX_DIV(height_delta2, dist), pixel_height_scale)) + horizon;

						// clamp the line to the visible region
						line_start = CLAMP(line_start, 0, draw_h);
						line_end = CLAMP(line_end, 0, draw_h);

						for (s.y = line_start; s.y < line_end; s.y++)
						{
							if (REX_SCALAR(zbuffer[s.y]) > dist)
							{
								rex_uint8 c = slab.color_side;
								//c = Rex::ColormapLookup(c, RexScalarToInteger(dist));
								Rex::SurfaceDrawPixel(dst, s.x, s.y, c);
								zbuffer[s.y] = RexScalarToInteger(dist);
							}
						}
					}

					// draw the top or bottom slab of the voxel
					{
						// this is where the magic happens
						if (dist2 > (dist + REX_SCALAR(1)))
							dist2 = (dist + REX_SCALAR(1));

						// height of the line on screen
						line_start = RexScalarToInteger(REX_MUL(REX_DIV(height_delta1, dist2), pixel_height_scale)) + horizon;
						line_end = RexScalarToInteger(REX_MUL(REX_DIV(height_delta2, dist2), pixel_height_scale)) + horizon;

						// clamp the line to the visible region
						line_start = CLAMP(line_start, 0, draw_h);
						line_end = CLAMP(line_end, 0, draw_h);

						for (s.y = line_start; s.y < line_end; s.y++)
						{
							if (REX_SCALAR(zbuffer[s.y]) > dist2)
							{
								rex_uint8 c = p.z > slab_z ? slab.color_top : slab.color_bottom;
								//c = Rex::ColormapLookup(c, RexScalarToInteger(dist));
								Rex::SurfaceDrawPixel(dst, s.x, s.y, c);
								zbuffer[s.y] = RexScalarToInteger(dist2);
							}
						}
					}

					// overall height of this column
					column_height -= (slab.skipped + slab.drawn);
				}
			}
		}

		// draw actors
		for (i = 0; i < world->actors.size(); i++)
		{
			Actor actor = world->actors[i];

			rex_vec3s v, pv;
			rex_vec2i sv;
			rex_int line_height, line_start, line_end;
			rex_int line_startx, line_endx;

			// transform the sprite into the player's view
			v.x = actor.origin.x - camera.origin.x;
			v.y = actor.origin.y - camera.origin.y;
			v.z = -actor.origin.z + camera.origin.z;

			// rotate the y coordinate into the player's view
			pv.y = REX_MUL(v.x, sn) + REX_MUL(v.y, cs);

			// if behind the player, don't draw
			// if beyond the draw distance, don't draw
			if (pv.y < REX_SCALAR(1) || pv.y > camera.draw_distance) continue;

			// rotate the x and z coordinates into the player's view
			pv.x = REX_MUL(-v.x, cs) - REX_MUL(-v.y, sn);
			pv.z = v.z - REX_DIV(REX_MUL(REX_SCALAR(camera.angles.x), pv.y), pixel_height_scale);

			// get screen coordinates
			sv.x = RexScalarToInteger(REX_DIV(REX_MUL(pv.x, pixel_height_scale), pv.y)) + (draw_w / 2);
			sv.y = RexScalarToInteger(REX_DIV(REX_MUL(pv.z, pixel_height_scale), pv.y)) + (draw_h / 2);

			// height of line on screen
			line_height = RexScalarToInteger(REX_MUL(REX_DIV(REX_SCALAR(4), pv.y), pixel_height_scale));

			line_start = sv.y - (line_height / 2);
			line_end = sv.y + (line_height / 2);

			line_startx = s.x - (line_height / 2);
			line_endx = s.x + (line_height / 2);

			// clamp line to the visible area
			line_start = CLAMP(line_start, 0, draw_h);
			line_end = CLAMP(line_end, 0, draw_h);

			line_startx = CLAMP(line_startx, 0, draw_w);
			line_endx = CLAMP(line_endx, 0, draw_w);

			if (sv.x > line_startx && sv.x < line_endx) 
			{
				//rex_int tex_x_inc = actor.color.width / line_height;
				//rex_int tex_x1 = tex_x_inc * (s.x - line_startx);
				//rex_int tex_x2 = tex_x1 + 1;
				//Rex::SurfaceBlit8(dst, s.x, line_start, s.x + 1, line_end, &actor.color, tex_x1, 0, tex_x2, actor.color.height, Rex::COLORKEY);

				// draw the line
				for (s.y = line_start; s.y < line_end; s.y++)
				{
					// check zbuffer
					if (REX_SCALAR(zbuffer[s.y]) > pv.y)
					{
						Rex::SurfaceDrawPixel(dst, s.x, s.y, 255);
						zbuffer[sv.y] = RexScalarToInteger(pv.y);
					}
				}
			}
		}
	}
}

// Set render dimensions
void SetRenderDimensions(rex_int w, rex_int h)
{
	if (zbuffer) delete zbuffer;

	draw_w = w;
	draw_h = h;

	zbuffer = new rex_uint8[h];
}

} // namespace Voxel
