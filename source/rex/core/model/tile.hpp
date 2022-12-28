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
// LAST EDITED:		December 27th, 2022
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
// Tile element
//

class Tile
{
	public:
		rex_bool floor;			// Is this a floor tile or not?
		rex_uint8 color_ns;		// North-south color
		rex_uint8 color_ew;		// East-west color
};

//
// Array of tile elements
//

class TileData
{
	public:
		vector<Tile> tiles;
};

} // namespace Rex

#endif // __REX_MODEL_TILE_H__
