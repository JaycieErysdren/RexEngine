// ========================================================
//
// FILE:			/source/rex/core/model/voxel.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Voxel Model header
//
// LAST EDITED:		December 26th, 2022
//
// ========================================================

#ifndef __REX_MODEL_VOXEL_H__
#define __REX_MODEL_VOXEL_H__

//
//
// Rex namespace: Voxel Model
//
//

namespace Rex
{

//
// RLE encoded voxel element
//

class VoxelSlab
{
	public:
		// Number of "air" voxels above the drawn voxels
		rex_uint8 voxels_skipped;

		// Number of drawn voxels
		rex_uint8 voxels_drawn;

		// Side color
		rex_uint8 color_side;

		// Top color
		rex_uint8 color_top;

		// Bottom color
		rex_uint8 color_bottom;
};

//
// Array of voxel slab elements
//

class VoxelColumn
{
	public:
		vector<VoxelSlab> slabs;
};

//
// Array of voxel column elements
//

class VoxelData
{
	public:
		vector<VoxelColumn> columns;
};

} // namespace Rex

#endif // __REX_MODEL_VOXEL_H__
