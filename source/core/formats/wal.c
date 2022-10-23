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
// LAST EDITED:		October 23rd, 2022
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
	wal->widths[1] = wal->header->width / 2;
	wal->widths[2] = wal->header->width / 4;
	wal->widths[3] = wal->header->width / 8;

	// Calculate mipmap heights
	wal->heights[0] = wal->header->height;
	wal->heights[1] = wal->header->height / 2;
	wal->heights[2] = wal->header->height / 4;
	wal->heights[3] = wal->header->height / 8;

	// Read in mipmap pixels
	for (i = 0; i < 4; i++)
	{
		// Allocate pixel memory
		wal->mipmaps[i] = calloc(wal->widths[i] * wal->heights[i], sizeof(rex_ubyte));

		// Seek to file offset
		Rex_IO_FSeek(file, wal->header->mipmap_offsets[i], SEEK_SET);

		// Read in pixels
		Rex_IO_FRead(wal->mipmaps[i], sizeof(rex_ubyte), wal->widths[i] * wal->heights[i], file);
	}

	// Close file pointer
	Rex_IO_FClose(file);

	// Return pointer to WAL container
	return wal;
}

// Free an id Software WAL object from memory.
void WAL_Free(wal_t *wal)
{
	rex_int i;

	// Free pixel buffers
	for (i = 0; i < 4; i++)
	{
		free(wal->mipmaps[i]);
	}

	// Free container
	free(wal);
}

// Save an id Software WAL object to a file.
void WAL_Save(rex_byte *filename, wal_t *wal)
{
	// Define variables
	rex_int i, mipmap_sizes[4];
	FILE *file;

	// Open file pointer
	file = Rex_IO_FOpen(filename, "wb");

	// Calculate mipmap sizes
	mipmap_sizes[0] = wal->widths[0] * wal->heights[0];
	mipmap_sizes[1] = wal->widths[1] * wal->heights[1];
	mipmap_sizes[2] = wal->widths[2] * wal->heights[2];
	mipmap_sizes[3] = wal->widths[3] * wal->heights[3];

	// Recalculate mipmap offsets, just to be safe
	wal->header->mipmap_offsets[0] = sizeof(wal_header_t);
	wal->header->mipmap_offsets[1] = wal->header->mipmap_offsets[0] + mipmap_sizes[0];
	wal->header->mipmap_offsets[2] = wal->header->mipmap_offsets[1] + mipmap_sizes[1];
	wal->header->mipmap_offsets[3] = wal->header->mipmap_offsets[2] + mipmap_sizes[2];

	// Save header
	Rex_IO_FWrite(wal->header, sizeof(wal_header_t), 1, file);

	// Save mipmaps
	Rex_IO_FWrite(wal->mipmaps[0], sizeof(rex_byte), mipmap_sizes[0], file);
	Rex_IO_FWrite(wal->mipmaps[1], sizeof(rex_byte), mipmap_sizes[1], file);
	Rex_IO_FWrite(wal->mipmaps[2], sizeof(rex_byte), mipmap_sizes[2], file);
	Rex_IO_FWrite(wal->mipmaps[3], sizeof(rex_byte), mipmap_sizes[3], file);

	// Close file pointer
	Rex_IO_FClose(file);
}

// Allocate an id Software WAL object.
wal_t *WAL_Allocate(rex_byte *name, rex_byte *next_name, rex_uint width, rex_uint height, rex_uint flags, rex_uint contents, rex_uint value)
{
	// Define variables
	rex_int mipmap_sizes[4];
	wal_t *wal;

	// Allocate memory
	wal = calloc(1, sizeof(wal_t));
	wal->header = calloc(1, sizeof(wal_header_t));

	// Assign saved fields
	strncpy(wal->header->name, name, sizeof(wal->header->name));
	strncpy(wal->header->next_name, next_name, sizeof(wal->header->next_name));
	wal->header->width = width;
	wal->header->height = height;
	wal->header->flags = flags;
	wal->header->contents = contents;
	wal->header->value = value;

	// Calculate mipmap widths
	wal->widths[0] = width;
	wal->widths[1] = width / 2;
	wal->widths[2] = width / 4;
	wal->widths[3] = width / 8;

	// Calculate mipmap heights
	wal->heights[0] = height;
	wal->heights[1] = height / 2;
	wal->heights[2] = height / 4;
	wal->heights[3] = height / 8;

	// Calculate mipmap sizes
	mipmap_sizes[0] = wal->widths[0] * wal->heights[0];
	mipmap_sizes[1] = wal->widths[1] * wal->heights[1];
	mipmap_sizes[2] = wal->widths[2] * wal->heights[2];
	mipmap_sizes[3] = wal->widths[3] * wal->heights[3];

	// Allocate mipmap buffers
	wal->mipmaps[0] = calloc(mipmap_sizes[0], sizeof(rex_ubyte));
	wal->mipmaps[1] = calloc(mipmap_sizes[1], sizeof(rex_ubyte));
	wal->mipmaps[2] = calloc(mipmap_sizes[2], sizeof(rex_ubyte));
	wal->mipmaps[3] = calloc(mipmap_sizes[3], sizeof(rex_ubyte));

	// Return pointer to WAL object
	return wal;
}
