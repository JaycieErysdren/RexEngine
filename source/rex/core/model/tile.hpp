// ========================================================
//
// FILE:			/source/rex/core/model/tile.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Tile Model header
//
// LAST EDITED:		December 26th, 2022
//
// ========================================================

#ifndef __REX_MODEL_TILE_H__
#define __REX_MODEL_TILE_H__

//
//
// Rex namespace: Tile Model
//
//

namespace Rex
{

//
// Array of tile elements
//

class TileData
{
	public:
		vector<rex_uint8> tiles;
};

} // namespace Rex

#endif // __REX_MODEL_TILE_H__
