// ========================================================
//
// FILE:			/source/rex/render/sector/render.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Sector namespace: Render header
//
// LAST EDITED:		December 19th, 2022
//
// ========================================================

#ifndef __SECTOR_RENDER_H__
#define __SECTOR_RENDER_H__

//
//
// Sector namespace: Render
//
//

namespace Sector
{

//
//
// Classes
//
//

// forward declarations
class Linedef;
class Sector;

// Vertex class
class Vertex
{
	public:

		//
		// Variables
		//

		rex_vec2s origin;
};

// Linedef class
class Linedef
{
	public:

		//
		// Variables
		//

		// The vertex IDs that makes up this linedef
		rex_uint16 v0;
		rex_uint16 v1;

		// The sector IDs that makes up this linedef portal
		rex_uint16 s0;
		rex_uint16 s1;

		// The color of this wall
		rex_uint8 color;
};

// Sector class
class Sector
{
	public:

		//
		// Variables
		//

		// Floor height and ceiling height
		rex_scalar height_floor;
		rex_scalar height_ceiling;

		// Floor color and ceiling color
		rex_uint8 color_floor;
		rex_uint8 color_ceiling;

		// The linedef IDs that make up this sector
		vector<rex_uint16> linedefs;
};

// SectorModel class
class SectorModel
{
	public:

		//
		// Variables
		//

		vector<Vertex> vertices;
		vector<Linedef> linedefs;
		vector<Sector> sectors;

		//
		// Danger zone
		//

		void *memory;

		//
		// Functions
		//

		rex_int AddVertex(rex_scalar x, rex_scalar y);
		rex_int AddLinedef(rex_uint16 v0, rex_uint16 v1, rex_uint8 color);
		rex_int AddSector(rex_scalar height_floor, rex_scalar height_ceiling, rex_uint8 color_floor, rex_uint8 color_ceiling);
};

//
//
// Functions
//
//



} // namespace Sector

#endif // __SECTOR_RENDER_H__
