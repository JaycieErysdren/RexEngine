// ========================================================
//
// FILE:			/source/rex/render/raycast/render.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Raycast namespace: Render header
//
// LAST EDITED:		December 18th, 2022
//
// ========================================================

#ifndef __RAYCAST_RENDER_H__
#define __RAYCAST_RENDER_H__

//
//
// Raycast namespace: Render
//
//

namespace Raycast
{

//
//
// Classes
//
//

// RaycastTile class definition
class RaycastTile
{
	public:

		//
		// Variables
		//

		// Tile height
		rex_scalar height;

		// Tile colors
		rex_uint8 color_ns;
		rex_uint8 color_ew;
		rex_uint8 color_floor;
};

// RaycastModel class definition
class RaycastModel
{
	public:

		//
		// Variables
		//

		// Dimensions (x, y)
		rex_vec2i dimensions;

		// Array of tile elements
		vector<rex_int8> tiles;

		//
		// Danger zone
		//

		// Pointer to the allocated memory for this RaycastModel
		void *memory;

		//
		// Functions
		//

		// Constructors
		RaycastModel();
		RaycastModel(rex_int size_x, rex_int size_y);

		// Get and set wall
		rex_int8 GetTile(rex_int x, rex_int y);
		void SetTile(rex_int x, rex_int y, rex_int8 val);
};

//
//
// Functions
//
//

//
// Add / free
//

// Add a RaycastModel to memory
RaycastModel *AddRaycastModel(rex_int size_x, rex_int size_y);

// Free a RaycastModel from memory
void FreeRaycastModel(RaycastModel *model);

//
// Rendering
//

// Render an image to the specified surface
void RenderRaycastModel(Rex::Surface *dst, Rex::Surface *zbuffer, RaycastModel *model, rex_vec3s origin, rex_vec3i angles, rex_scalar draw_distance, rex_scalar pixel_height_scale);

} // namespace Raycast

#endif // __RAYCAST_RENDER_H__
