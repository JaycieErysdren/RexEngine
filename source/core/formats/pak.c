// ========================================================
//
// FILE:			/source/core/formats/pak.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		id Software PAK files.
//
// LAST EDITED:		October 23rd, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// PAK magic
rex_byte_c pak_magic[4] = "PACK";

// Load an id Software PAK file into memory. Returns a pointer to a PAK container.
pak_t *PAK_Load(rex_byte *filename)
{
	// Variables
	rex_int i;
	pak_t *pak;
	FILE *file;

	// Open file pointer
	file = Rex_IO_FOpen(filename, "rb");

	// Allocate memory
	pak = calloc(1, sizeof(pak_t));
	pak->header = calloc(1, sizeof(pak_header_t));

	// Read in header
	Rex_IO_FRead(pak->header, sizeof(pak_header_t), 1, file);

	// Compare magic
	if (memcmp(pak->header->magic, pak_magic, 4))
		Rex_Failure("%s has invalid file signature %s", filename, pak->header->magic);

	// Calculate number of files
	pak->num_files = pak->header->len_filetable / sizeof(pak_file_t);

	// Seek forward to file table
	Rex_IO_FSeek(file, pak->header->ofs_filetable, SEEK_SET);

	// Allocate file table
	pak->files = calloc(pak->num_files, sizeof(pak_file_t));

	// Read in file table
	Rex_IO_FRead(pak->files, sizeof(pak_file_t), pak->num_files, file);

	// Close file pointer
	Rex_IO_FClose(file);

	// Return pointer to PAK container
	return pak;
}

// Free an id Software PAK file from memory.
void PAK_Free(pak_t *pak)
{
	// Free header
	free(pak->header);

	// Free file table
	free(pak->files);

	// Free container
	free(pak);
}
