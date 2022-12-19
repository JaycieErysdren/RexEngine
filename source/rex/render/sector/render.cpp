// ========================================================
//
// FILE:			/source/rex/render/sector/sector.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Sector namespace: Render implementation
//
// LAST EDITED:		December 19th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// Sector renderer header
#include "render.hpp"

//
//
// Sector namespace: Render
//
//

namespace Sector
{

//
//
// Functions
//
//

//
// Add / free
//

// Add a SectorModel to memory
SectorModel *AddSectorModel()
{
	void *model_memory = Rex::MemPool_Alloc(Rex::MEMORY_RENDERER, sizeof(SectorModel));
	SectorModel *model = new(model_memory) SectorModel;

	model->memory = model_memory;

	return model;
}

// Free a SectorModel from memory
void FreeSectorModel(SectorModel *model)
{
	if (model == NULL) return;

	void *model_memory = model->memory;

	// Call destructor
	model->~SectorModel();

	// Free the actor
	Rex::MemPool_Free(Rex::MEMORY_RENDERER, model_memory);
}

//
// SectorModel
//

// Add a vertex. Returns the index of the vertex
rex_int SectorModel::AddVertex(rex_scalar x, rex_scalar y)
{
	Vertex vertex;

	vertex.origin.x = x;
	vertex.origin.y = y;

	vertices.push_back(vertex);

	return vertices.size() - 1;
}

// Add a linedef. Returns the index of the linedef
rex_int SectorModel::AddLinedef(rex_uint16 v0, rex_uint16 v1, rex_uint8 color)
{
	Linedef linedef;

	linedef.color = color;
	linedef.v0 = v0;
	linedef.v1 = v1;

	linedefs.push_back(linedef);

	return linedefs.size() - 1;
}

// Add a sector. Returns the index of the sector
rex_int SectorModel::AddSector(rex_scalar height_floor, rex_scalar height_ceiling, rex_uint8 color_floor, rex_uint8 color_ceiling)
{
	Sector sector;

	sector.height_floor = height_floor;
	sector.height_ceiling = height_ceiling;
	sector.color_floor = color_floor;
	sector.color_ceiling = color_ceiling;

	sectors.push_back(sector);

	return sectors.size() - 1;
}

} // namespace Sector
