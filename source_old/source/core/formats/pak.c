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
// LAST EDITED:		November 8th, 2022
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

	// Sanity check
	if (strlen(filename) < 1 || filename == NULL)
	{
		Rex_MakeError("Provided filename string is 0 characters or NULL");
		return NULL;
	}

	// Allocate memory
	pak = calloc(1, sizeof(pak_t));
	pak->filename = calloc(strlen(filename + 1), sizeof(rex_byte));
	pak_header = calloc(1, sizeof(pak_header_t));

	// Copy filename string
	strcpy(pak->filename, filename);

	// Open file pointer
	pak->file_pointer = Rex_IO_FOpen(filename, "rb");

	// Check if file exists
	if (pak->file_pointer == NULL)
	{
		PAK_Close(pak);
		return NULL;
	}

	// Read in header
	Rex_IO_FRead(pak_header, sizeof(pak_header_t), 1, pak->file_pointer);

	// Compare magic
	if (memcmp(pak_header->magic, pak_magic, 4))
	{
		PAK_Close(pak);
		Rex_MakeError("Provided file \"%s\" has invalid file signature \"%s\"", filename, pak_header->magic);
		return NULL;
	}

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

	// Sanity check 1
	if (len_filename < 1 || filename == NULL)
	{
		Rex_MakeError("The provided filename is zero characters or NULL");
		*file_buffer = NULL;
		return 0;
	}

	// Sanity check 2
	if (pak == NULL)
	{
		Rex_MakeError("PAK object does not exist");
		*file_buffer = NULL;
		return 0;
	}

	// Sanity check 3
	if (len_filename > 56)
	{
		Rex_MakeError("The provided filename is longer than the allowed filenames in the PAK format");
		*file_buffer = NULL;
		return 0;
	}

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

	// Couldn't find the file
	Rex_MakeError("PAK file '%s' does not appear to contain '%s'", pak->filename, filename);
	*file_buffer = NULL;
	return 0;
}

// Traverse a currently open PAK file and search for a file by its index. Returns the length of the file, in bytes.
rex_int PAK_GetFileByIndex(pak_t *pak, rex_int index, void **file_buffer)
{
	// Variables
	void *buffer;

	// Sanity check
	if (index >= pak->num_files)
		Rex_MakeError("Index into %s file table out of range!", pak->filename);

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
	// Sanity check
	if (pak == NULL)
		return;

	// Free filename buffer
	if (pak->filename)
		free(pak->filename);

	// Close file pointer
	if (pak->file_pointer)
		Rex_IO_FClose(pak->file_pointer);

	// Free file table
	if (pak->file_table)
		free(pak->file_table);

	// Free container
	if (pak)
		free(pak);
}
