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

// Add a vertex at the specified index
void SectorModel::AddVertexAtIndex(rex_int i, rex_scalar x, rex_scalar y)
{
	// Resize vertices struct to fit
	if (i >= vertices.size()) vertices.resize(i);

	// Add vertex positions to it
	vertices[i].origin.x = x;
	vertices[i].origin.y = y;
}

// Add a linedef at the specified index
void SectorModel::AddLinedefAtIndex(rex_int i, rex_uint16 v0, rex_uint16 v1, rex_uint8 color)
{
	// Resize linedefs struct to fit
	if (i >= linedefs.size()) linedefs.resize(i + 1);

	// Add linedef data to it
	linedefs[i].color = color;
	linedefs[i].v0 = v0;
	linedefs[i].v1 = v1;
}

} // namespace Sector
