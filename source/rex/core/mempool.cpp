// ========================================================
//
// FILE:			/source/rex/core/mempool.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Memory Pool implementation
//
// LAST EDITED:		December 16th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// mplite header
#include "thrdprty/mplite/mplite.h"

//
//
// Rex namespace: Memory Pool
//
//

namespace Rex
{

//
//
// Global variables
//
//

// World data
mplite_t mempool_world_data;
rex_int8 *buffer_world_data;
rex_int buffer_world_data_size = 8000000; // 8 mb
rex_int buffer_world_data_min_alloc = 16; // 16 bytes

//
//
// Functions
//
//

void MemPool_Init()
{
	// allocate memory for world data
	buffer_world_data = (rex_int8 *)malloc(buffer_world_data_size);

	// initialize world data memory pool
	mplite_init(&mempool_world_data, buffer_world_data, buffer_world_data_size, buffer_world_data_min_alloc, NULL);
}

} // namespace Rex
