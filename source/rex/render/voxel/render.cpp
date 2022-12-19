// ========================================================
//
// FILE:			/source/rex/render/voxel/render.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Voxel namespace: Render implementation
//
// LAST EDITED:		December 18th, 2022
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
// Functions
//
//

// Render an image to the specified surface
void RenderVoxelModel(Rex::Surface *dst, Rex::Surface *zbuffer, VoxelModel *model, rex_vec3s origin, rex_vec3i angles, rex_scalar draw_distance, rex_scalar pixel_height_scale)
{
	if (model == NULL) return;

	// General variables
	rex_int i;

	// Sin and cos of the camera's yaw
	rex_scalar sn = Rex::math_table->sin[angles.y];
	rex_scalar cs = Rex::math_table->cos[angles.y];

	// Drawable area
	rex_int draw_w = dst->width;
	rex_int draw_h = dst->height;

	// Screen coords
	rex_vec3i s;

	// Horizon
	rex_int horizon = -angles.x + (draw_h / 2);

	// Draw left to right
	for (s.x = 0; s.x < draw_w; s.x++)
	{
		// variables
		rex_vec2s ray_dir, delta_dist, side_dist;
		rex_vec2i step, map_pos;

		// map pos (int)
		map_pos.x = RexScalarToInteger(origin.x);
		map_pos.y = RexScalarToInteger(origin.y);

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
			side_dist.x = REX_MUL((origin.x - REX_SCALAR(map_pos.x)), delta_dist.x);
		}
		else
		{
			step.x = 1;
			side_dist.x = REX_MUL((REX_SCALAR(map_pos.x) + REX_SCALAR(1) - origin.x), delta_dist.x);
		}

		if (ray_dir.y < 0)
		{
			step.y = -1;
			side_dist.y = REX_MUL((origin.y - REX_SCALAR(map_pos.y)), delta_dist.y);
		}
		else
		{
			step.y = 1;
			side_dist.y = REX_MUL((REX_SCALAR(map_pos.y) + REX_SCALAR(1) - origin.y), delta_dist.y);
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
			if (dist > draw_distance) break;

			// if out of bounds, keep going in hopes of finding something in-bounds again
			// this allows rendering the map from an out of bounds location
			if (map_pos.x > (model->dimensions.x - 1) || map_pos.x < 0) continue;
			if (map_pos.y > (model->dimensions.y - 1) || map_pos.y < 0) continue;

			// voxel draw loop
			if (dist > REX_SCALAR(1) && dist2 > REX_SCALAR(1))
			{
				VoxelColumn column = model->GetColumn(map_pos.x, map_pos.y);

				rex_int column_height = model->dimensions.z;

				// draw the slabs from top to bottom
				for (i = 0; i < column.slabs.size(); i++)
				{
					VoxelSlab slab = column.slabs[i];

					// z position of the slab
					rex_scalar slab_z = REX_SCALAR(column_height - slab.voxels_skipped);

					// height of the slab
					rex_scalar slab_height = REX_SCALAR(slab.voxels_drawn);

					// height delta 1 & 2
					rex_scalar height_delta1 = origin.z - slab_z;
					rex_scalar height_delta2 = origin.z - (slab_z - slab_height);

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
							s.z = Rex::SurfaceGetPixel(zbuffer, s.x, s.y);

							if (REX_SCALAR(s.z) > dist)
							{
								rex_uint8 c = slab.color_side;
								//c = Rex::ColormapLookup(c, RexScalarToInteger(dist));
								Rex::SurfaceDrawPixel(dst, s.x, s.y, c);
								Rex::SurfaceDrawPixel(zbuffer, s.x, s.y, RexScalarToInteger(dist));
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
							s.z = Rex::SurfaceGetPixel(zbuffer, s.x, s.y);

							if (REX_SCALAR(s.z) > dist2)
							{
								rex_uint8 c = origin.z > slab_z ? slab.color_top : slab.color_bottom;
								//c = Rex::ColormapLookup(c, RexScalarToInteger(dist));
								Rex::SurfaceDrawPixel(dst, s.x, s.y, c);
								Rex::SurfaceDrawPixel(zbuffer, s.x, s.y, RexScalarToInteger(dist2));
							}
						}
					}

					// overall height of this column
					column_height -= (slab.voxels_skipped + slab.voxels_drawn);

					if (column_height < 1) break;
					if (i >= column.slabs.size()) break;
				}
			}
		}
	}
}

} // namespace Voxel
