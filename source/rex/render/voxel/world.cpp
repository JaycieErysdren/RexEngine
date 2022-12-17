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
Slab::Slab(rex_uint8 skipped_voxels, rex_uint8 drawn_voxels, rex_uint8 side_color, rex_uint8 top_color, rex_uint8 bottom_color)
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

const char voxel_file_world_magic[] = "VREX";
const char voxel_file_slab_magic[] = "SLAB";
const char voxel_file_column_magic[] = "COLM";

// Constructor with default size
World::World()
{
	// Set world dimensions to defaults
	size.x = 1024;
	size.y = 1024;
	size.z = 256;

	// Fill up the columns array
	columns.resize(size.x * size.y);
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
	columns.resize(size.x * size.y);
}

// Constructor to load world from file
World::World(string filename)
{
	// variables
	rex_int32 x, y, z, i;
	rex_int32 num_columns;

	// open file
	FILE *file = fopen(filename.c_str(), "rb");
	if (file == NULL) return;

	// read magic
	char magic[4];
	fread(magic, sizeof(char), 4, file);
	if (memcmp(magic, voxel_file_world_magic, 4) != 0) return;

	// read map dimensions
	fread(&size.x, sizeof(rex_int32), 1, file);
	fread(&size.y, sizeof(rex_int32), 1, file);
	fread(&size.z, sizeof(rex_int32), 1, file);

	// read number of columns
	fread(&num_columns, sizeof(rex_int32), 1, file);

	// read map name
	char namebuf[108];
	name = fgets(namebuf, 108, file);

	// seek to end of header
	fseek(file, 128L, SEEK_SET);

	// fill up the columns array
	columns.resize(size.x * size.y);

	for (i = 0; i < num_columns; i++)
	{
		// read magic
		fread(magic, sizeof(char), 4, file);
		if (memcmp(magic, voxel_file_column_magic, 4) != 0) return;

		// read x and y of column
		fread(&x, sizeof(rex_int32), 1, file);
		fread(&y, sizeof(rex_int32), 1, file);

		// read number of slabs
		rex_int32 num_slabs;
		fread(&num_slabs, sizeof(rex_int32), 1, file);
		if (num_slabs == 0) continue;

		// read slabs
		for (z = 0; z < num_slabs; z++)
		{
			// read magic
			fread(magic, sizeof(char), 4, file);
			if (memcmp(magic, voxel_file_slab_magic, 4) != 0) return;

			Slab slab;

			fread(&slab.skipped, sizeof(rex_uint8), 1, file);
			fread(&slab.drawn, sizeof(rex_uint8), 1, file);
			fread(&slab.color_side, sizeof(rex_uint8), 1, file);
			fread(&slab.color_top, sizeof(rex_uint8), 1, file);
			fread(&slab.color_bottom, sizeof(rex_uint8), 1, file);

			AddSlab(x, y, slab);
		}
	}

	// close file
	fclose(file);
}

// Save world to file
void World::Save(string filename)
{
	// variables
	rex_int32 x, y, z;

	// open file
	FILE *file = fopen(filename.c_str(), "wb");
	if (file == NULL) return;

	// write magic
	fwrite(voxel_file_world_magic, sizeof(char), 4, file);

	// write map dimensions
	fwrite(&size.x, sizeof(rex_int32), 1, file);
	fwrite(&size.y, sizeof(rex_int32), 1, file);
	fwrite(&size.z, sizeof(rex_int32), 1, file);

	// write number of columns
	rex_int32 num_columns = 0;

	for (y = 0; y < size.y; y++)
	{
		for (x = 0; x < size.x; x++)
		{
			Column column = columns[y * size.x + x];
			rex_int32 num_slabs = column.slabs.size();

			if (num_slabs > 0) num_columns++;
		}
	}

	fwrite(&num_columns, sizeof(rex_int32), 1, file);

	// write map name
	fwrite(name.c_str(), sizeof(char), name.size(), file);

	// pad header to 128 bytes
	string padding(108 - name.size(), '\0');
	fwrite(padding.c_str(), sizeof(char), padding.size(), file);

	// write columns
	for (y = 0; y < size.y; y++)
	{
		for (x = 0; x < size.x; x++)
		{
			Column column = columns[y * size.x + x];
			rex_int32 num_slabs = column.slabs.size();

			if (num_slabs < 1) continue;

			// write magic
			fwrite(voxel_file_column_magic, sizeof(char), 4, file);

			// write x and y
			fwrite(&x, sizeof(rex_int32), 1, file);
			fwrite(&y, sizeof(rex_int32), 1, file);

			// write number of slabs
			fwrite(&num_slabs, sizeof(rex_int32), 1, file);

			// write slabs
			for (z = 0; z < num_slabs; z++)
			{
				Slab slab = column.slabs[z];

				// write magic
				fwrite(voxel_file_slab_magic, sizeof(char), 4, file);

				// write skipped & drawn voxels
				fwrite(&slab.skipped, sizeof(rex_uint8), 1, file);
				fwrite(&slab.drawn, sizeof(rex_uint8), 1, file);

				// write colors
				fwrite(&slab.color_side, sizeof(rex_uint8), 1, file);
				fwrite(&slab.color_top, sizeof(rex_uint8), 1, file);
				fwrite(&slab.color_bottom, sizeof(rex_uint8), 1, file);
			}
		}
	}

	// close file
	fclose(file);
}

// Add a slab at the specificed column coordinate
void World::AddSlab(rex_int x, rex_int y, Slab slab)
{
	columns[(y * size.y) + x].AddSlab(slab);
}

// Add an actor at the specificed coordinate. Returns the ID of the actor
rex_int World::AddActor(rex_scalar x, rex_scalar y, rex_scalar z)
{
	Actor actor;

	actor.origin.x = x;
	actor.origin.y = y;
	actor.origin.z = z;

	actors.push_back(actor);

	return actors.size() - 1;
}

// Add an actor. Returns the ID of the actor
rex_int World::AddActor(Actor actor)
{
	actors.push_back(actor);

	return actors.size() - 1;
}

// Update the actor with the specified ID
void World::UpdateActor(rex_int id, Actor actor)
{
	actors[id] = actor;
}

// Returns the column at the specificed coordinate
Column World::GetColumn(rex_int x, rex_int y)
{
	return columns[(y * size.y) + x];
}

} // namespace Voxel
