// ========================================================
//
// FILE:			/source/rex/modules/core/io/bootstrap.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Bootstrap implementation
//
// LAST EDITED:		December 11th, 2022
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
	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_DOS)

	DOS::Initialize();

	#endif

	ConsoleInitialize();

	frame_end_time = GetTicks64();

	return true;
}

// Shutdown basic functions of Rex Engine
bool Shutdown()
{
	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_DOS)

	DOS::Shutdown();

	#endif

	ConsoleShutdown();

	return true;
}

} // namespace Rex
