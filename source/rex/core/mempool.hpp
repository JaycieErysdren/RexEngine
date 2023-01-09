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

//
// Custom memset()
//

// Generic
void *MemSet(void *dst, void *src, size_t size, size_t n);

// 8-bit integer
void *MemSet8(void *dst, rex_uint8 src, size_t n);

// 16-bit integer
void *MemSet16(void *dst, rex_uint16 src, size_t n);

// 32-bit integer
void *MemSet32(void *dst, rex_uint32 src, size_t n);

// 64-bit integer
void *MemSet64(void *dst, rex_uint64 src, size_t n);

} // namespace Rex

#endif // __REX_MEMPOOL_H__
