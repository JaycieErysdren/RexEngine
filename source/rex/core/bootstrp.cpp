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
	// Initialize Memory Pool
	if (MemPool_Init() == false) return false;

	// Initialize logging
	if (Log_Init() == false) return false;

	// Initialize platform-specific handlers
	return Platform_Init();
}

// Shutdown basic functions of Rex Engine
bool Quit()
{
	// Shutdown Memory Pool
	if (MemPool_Quit() == false) return false;

	// Shutdown VFS
	VFS_Quit();

	// Shutdown logging
	if (Log_Quit() == false) return false;

	// Shutdown platform-specific handlers
	return Platform_Quit();
}

} // namespace Rex
