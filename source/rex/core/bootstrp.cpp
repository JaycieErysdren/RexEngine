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
bool Initialize()
{
	// Initialize Memory Pool
	MemPool_Init();

	// Initialize logging
	if (Log_Init() == false) return false;

	// Initialize platform-specific handlers
	return Platform_Init();
}

// Shutdown basic functions of Rex Engine
bool Shutdown()
{
	// Shutdown Memory Pool
	MemPool_Shutdown();

	// Shutdown VFS
	VFS_Shutdown();

	// Shutdown logging
	if (Log_Shutdown() == false) return false;

	// Shutdown platform-specific handlers
	return Platform_Shutdown();
}

} // namespace Rex
