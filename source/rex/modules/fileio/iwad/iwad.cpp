// ========================================================
//
// FILE:			/source/rex/modules/fileio/iwad/iwad.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		IWAD namespace: Implementation
//
// LAST EDITED:		December 21st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// IWAD header
#include "iwad.hpp"

//
//
// IWAD namespace: Implementation
//
//

namespace IWAD
{

//
//
// Functions
//
//

bool CreateFileTable(Rex::VFS_Handle *handle)
{
	// variables
	rex_int32 num_lumps;
	rex_int32 ofs_table;
	rex_int i;
	Rex::VFS_File file;
	char filename[8];
	rex_int32 len, ofs;

	// get number of lumps and position of table
	fseek(handle->file_handle, 4L, SEEK_SET);
	fread(&num_lumps, sizeof(rex_int32), 1, handle->file_handle);
	fread(&ofs_table, sizeof(rex_int32), 1, handle->file_handle);

	// seek to lump table
	fseek(handle->file_handle, ofs_table, SEEK_SET);

	// read lump table
	for (i = 0; i < num_lumps; i++)
	{
		memset(filename, '\0', 8);
		fread(&ofs, sizeof(rex_int32), 1, handle->file_handle);
		fread(&len, sizeof(rex_int32), 1, handle->file_handle);
		fread(filename, sizeof(char), 8, handle->file_handle);

		file.filename = filename;
		file.fileofs = ofs;
		file.filesize = len;

		// don't put virtual lumps in the directory
		if (len > 0)
			handle->files.push_back(file);
	}

	return true;
}

} // namespace IWAD
