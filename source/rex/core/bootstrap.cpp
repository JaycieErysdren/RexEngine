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
// LAST EDITED:		December 18th, 2022
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

// blegh
void *math_table_memory;

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

	MemPool_Init();

	// allocate math table
	math_table_memory = MemPool_Alloc(MEMORY_PRIVATE, sizeof(MathTable));
	math_table = new(math_table_memory)MathTable;

	return true;
}

// Shutdown basic functions of Rex Engine
bool Shutdown()
{
	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_DOS)

	DOS::Shutdown();

	#endif

	ConsoleShutdown();

	// shutdown math table
	math_table->~MathTable();
	MemPool_Free(MEMORY_PRIVATE, math_table_memory);

	return true;
}

} // namespace Rex
