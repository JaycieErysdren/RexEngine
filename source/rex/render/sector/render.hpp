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

		// The vertices that makes up this linedef
		vector<Vertex *> vertices;

		// The sectors that makes up this linedef portal
		vector<Sector *> sectors;

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

		// Array of linedefs that make up this sector
		vector<Linedef *> linedefs;
};

// SectorModel class
class SectorModel
{
	public:

		//
		// Variables
		//

		vector<Vertex *> vertices;
		vector<Linedef *> linedefs;
		vector<Sector *> sectors;

		//
		// Danger zone
		//

		void *memory;

		//
		// Functions
		//

		Vertex *AddVertex(rex_scalar x, rex_scalar y);
		Linedef *AddLinedef(Vertex *v0, Vertex *v1, rex_uint8 color);
};

//
//
// Functions
//
//



} // namespace Sector

#endif // __SECTOR_RENDER_H__
