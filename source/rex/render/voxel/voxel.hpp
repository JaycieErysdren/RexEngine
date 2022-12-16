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
// LAST EDITED:		December 15th, 2022
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

// Actor class definition
class Actor
{
	public:
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
		rex_color color_side;

		// Top color
		rex_color color_top;

		// Bottom color
		rex_color color_bottom;

		//
		// Functions
		//

		// Constructor
		Slab();

		// Constructor with variables
		Slab(rex_uint8 skipped_voxels, rex_uint8 drawn_voxels, rex_color side_color, rex_color top_color, rex_color bottom_color);
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

		// Array of actors in the worldf
		vector<Actor> actors;

		//
		// Functions
		//

		// Constructor with default size
		World();

		// Constructor with size & name declaration
		World(string world_name, rex_int size_x, rex_int size_y, rex_int size_z);

		// Constructor to load from file
		World(string filename);

		// Add a slab at the specificed column coordinate
		void AddSlab(rex_int x, rex_int y, Slab slab);

		// Add an actor at the specified coordinate
		void AddActor(rex_scalar x, rex_scalar y, rex_scalar z);

		// Add an actor
		void AddActor(Actor actor);

		// Returns the column at the specificed coordinate
		Column GetColumn(rex_int x, rex_int y);

	private:

		//
		// Functions
		//

		// Fill up the columns array
		void FillColumnArray();
};

//
//
// Functions
//
//

//
// Rendering
//

// Initialize renderer
void Initialize(rex_int render_width, rex_int render_height);

// Shutdown renderer
void Shutdown();

// Render an image to the specified surface
void Render(Rex::Surface *dst, Rex::Camera camera, World *world, rex_scalar pixel_height_scale);

// Set render dimensions
void SetRenderDimensions(rex_int w, rex_int h);

} // namespace Voxel

#endif // __VOXEL_H__
