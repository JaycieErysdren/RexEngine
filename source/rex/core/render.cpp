// ========================================================
//
// FILE:			/source/rex/core/render.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Render implementation
//
// LAST EDITED:		December 18th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Render
//
//

namespace Rex
{

//
//
// Functions
//
//

//
// Actor3D
//

// Renders an Actor3D scene hierarchy
void RenderScene3D(Surface *dst, Surface *zbuffer, Actor3D *root, Actor3D *camera, rex_scalar pixel_height_scale)
{
	if (root == NULL) return;

	// Render this actor
	switch (root->type)
	{
		case ACTOR3D_VOXELMODEL:
			Voxel::RenderVoxelModel(dst, zbuffer, (Voxel::VoxelModel *)root->model, camera->origin, camera->angles, camera->draw_distance, pixel_height_scale);
			break;

		default:
			break;
	}

	// Render children
	for (rex_int i = 0; i < root->children.size(); i++)
	{
		RenderScene3D(dst, zbuffer, root->children[i], camera, pixel_height_scale);
	}
}

//
// Actor2D
//

// Renders an Actor2D scene hierarchy
void RenderScene2D(Surface *dst, Surface *zbuffer, Actor2D *root, rex_scalar pixel_height_scale)
{
	if (root == NULL) return;

	// Render this actor
	if (root->type != ACTOR2D_NONE) root->Draw(dst, root->color_blit_mode);

	// Render children
	for (rex_int i = 0; i < root->children.size(); i++)
	{
		RenderScene2D(dst, zbuffer, root->children[i], pixel_height_scale);
	}
}

} // namespace Rex
