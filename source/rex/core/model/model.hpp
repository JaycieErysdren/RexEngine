// ========================================================
//
// FILE:			/source/rex/core/model/model.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Model header
//
// LAST EDITED:		December 25th, 2022
//
// ========================================================

#ifndef __REX_MODEL_H__
#define __REX_MODEL_H__

//
//
// Rex namespace: Model
//
//

namespace Rex
{

//
// Model container object
//

class Model
{
	public:

		// Model data type
		union
		{
			VoxelData *voxel;
			PolyData *poly;
			TileData *tile;
			SectorData *sector;
			SpriteData *sprite;
		} data;

		// Selector for model data
		enum
		{
			Voxel,
			Poly,
			Tile,
			Sector,
			Sprite,
		} tag;
};

} // namespace Rex

#endif // __REX_MODEL_H__
