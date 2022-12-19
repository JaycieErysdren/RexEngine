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


} // namespace Sector
