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

// PAK header
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
// Functions
//
//

bool CreateFileTable(Rex::VFS_Handle *handle)
{
	// variables
	rex_int32 ofs_filetable;
	rex_int32 len_filetable;
	rex_int32 num_files;

	// get len and size of filetable
	fseek(handle->file_handle, 4L, SEEK_SET);
	fread(&ofs_filetable, sizeof(rex_int32), 1, handle->file_handle);
	fread(&len_filetable, sizeof(rex_int32), 1, handle->file_handle);

	num_files = len_filetable / 64;

	// seek to file table
	fseek(handle->file_handle, ofs_filetable, SEEK_SET);

	// read in the file table
	for (rex_int i = 0; i < num_files; i++)
	{
		// variables
		Rex::VFS_File file;
		char name[56];

		// read in filename
		fread(&name, sizeof(char), 56, handle->file_handle);
		file.filename = name;

		// read in variables
		fread(&file.fileofs, sizeof(rex_int32), 1, handle->file_handle);
		fread(&file.filesize, sizeof(rex_int32), 1, handle->file_handle);

		handle->files.push_back(file);
	}

	return true;
}

} // namespace PAK
