// ========================================================
//
// FILE:			/source/rex/modules/fileio/wad/wad.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		WAD namespace: Implementation
//
// LAST EDITED:		December 21st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// WAD header
#include "wad.hpp"

//
//
// WAD namespace: Implementation
//
//

namespace WAD
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
	char filename[16];
	rex_int32 len, ofs;

	// get number of lumps and position of table
	fseek(handle->file_handle, 4L, SEEK_SET);
	fread(&num_lumps, sizeof(rex_int32), 1, handle->file_handle);
	fread(&ofs_table, sizeof(rex_int32), 1, handle->file_handle);

	// seek to lump table
	fseek(handle->file_handle, ofs_table, SEEK_SET);

	// read lump table
	if (handle->format == Rex::VFS_FORMAT_PWAD || handle->format == Rex::VFS_FORMAT_IWAD)
	{
		// DOOM WADs
		for (i = 0; i < num_lumps; i++)
		{
			memset(filename, '\0', 16);
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
	}
	else if (handle->format == Rex::VFS_FORMAT_WAD2 || handle->format == Rex::VFS_FORMAT_WAD3)
	{
		// Quake & Half-Life WADs
		for (i = 0; i < num_lumps; i++)
		{
			memset(filename, '\0', 16);

			// offset and len
			fread(&ofs, sizeof(rex_int32), 1, handle->file_handle);
			fread(&len, sizeof(rex_int32), 1, handle->file_handle);

			// filename
			fseek(handle->file_handle, 8L, SEEK_CUR);
			fread(filename, sizeof(char), 16, handle->file_handle);

			file.filename = filename;
			file.fileofs = ofs;
			file.filesize = len;

			// don't put virtual lumps in the directory
			if (len > 0)
				handle->files.push_back(file);
		}
	}
	else
	{
		// fail
		return false;
	}

	return true;
}

} // namespace WAD
