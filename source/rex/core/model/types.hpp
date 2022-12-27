// ========================================================
//
// FILE:			/source/rex/core/model/types.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Model types header
//
// LAST EDITED:		December 25th, 2022
//
// ========================================================

#ifndef __REX_MODEL_TYPES_H__
#define __REX_MODEL_TYPES_H__

//
//
// Rex namespace: Model Types
//
//

namespace Rex
{

//
// Vertex
//

class Vertex
{
	public:
		rex_scalar x, y, z;
};

} // namespace Rex

#include "voxel.hpp"
#include "poly.hpp"
#include "tile.hpp"
#include "sector.hpp"
#include "sprite.hpp"
#include "model.hpp"

#endif // __REX_MODEL_TYPES_H__
