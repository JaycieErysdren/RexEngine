// ========================================================
//
// FILE:			/source/rex/core/render.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Render header
//
// LAST EDITED:		December 18th, 2022
//
// ========================================================

#ifndef __REX_RENDER_H__
#define __REX_RENDER_H__

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

// Renders an Actor3D scene hierarchy
void RenderScene3D(Surface *dst, Surface *zbuffer, Actor3D *root, Actor3D *camera, rex_scalar pixel_height_scale);

// Renders an Actor2D scene hierarchy
void RenderScene2D(Surface *dst, Surface *zbuffer, Actor2D *root, rex_scalar pixel_height_scale);

} // namespace Rex

#endif // __REX_RENDER_H__
