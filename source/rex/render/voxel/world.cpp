// ========================================================
//
// FILE:			/source/rex/modules/voxel/world.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Voxel namespace: World implementation
//
// LAST EDITED:		December 15th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// Voxel renderer header
#include "voxel.hpp"

//
//
// Voxel namespace: World
//
//

namespace Voxel
{

//===========================================
//
// Slab
//
//===========================================

// Constructor
Slab::Slab()
{

}

// Constructor with variables
Slab::Slab(rex_uint8 skipped_voxels, rex_uint8 drawn_voxels, rex_color side_color, rex_color top_color, rex_color bottom_color)
{
	skipped = skipped_voxels;
	drawn = drawn_voxels;
	color_side = side_color;
	color_top = top_color;
	color_bottom = bottom_color;
}

//===========================================
//
// Column
//
//===========================================

// Add a slab to the slab array
void Column::AddSlab(Slab slab)
{
	slabs.push_back(slab);
}

//===========================================
//
// World
//
//===========================================

//
// Public
//

// Constructor with default size
World::World()
{
	// Set world dimensions to defaults
	size.x = 1024;
	size.y = 1024;
	size.z = 256;

	// Fill up the columns array
	FillColumnArray();
}

// Constructor with size declaration
World::World(string world_name, rex_int size_x, rex_int size_y, rex_int size_z)
{
	// Set world dimensions
	size.x = size_x;
	size.y = size_y;
	size.z = size_z;

	// Set world name
	name = world_name;

	// Fill up the columns array
	FillColumnArray();
}

// Constructor to load from file
World::World(string filename)
{
	// open file
	FILE *file = fopen(filename.c_str(), "rb");
	if (file == NULL) return;

	// close file
	fclose(file);
}

// Add a slab at the specificed column coordinate
void World::AddSlab(rex_int x, rex_int y, Slab slab)
{
	columns[(y * size.y) + x].AddSlab(slab);
}

// Returns the column at the specificed coordinate
Column World::GetColumn(rex_int x, rex_int y)
{
	return columns[(y * size.y) + x];
}

//
// Private
//

// Fill up the columns array
void World::FillColumnArray()
{
	columns.resize(size.x * size.y);
}

} // namespace Voxel
