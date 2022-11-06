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
// LAST EDITED:		November 6th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// PAK magic
const rex_byte pak_magic[4] = "PACK";

// Open an id Software PAK file for reading. Returns a pointer to an open PAK object.
pak_t *PAK_Open(rex_byte *filename)
{
	// Variables
	rex_int i;
	pak_t *pak;
	pak_header_t *pak_header;

	// Allocate memory
	pak = calloc(1, sizeof(pak_t));
	pak->filename = calloc(strlen(filename + 1), sizeof(rex_byte));
	pak_header = calloc(1, sizeof(pak_header_t));

	// Copy filename string
	strcpy(pak->filename, filename);

	// Open file pointer
	pak->file_pointer = Rex_IO_FOpen(filename, "rb");

	// Read in header
	Rex_IO_FRead(pak_header, sizeof(pak_header_t), 1, pak->file_pointer);

	// Compare magic
	if (memcmp(pak_header->magic, pak_magic, 4))
		Rex_Failure("%s has invalid file signature %s", filename, pak_header->magic);

	// Calculate number of files
	pak->num_files = pak_header->len_filetable / sizeof(pak_file_t);

	// Seek forward to file table
	Rex_IO_FSeek(pak->file_pointer, pak_header->ofs_filetable, SEEK_SET);

	// Allocate file table
	pak->file_table = calloc(pak->num_files, sizeof(pak_file_t));

	// Read in file table
	Rex_IO_FRead(pak->file_table, sizeof(pak_file_t), pak->num_files, pak->file_pointer);

	// Return pointer to PAK object
	return pak;
}

// Traverse a currently open PAK file and search for a file by its name.  Returns the length of the file, in bytes.
rex_int PAK_GetFileByFilename(pak_t *pak, rex_byte *filename, void **file_buffer)
{
	// Variables
	rex_int i;
	rex_int len_filename = (rex_int)strlen(filename);

	// Sanity check
	if (len_filename > 56)
		return 0;

	// Loop through the filetable and compare the filenames
	for (i = 0; i < pak->num_files; i++)
	{
		// Check if the whole string matches
		if (!strncmp(filename, pak->file_table[i].filename, len_filename))
		{
			// Allocate memory
			*file_buffer = calloc(1, pak->file_table[i].len_file);

			// Seek to the file offset
			Rex_IO_FSeek(pak->file_pointer, pak->file_table[i].ofs_file, SEEK_SET);

			// Read in file data
			Rex_IO_FRead(*file_buffer, pak->file_table[i].len_file, 1, pak->file_pointer);

			// Return the size of the file
			return pak->file_table[i].len_file;
		}
	}

	return 0;
}

// Traverse a currently open PAK file and search for a file by its index. Returns the length of the file, in bytes.
rex_int PAK_GetFileByIndex(pak_t *pak, rex_int index, void **file_buffer)
{
	// Variables
	void *buffer;

	// Sanity check
	if (index >= pak->num_files)
		Rex_Failure("Index into PAK file table out of range!");

	// Allocate memory
	*file_buffer = calloc(1, pak->file_table[index].len_file);

	// Seek to the file offset
	Rex_IO_FSeek(pak->file_pointer, pak->file_table[index].ofs_file, SEEK_SET);

	// Read in file data
	Rex_IO_FRead(*file_buffer, pak->file_table[index].len_file, 1, pak->file_pointer);

	return pak->file_table[index].len_file;
}

// Close an id Software PAK file and free the associated memory.
void PAK_Close(pak_t *pak)
{
	// Free filename buffer
	free(pak->filename);

	// Close file pointer
	Rex_IO_FClose(pak->file_pointer);

	// Free file table
	if (pak->file_table)
		free(pak->file_table);

	// Free container
	free(pak);
}
