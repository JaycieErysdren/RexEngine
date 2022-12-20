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
rex_uint8 RaycastModel::GetTile(rex_int x, rex_int y)
{
	return tiles[(y * dimensions.y) + x];
}

// Set the tile data at the specified coordinate
void RaycastModel::SetTile(rex_int x, rex_int y, rex_uint8 val)
{
	tiles[(y * dimensions.y) + x] = val;
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

	// Free surfaces
	for (rex_int i = 0; i < model->surfaces.size(); i++)
	{
		Rex::SurfaceDestroy(&model->surfaces[i]);
	}

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
	// Draw bounds
	rex_int32 draw_w = dst->width;
	rex_int32 draw_h = dst->height;

	// The positions of the pixels we'll be drawing
	rex_vec3i s;

	// Horizon
	rex_int horizon = -angles.x + (draw_h / 2);

	// Current sin, cos and tan of player's yaw
	rex_scalar sn = Rex::math_table->sin[angles.y];
	rex_scalar cs = Rex::math_table->cos[angles.y];

	// Ray sweep loop
	for (s.x = 0; s.x < draw_w; s.x++)
	{
		rex_vec2s raydir;
		rex_vec2s delta_dist;
		rex_vec2s side_dist;
		rex_vec3i map_pos, step;
		map_pos.x = RexScalarToInteger(origin.x);
		map_pos.y = RexScalarToInteger(origin.y);
		bool hit = false, side = false;
		bool oob = false;

		// calculate ray direction
		raydir.x = REX_MUL(REX_DIV(REX_SCALAR(2.0f), REX_SCALAR(draw_w)), REX_SCALAR(s.x)) - REX_SCALAR(1.0f);
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
			step.x = -1;
			side_dist.x = REX_MUL((origin.x - REX_SCALAR(map_pos.x)), delta_dist.x);
		}
		else
		{
			step.x = 1;
			side_dist.x = REX_MUL((REX_SCALAR(map_pos.x) + REX_SCALAR(1) - origin.x), delta_dist.x);
		}

		if (raydir.y < 0)
		{
			step.y = -1;
			side_dist.y = REX_MUL((origin.y - REX_SCALAR(map_pos.y)), delta_dist.y);
		}
		else
		{
			step.y = 1;
			side_dist.y = REX_MUL((REX_SCALAR(map_pos.y) + REX_SCALAR(1) - origin.y), delta_dist.y);
		}

		//perform DDA
		while (hit == false && oob == false)
		{
			if (side_dist.x < side_dist.y)
			{
				side_dist.x += delta_dist.x;
				map_pos.x += step.x;
				side = false;
			}
			else
			{
				side_dist.y += delta_dist.y;
				map_pos.y += step.y;
				side = true;
			}

			//Check if ray has hit a wall
			if (model->GetTile(map_pos.x, map_pos.y) > 0)
				hit = true;

			// Check if ray is out of bounds
			if (map_pos.y >= model->dimensions.y || map_pos.y < 0 || map_pos.x >= model->dimensions.x || map_pos.x < 0)
				oob = true;
		}

		if (oob == true) continue;

		rex_scalar dist;

		if (side == false) dist = (side_dist.x - delta_dist.x);
		else dist = (side_dist.y - delta_dist.y);

		if (dist > REX_SCALAR(0))
		{
			// height delta 1 & 2
			rex_scalar tile_z = REX_SCALAR(1);
			rex_scalar tile_height = REX_SCALAR(1);
			rex_scalar height_delta1 = origin.z - tile_z;
			rex_scalar height_delta2 = origin.z - (tile_z - tile_height);

			//Calculate height of line to draw on screen
			rex_int line_start = RexScalarToInteger(REX_MUL(REX_DIV(height_delta1, dist), pixel_height_scale)) + horizon;
			rex_int line_end = RexScalarToInteger(REX_MUL(REX_DIV(height_delta2, dist), pixel_height_scale)) + horizon;

			rex_int line_height = line_end - line_start;

			// clamp to vertical area
			line_start = CLAMP(line_start, 0, draw_h);
			line_end = CLAMP(line_end, 0, draw_h);

			bool tmap = true;

			if (tmap == true)
			{
				rex_uint8 tindex = model->GetTile(map_pos.x, map_pos.y) - 1;

				// calculate value of wallX
				rex_scalar wall_x; // where exactly the wall was hit

				if (side == false) wall_x = origin.y + REX_MUL(dist, raydir.y);
				else wall_x = origin.x + REX_MUL(dist, raydir.x);

				wall_x -= REX_FLOOR(wall_x);

				//x coordinate on the texture
				rex_int32 tex_x = RexScalarToInteger(REX_MUL(wall_x, REX_SCALAR(64)));
				if (side == false && raydir.x > 0) tex_x = 64 - tex_x - 1;
				if (side == true && raydir.y < 0) tex_x = 64 - tex_x - 1;

				// How much to increase the texture coordinate per screen pixel
				rex_scalar step = REX_SAFEDIV(REX_MUL(REX_SCALAR(1.0f), REX_SCALAR(64)), REX_SCALAR(line_height));

				// Starting texture coordinate
				rex_scalar texcoord = REX_MUL(REX_SCALAR(line_start - (draw_h / 2) + (line_height / 2)), step);

				for (s.y = line_start; s.y < line_end; s.y++)
				{
					if (REX_SCALAR(Rex::SurfaceGetPixel(zbuffer, s.x, s.y)) > dist)
					{
						// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
						rex_int32 tex_y = RexScalarToInteger(texcoord) & (64 - 1);
						texcoord += step;
						uint8_t color = Rex::SurfaceGetPixel(&model->surfaces[tindex], tex_x, tex_y);

						// Lookup in colormap for brightness
						if (side == true) color = Rex::ColormapLookup(color, RexScalarToInteger(dist) - 4);
						else color = Rex::ColormapLookup(color, RexScalarToInteger(dist));

						Rex::SurfaceDrawPixel(dst, s.x, s.y, color);
						Rex::SurfaceDrawPixel(zbuffer, s.x, s.y, RexScalarToInteger(dist));
					}

					if (s.y > draw_h || s.y < 0) break;
				}
			}
			else
			{
				// choose wall color
				rex_uint8 color = model->GetTile(map_pos.x, map_pos.y);

				// Lookup in colormap for brightness
				if (side == true) color = Rex::ColormapLookup(color, RexScalarToInteger(dist) - 4);
				else color = Rex::ColormapLookup(color, RexScalarToInteger(dist));

				// draw with zbuffer
				for (s.y = line_start; s.y < line_end; s.y++)
				{
					if (REX_SCALAR(Rex::SurfaceGetPixel(zbuffer, s.x, s.y)) > dist)
					{
						Rex::SurfaceDrawPixel(dst, s.x, s.y, color);
						Rex::SurfaceDrawPixel(zbuffer, s.x, s.y, RexScalarToInteger(dist));
					}

					if (s.y > draw_h || s.y < 0) break;
				}
			}
		}
	}
}

} // namespace Raycast
