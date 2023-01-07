// ========================================================
//
// FILE:			/source/rex/core/context.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Context implementation
//
// LAST EDITED:		January 6th, 2023
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Context
//
//

namespace Rex
{

//
//
// Globals
//
//

// Engine context
EngineContext *engine_context;

//
//
// Functions
//
//

//
// EngineContext class
//

// Constructor
EngineContext::EngineContext()
{
	// Set start time
	this->time_start = time(NULL);

	// Set default message handler
	this->MessageHandler = Platform_MessageHandler;
}

// Destructor
EngineContext::~EngineContext()
{
	// Set end time
	this->time_end = time(NULL);
}

} // namespace Rex
