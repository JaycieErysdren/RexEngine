// ========================================================
//
// FILE:			/source/core/formats/wal.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		id Software WAL files.
//
// LAST EDITED:		October 22nd, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Load an id Software WAL file into memory. Returns a pointer to a WAL container.
wal_t *WAL_Load(rex_byte *filename)
{
	// Define variables
	rex_int i;
	wal_t *wal;
	FILE *file;

	// Open file pointer
	file = Rex_IO_FOpen(filename, "rb");

	// Allocate memory
	wal = calloc(1, sizeof(wal_t));
	wal->header = calloc(1, sizeof(wal_header_t));

	// Read in header
	Rex_IO_FRead(wal->header, sizeof(wal_header_t), 1, file);

	// Calculate mipmap widths
	wal->widths[0] = wal->header->width;
	wal->widths[1] = wal->widths[0] / 2;
	wal->widths[2] = wal->widths[1] / 2;
	wal->widths[3] = wal->widths[2] / 2;

	// Calculate mipmap heights
	wal->heights[0] = wal->header->height;
	wal->heights[1] = wal->heights[0] / 2;
	wal->heights[2] = wal->heights[1] / 2;
	wal->heights[3] = wal->heights[2] / 2;

	// Read in mipmap pixels
	for (i = 0; i < 4; i++)
	{
		// Allocate pixel memory
		wal->pixels[i] = calloc(wal->widths[i] * wal->heights[i], sizeof(rex_ubyte));

		// Seek to file offset
		Rex_IO_FSeek(file, wal->header->mipmap_offsets[i], SEEK_SET);

		// Read in pixels
		Rex_IO_FRead(wal->pixels[i], sizeof(rex_ubyte), wal->widths[i] * wal->heights[i], file);
	}

	// Close file pointer
	Rex_IO_FClose(file);

	// Return pointer to WAL container
	return wal;
}

// Free an id Software WAL file from memory.
void WAL_Free(wal_t *wal)
{
	rex_int i;

	// Free pixel buffers
	for (i = 0; i < 4; i++)
	{
		free(wal->pixels[i]);
	}

	// Free container
	free(wal);
}
