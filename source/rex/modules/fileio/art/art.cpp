// ========================================================
//
// FILE:			/source/rex/modules/fileio/art/art.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		ART namespace: Implementation
//
// LAST EDITED:		December 22nd, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// ART header
#include "art.hpp"

//
//
// ART namespace: Implementation
//
//

namespace ART
{

//
//
// Functions
//
//

bool CreateFileTable(Rex::VFS_Handle *handle)
{
	rex_int32 artversion;
	rex_int32 numtiles;
	rex_int32 localtilestart;
	rex_int32 localtileend;
	Rex::VFS_File file;

	// seek to start of file
	fseek(handle->file_handle, 0L, SEEK_SET);

	// read in header
	fread(&artversion, sizeof(rex_int32), 1, handle->file_handle);
	fread(&numtiles, sizeof(rex_int32), 1, handle->file_handle);
	fread(&localtilestart, sizeof(rex_int32), 1, handle->file_handle);
	fread(&localtileend, sizeof(rex_int32), 1, handle->file_handle);

	// art version is supposed to always be 1
	if (artversion != 1) return false;

	// real number of tiles
	rex_int32 real_numtiles = localtileend - localtilestart + 1;

	// array of sizes
	rex_int16 tilesizx[real_numtiles];
	rex_int16 tilesizy[real_numtiles];
	rex_int32 picanm[real_numtiles];

	// read in arrays
	fread(&tilesizx, sizeof(rex_int16), real_numtiles, handle->file_handle);
	fread(&tilesizy, sizeof(rex_int16), real_numtiles, handle->file_handle);
	fread(&picanm, sizeof(rex_int32), real_numtiles, handle->file_handle);

	for (rex_int i = 0; i < real_numtiles; i++)
	{
		rex_int tilesiz = tilesizx[i] * tilesizy[i];

		file.filename = i;
		file.fileofs = ftell(handle->file_handle);
		file.filesize = tilesiz;

		handle->files.push_back(file);

		cout << file.filename << endl;
		cout << file.fileofs << endl;
		cout << file.filesize << endl;

		fseek(handle->file_handle, tilesiz, SEEK_CUR);
	}

	return true;
}

} // namespace ART
