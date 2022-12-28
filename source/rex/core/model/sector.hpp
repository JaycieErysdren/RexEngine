// ========================================================
//
// FILE:			/source/rex/core/model/sector.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Sector Model header
//
// LAST EDITED:		December 26th, 2022
//
// ========================================================

#ifndef __REX_MODEL_SECTOR_H__
#define __REX_MODEL_SECTOR_H__

//
//
// Rex namespace: Sector Model
//
//

namespace Rex
{

//
// Wall
//

class Wall
{
	public:
		rex_int32 vert1, vert2;		// Vertex IDs (wall)
		rex_int32 sect1, sect2;		// Sector IDs (portal)
		rex_uint8 color_upper;		// Upper color
		rex_uint8 color_middle;		// Middle color
		rex_uint8 color_lower;		// Lower color
};

//
// Sector
//

class Sector
{
	public:
		rex_scalar floor_height;	// Height of floor
		rex_scalar ceiling_height;	// Height of ceiling
		rex_uint8 floor_color;		// Color of floor
		rex_uint8 ceiling_color;	// Color of ceiling
		rex_uint8 light_level;		// Light level modifier
		vector<rex_int32> walls;	// Wall IDs
};

//
// Arrays of vertices, walls, and sectors
//

class SectorData
{
	public:
		vector<Vertex> vertices;	// Array of vertices
		vector<Wall> walls;			// Array of walls
		vector<Sector> sectors;		// Array of sectors
};

} // namespace Rex

#endif // __REX_MODEL_SECTOR_H__
