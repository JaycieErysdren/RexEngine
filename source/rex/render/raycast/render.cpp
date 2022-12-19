// ========================================================
//
// FILE:			/source/rex/render/raycast/render.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Raycast namespace: Render implementation
//
// LAST EDITED:		December 18th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// Raycast renderer header
#include "render.hpp"

//
//
// Raycast namespace: Render
//
//

namespace Raycast
{

//
//
// Functions
//
//

//
// RaycastModel
//

// Class constructor
RaycastModel::RaycastModel()
{
	dimensions.x = 32;
	dimensions.y = 32;

	tiles.resize(dimensions.x * dimensions.y);
}

// Class constructor with parameters
RaycastModel::RaycastModel(rex_int size_x, rex_int size_y)
{
	dimensions.x = size_x;
	dimensions.y = size_y;

	tiles.resize(dimensions.x * dimensions.y);
}

// Get the tile data at the specified coordinate
RaycastTile RaycastModel::GetTile(rex_int x, rex_int y)
{
	return tiles[(y * dimensions.y) + x];
}

// Set the tile data at the specified coordinate
void RaycastModel::SetTile(rex_int x, rex_int y, RaycastTile tile)
{
	tiles[(y * dimensions.y) + x] = tile;
}

//
// Add / free
//

// Add a RaycastModel to memory
RaycastModel *AddRaycastModel(rex_int size_x, rex_int size_y)
{
	void *model_memory = Rex::MemPool_Alloc(Rex::MEMORY_RENDERER, sizeof(RaycastModel));
	RaycastModel *model = new(model_memory) RaycastModel(size_x, size_y);

	model->memory = model_memory;

	return model;
}

// Free a RaycastModel from memory
void FreeRaycastModel(RaycastModel *model)
{
	if (model == NULL) return;

	void *model_memory = model->memory;

	// Call destructor
	model->~RaycastModel();

	// Free the actor
	Rex::MemPool_Free(Rex::MEMORY_RENDERER, model_memory);
}

//
// Rendering
//

// Render an image to the specified surface
void RenderRaycastModel(Rex::Surface *dst, Rex::Surface *zbuffer, RaycastModel *model, rex_vec3s origin, rex_vec3i angles, rex_scalar draw_distance, rex_scalar pixel_height_scale)
{
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
		bool hit = false;
		rex_int side;
		rex_scalar dist;

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
				case 1: dist = side_dist.x - delta_dist.x; break;

				case 2: dist = side_dist.y - delta_dist.y; break;

				default: break;
			}

			// if it goes beyond the draw distance, cut off the ray
			if (dist > draw_distance) break;

			// if out of bounds, keep going in hopes of finding something in-bounds again
			// this allows rendering the map from an out of bounds location
			if (map_pos.x > (model->dimensions.x - 1) || map_pos.x < 0) continue;
			if (map_pos.y > (model->dimensions.y - 1) || map_pos.y < 0) continue;

			if (model->GetTile(map_pos.x, map_pos.y).height > REX_SCALAR(0))
			{
				hit = true;
				break;
			}
		}

		if (hit == true && dist > REX_SCALAR(1))
		{
			RaycastTile tile = model->GetTile(map_pos.x, map_pos.y);

			rex_int line_start, line_end, line_height;

			// height of the line on screen
			line_height = RexScalarToInteger(REX_MUL(REX_DIV(REX_SCALAR(draw_h), dist), pixel_height_scale)) + horizon;

			line_start = -line_height / 2 + draw_h / 2;
			line_end = line_height / 2 + draw_h / 2;

			// clamp the line to the visible region
			line_start = CLAMP(line_start, 0, draw_h);
			line_end = CLAMP(line_end, 0, draw_h);

			for (s.y = line_start; s.y < line_end; s.y++)
			{
				s.z = Rex::SurfaceGetPixel(zbuffer, s.x, s.y);

				if (REX_SCALAR(s.z) > dist)
				{
					rex_uint8 c = tile.color_ew;
					Rex::SurfaceDrawPixel(dst, s.x, s.y, c);
					Rex::SurfaceDrawPixel(zbuffer, s.x, s.y, RexScalarToInteger(dist));
				}
			}
		}
	}
}

} // namespace Raycast
