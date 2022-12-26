// ========================================================
//
// FILE:			/source/rex/core/model.hpp
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

// Voxel model data
class VoxelData
{

};

// Poly model data
class PolyData
{

};

// Tile model data
class TileData
{

};

// Sector model data
class SectorData
{

};

// Sprite model data
class SpriteData
{
	public:
		Surface color;
};

// Model container object
class Model
{
	public:

		// model data type
		union
		{
			VoxelData *voxel;
			PolyData *poly;
			TileData *tile;
			SectorData *sector;
			SpriteData *sprite;
		} data;

		// selector for model data
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
