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
// LAST EDITED:		December 30th, 2022
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

// Private data
mplite_t mempool_private;
rex_int8 *buffer_private;
rex_int buffer_private_min_alloc = 16;
rex_int buffer_private_size = 16384;

// Actor data
mplite_t mempool_actors;
rex_int8 *buffer_actors;
rex_int buffer_actors_min_alloc = 16;
rex_int buffer_actors_size = 16384;

// Renderer data
mplite_t mempool_renderer;
rex_int8 *buffer_renderer;
rex_int buffer_renderer_min_alloc = 16;
rex_int buffer_renderer_size = 16384;

// Surface data
mplite_t mempool_surfaces;
rex_int8 *buffer_surfaces;
rex_int buffer_surfaces_min_alloc = 16;
rex_int buffer_surfaces_size = 16384;

// Application data
mplite_t mempool_application;
rex_int8 *buffer_application;
rex_int buffer_application_min_alloc = 16;
rex_int buffer_application_size = 16384;

//
//
// Functions
//
//

// Initialize memory pools
bool MemPool_Init()
{
	// Private data
	buffer_private = (rex_int8 *)calloc(buffer_private_size, sizeof(rex_int8));
	mplite_init(&mempool_private, buffer_private, buffer_private_size, buffer_private_min_alloc, NULL);
	if (buffer_private == NULL) return false;

	// Actor data
	buffer_actors = (rex_int8 *)calloc(buffer_actors_size, sizeof(rex_int8));
	mplite_init(&mempool_actors, buffer_actors, buffer_actors_size, buffer_actors_min_alloc, NULL);
	if (buffer_actors == NULL) return false;

	// Renderer data
	buffer_renderer = (rex_int8 *)calloc(buffer_renderer_size, sizeof(rex_int8));
	mplite_init(&mempool_renderer, buffer_renderer, buffer_renderer_size, buffer_renderer_min_alloc, NULL);
	if (buffer_renderer == NULL) return false;

	// Surface data
	buffer_surfaces = (rex_int8 *)calloc(buffer_surfaces_size, sizeof(rex_int8));
	mplite_init(&mempool_surfaces, buffer_surfaces, buffer_surfaces_size, buffer_surfaces_min_alloc, NULL);
	if (buffer_surfaces == NULL) return false;

	// Application data
	buffer_application = (rex_int8 *)calloc(buffer_actors_size, sizeof(rex_int8));
	mplite_init(&mempool_application, buffer_application, buffer_application_size, buffer_application_min_alloc, NULL);
	if (buffer_application == NULL) return false;

	return true;
}

// Free memory pools
bool MemPool_Quit()
{
	if (buffer_private) free(buffer_private);
	if (buffer_actors) free(buffer_actors);
	if (buffer_renderer) free(buffer_renderer);
	if (buffer_surfaces) free(buffer_surfaces);
	if (buffer_application) free(buffer_application);

	return true;
}

// Allocate a number of bytes from the specified memory pool
void *MemPool_Alloc(rex_mempool pool, size_t size)
{
	switch (pool)
	{
		case MEMORY_PRIVATE:
			return mplite_malloc(&mempool_private, size);

		case MEMORY_ACTORS:
			return mplite_malloc(&mempool_actors, size);

		case MEMORY_RENDERER:
			return mplite_malloc(&mempool_renderer, size);

		case MEMORY_SURFACES:
			return mplite_malloc(&mempool_surfaces, size);

		case MEMORY_APPLICATION:
			return mplite_malloc(&mempool_application, size);

		default:
			return NULL;
	}
}

// Free the memory allocated from the specified pool
void MemPool_Free(rex_mempool pool, void *memory)
{
	switch (pool)
	{
		case MEMORY_PRIVATE:
			mplite_free(&mempool_private, memory);
			break;

		case MEMORY_ACTORS:
			mplite_free(&mempool_actors, memory);
			break;

		case MEMORY_RENDERER:
			mplite_free(&mempool_renderer, memory);
			break;

		case MEMORY_SURFACES:
			mplite_free(&mempool_surfaces, memory);
			break;

		case MEMORY_APPLICATION:
			mplite_free(&mempool_application, memory);
			break;

		default:
			break;
	}
}

void MemPool_PrintStatus(rex_mempool pool)
{
	switch (pool)
	{
		case MEMORY_PRIVATE:
			mplite_print_stats(&mempool_private, puts);
			break;

		case MEMORY_ACTORS:
			mplite_print_stats(&mempool_actors, puts);
			break;

		case MEMORY_RENDERER:
			mplite_print_stats(&mempool_renderer, puts);
			break;

		case MEMORY_SURFACES:
			mplite_print_stats(&mempool_surfaces, puts);
			break;

		case MEMORY_APPLICATION:
			mplite_print_stats(&mempool_application, puts);
			break;

		default:
			break;
	}
}

} // namespace Rex
