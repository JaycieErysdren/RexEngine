// ========================================================
//
// FILE:			/source/rex/core/mempool.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Memory Pool header
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

#ifndef __REX_MEMPOOL_H__
#define __REX_MEMPOOL_H__

//
//
// Rex namespace: Memory Pool
//
//

namespace Rex
{

typedef enum
{
	MEMORY_PRIVATE = 1,
	MEMORY_ACTORS,
	MEMORY_RENDERER,
	MEMORY_SURFACES,
	MEMORY_APPLICATION,
} rex_mempool;

// Initialize memory pools
bool MemPool_Init();

// Shutdown memory pools
bool MemPool_Quit();

// Allocate a number of bytes from the specified memory pool
void *MemPool_Alloc(rex_mempool pool, size_t size);

// Free the memory allocated from a pool
void MemPool_Free(rex_mempool pool, void *memory);

// Print status of the specified memory pool
void MemPool_PrintStatus(rex_mempool pool);

} // namespace Rex

#endif // __REX_MEMPOOL_H__
