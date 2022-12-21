// ========================================================
//
// FILE:			/source/rex/modules/fileio/pak/pak.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		PAK namespace: Implementation
//
// LAST EDITED:		December 21st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// PAK renderer header
#include "pak.hpp"

//
//
// PAK namespace: Implementation
//
//

namespace PAK
{

//
//
// Globals
//
//

const char pak_magic[5] = "PACK";

//
//
// Functions
//
//

//
// Handle class
//

// Open a PAK handle
bool Handle::Open(string filename, handle_type handle_type)
{
	if (filename.empty()) return false;

	switch (handle_type)
	{
		// user is reading from a PAK file.
		// if the handle is NULL, return false;
		case READ:
			file_handle = fopen(filename.c_str(), "rb");
			if (file_handle == NULL) return false;
			break;

		// user is writing to a PAK file.
		// this encompasses writing a new one, as well as editing an existing one
		case WRITE:
			file_handle = fopen(filename.c_str(), "wb");
			break;

		default:
			return false;
	}

	if (file_handle == NULL)
	{
		rex_int32 zero = 0;
		fwrite(pak_magic, sizeof(char), 4, file_handle); // write pak magic
		fwrite(&zero, sizeof(rex_int32), 1, file_handle); // write filetable offset
		fwrite(&zero, sizeof(rex_int32), 1, file_handle); // write filetable len

		return true;
	}

	// Check file magic
	char magic[4];
	fread(magic, sizeof(char), 4, file_handle);
	if (memcmp(magic, pak_magic, 4) != 0) return false;

	// Get filetable offset
	rex_int32 ofs_filetable;
	rex_int32 len_filetable;
	fread(&ofs_filetable, sizeof(rex_int32), 1, file_handle);
	fread(&len_filetable, sizeof(rex_int32), 1, file_handle);
	if (ofs_filetable < 0 || len_filetable < 1) return false;

	// The number of files in this PAK
	rex_int num_files = len_filetable / 64; // 64 is the size of a filetable entry

	// Read in the file table
	fseek(file_handle, ofs_filetable, SEEK_SET);

	for (rex_int i = 0; i < num_files; i++)
	{
		// variables
		FileInfo file;
		char fname[56];

		// read in filename
		fread(&fname, sizeof(char), 56, file_handle);
		file.filename = fname;

		// read in variables
		fread(&file.file_offset, sizeof(rex_int32), 1, file_handle);
		fread(&file.file_len, sizeof(rex_int32), 1, file_handle);

		files.push_back(file);
	}

	return true;
}

// Close a PAK handle
void Handle::Close()
{
	if (file_handle)
		fclose(file_handle);
}

// Get the file of the data in a memory-allocated buffer. Free it with free().
bool Handle::GetFile(string filename, void **buffer, rex_int *buffer_len)
{
	rex_int i;

	// if pointer is null, fail
	if (buffer == NULL) return false;

	for (i = 0; i < files.size(); i++)
	{
		if (filename.compare(files[i].filename) == 0)
		{
			// get the length of the file
			*buffer_len = files[i].file_len;

			// alloc buffer
			*buffer = calloc(*buffer_len, sizeof(rex_uint8));

			fseek(file_handle, files[i].file_offset, SEEK_SET);

			fread(*buffer, sizeof(rex_uint8), *buffer_len, file_handle);

			return true;
		}
	}

	// failed
	buffer = NULL;
	return false;
}

// Add a file to the PAK
bool Handle::AddFile(string filename, void *buffer, rex_int buffer_len)
{
	if (type != WRITE) return false;

	return true;
}

} // namespace PAK
