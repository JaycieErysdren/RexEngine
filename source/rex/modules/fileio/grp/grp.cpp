// ========================================================
//
// FILE:			/source/rex/modules/fileio/grp/grp.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		GRP namespace: Implementation
//
// LAST EDITED:		December 21st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// GRP header
#include "grp.hpp"

//
//
// GRP namespace: Implementation
//
//

namespace GRP
{

//
//
// Functions
//
//

bool CreateFileTable(Rex::VFS_Handle *handle)
{
	// variables
	rex_int i;
	Rex::VFS_File file;

	// get number of files
	fseek(handle->file_handle, 12L, SEEK_SET);
	rex_uint32 num_files;
	fread(&num_files, sizeof(rex_uint32), 1, handle->file_handle);

	// read in file table
	for (i = 0; i < num_files; i++)
	{
		// variables
		char filename[12];
		rex_uint32 len_file;

		// read filename
		memset(filename, '0', 12);
		fread(filename, sizeof(char), 12, handle->file_handle);
		fread(&len_file, sizeof(rex_uint32), 1, handle->file_handle);

		file.filename = filename;
		file.filesize = len_file;

		handle->files.push_back(file);
	}

	// read in file offsets
	for (i = 0; i < num_files; i++)
	{
		handle->files[i].fileofs = ftell(handle->file_handle);
		fseek(handle->file_handle, handle->files[i].filesize, SEEK_CUR);
	}

	return true;
}

} // namespace GRP
