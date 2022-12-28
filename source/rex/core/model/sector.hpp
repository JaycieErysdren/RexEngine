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
// Linedef
//

class Linedef
{
	public:
		rex_int32 vert1, vert2;		// Vertex IDs
		rex_int32 side1, side2;		// Sidedef IDs
};

//
// Sidedef
//

class Sidedef
{
	public:
		rex_int32 sector;			// Sector ID
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
};

//
// Arrays of sectors, linedefs and vertices
//

class SectorData
{
	public:
		vector<Vertex> vertices;
		vector<Sector> sectors;
		vector<Linedef> linedefs;
		vector<Sidedef> sidedefs;
};

} // namespace Rex

#endif // __REX_MODEL_SECTOR_H__
