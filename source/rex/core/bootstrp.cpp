// ========================================================
//
// FILE:			/source/rex/core/bootstrp.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Bootstrap implementation
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Bootstrap
//
//

namespace Rex
{

//
//
// Functions
//
//

// Initialize basic functions of Rex Engine
bool Init()
{
	// Check if engine has already been initialized
	if (engine_context)
	{
		Message("Init()", "Cannot initialize engine, it appears to have already been initialized!", WARNING);
		return false;
	}

	// Create new engine context
	engine_context = new EngineContext;

	// Initialize memory pool
	if (MemPool_Init() == false)
	{
		Message("Init()", "Couldn't initialize memory pools!", FAILURE);
		return false;
	}

	// Initialize logging
	if (Log_Init() == false)
	{
		Message("Init()", "Couldn't initialize logging!", FAILURE);
		return false;
	}

	// Initialize platform-specific handlers
	if (Platform_Init() == false)
	{
		Message("Init()", "Couldn't initialize platform handlers!", FAILURE);
		return false;
	}

	return true;
}

// Shutdown basic functions of Rex Engine
void Quit()
{
	// Shutdown engine context
	if (engine_context) delete engine_context;

	// Shutdown memory pool
	MemPool_Quit();

	// Shutdown VFS
	VFS_Quit();

	// Shutdown logging
	Log_Quit();

	// Shutdown platform-specific handlers
	Platform_Quit();
}

} // namespace Rex
