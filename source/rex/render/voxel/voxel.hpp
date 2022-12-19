// ========================================================
//
// FILE:			/source/rex/modules/voxel/voxel.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Voxel renderer header
//
// LAST EDITED:		December 17th, 2022
//
// ========================================================

#ifndef __VOXEL_H__
#define __VOXEL_H__

//
//
// Voxel namespace: Render
//
//

namespace Voxel
{

//
//
// Classes
//
//

// VoxelSlab class definition
class VoxelSlab
{
	public:

		//
		// Variables
		//

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

		//
		// Functions
		//

		// Constructors
		VoxelSlab();
};

// VoxelColumn class definition
class VoxelColumn
{
	public:

		//
		// Variables
		//

		// Array of VoxelSlab elemnts
		vector<VoxelSlab> slabs;

		//
		// Functions
		//

		// Constructors
		VoxelColumn();

		// Add a slab to the slab array
		void AddSlab(VoxelSlab slab);
};

// VoxelModel class definition
class VoxelModel
{
	public:

		//
		// Variables
		//

		// Dimensions (x, y, z)
		rex_vec3i dimensions;

		// Array of VoxelColumn elements
		vector<VoxelColumn> columns;

		//
		// Danger zone
		//

		// Pointer to the allocated memory for this VoxelModel
		void *memory;

		//
		// Functions
		//

		// Constructors
		VoxelModel();
		VoxelModel(rex_int size_x, rex_int size_y, rex_int size_z);

		// Add a slab to the specified column's slab array
		void AddSlabToColumn(rex_int x, rex_int y, VoxelSlab slab);

		// Get the column at the specificed x and y coordinate
		VoxelColumn GetColumn(rex_int x, rex_int y);
};

// Actor class definition
class Actor
{
	public:
		Rex::Surface color;
		rex_vec3s origin;
};

// Slab class definition
class Slab
{
	public:

		//
		// Variables
		//

		// Number of air voxels above the drawn voxels
		rex_uint8 skipped;

		// Number of drawn voxels
		rex_uint8 drawn;

		// Side color
		rex_uint8 color_side;

		// Top color
		rex_uint8 color_top;

		// Bottom color
		rex_uint8 color_bottom;

		//
		// Functions
		//

		// Constructor
		Slab();

		// Constructor with variables
		Slab(rex_uint8 skipped_voxels, rex_uint8 drawn_voxels, rex_uint8 side_color, rex_uint8 top_color, rex_uint8 bottom_color);
};


// Column class definition
class Column
{
	public:

		//
		// Variables
		//

		// Array of RLE elemnts
		vector<Slab> slabs;

		//
		// Functions
		//

		// Add a slab to the slab array
		void AddSlab(Slab slab);
};

// World class definition
class World
{
	public:

		//
		// Variables
		//

		// World name
		rex_string name;

		// World dimensions (x, y, z)
		rex_vec3i size;

		// Array of world columns (x * y)
		vector<Column> columns;

		// Array of actors in the world
		vector<Actor> actors;

		//
		// Functions
		//

		// Constructor with default size
		World();

		// Constructor with size & name declaration
		World(string world_name, rex_int size_x, rex_int size_y, rex_int size_z);

		// Constructor to load world from file
		World(string filename);

		// Save world to file
		void Save(string filename);

		// Add a slab at the specificed column coordinate
		void AddSlab(rex_int x, rex_int y, Slab slab);

		// Add an actor at the specified coordinate. Returns the ID of the actor
		rex_int AddActor(rex_scalar x, rex_scalar y, rex_scalar z);

		// Add an actor. Returns the ID of the actor
		rex_int AddActor(Actor actor);

		// Update the actor with the specified ID
		void UpdateActor(rex_int id, Actor actor);

		// Returns the column at the specificed coordinate
		Column GetColumn(rex_int x, rex_int y);
};

//
//
// Functions
//
//

//
// Add / free
//

// Add a VoxelModel to memory
VoxelModel *AddVoxelModel(rex_int size_x, rex_int size_y, rex_int size_z);

// Free a VoxelModel from memory
void FreeVoxelModel(VoxelModel *model);

//
// Rendering
//

// Render an image to the specified surface
void RenderVoxelModel(Rex::Surface *dst, Rex::Surface *zbuffer, VoxelModel *model, rex_vec3s origin, rex_vec3i angles, rex_scalar draw_distance, rex_scalar pixel_height_scale);

} // namespace Voxel

#endif // __VOXEL_H__
