// ========================================================
//
// FILE:			/source/rex/core/softrend/sector.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Sector Renderer header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

#ifndef __REX_SOFTREND_sECTOR_H__
#define __REX_SOFTREND_sECTOR_H__

//
//
// Rex namespace: Sector Renderer
//
//

namespace Rex
{

// Renders a SectorData object
void RenderSectorData(Surface *dst, Surface *zbuffer, SectorData *data, rex_vec3s origin, rex_vec3i angles, rex_scalar draw_distance, rex_scalar pixel_height_scale);

} // namespace Rex

#endif // __REX_SOFTREND_sECTOR_H__
