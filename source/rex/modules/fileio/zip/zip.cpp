// ========================================================
//
// FILE:			/source/rex/modules/fileio/zip/zip.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		ZIP namespace: Implementation
//
// LAST EDITED:		December 21st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// ZIP header
#include "zip.hpp"

//
//
// ZIP namespace: Implementation
//
//

namespace ZIP
{

//
// ZIP structure
//

typedef struct
{
	char magic[4]						ATTR_PACKED;
	rex_int16 version					ATTR_PACKED;
	rex_int16 bitflag					ATTR_PACKED;
	rex_int16 compression_method		ATTR_PACKED;
	rex_int16 lastmod_time				ATTR_PACKED;
	rex_int16 lastmod_date				ATTR_PACKED;
	rex_int32 crc32						ATTR_PACKED;
	rex_int32 compressed_size			ATTR_PACKED;
	rex_int32 uncompressed_size			ATTR_PACKED;
	rex_int16 len_filename				ATTR_PACKED;
	rex_int16 len_extrafield			ATTR_PACKED;
} zip_local_file_header_t;

//
//
// Functions
//
//

bool CreateFileTable(Rex::VFS_Handle *handle)
{
	// allocate zip header struct
	zip_local_file_header_t *zip = (zip_local_file_header_t *)calloc(1, sizeof(zip_local_file_header_t));

	// go back to start of file
	fseek(handle->file_handle, 0L, SEEK_SET);

	char filename[256];
	Rex::VFS_File file;

	while (fread(zip, sizeof(zip_local_file_header_t), 1, handle->file_handle))
	{
		// compare header
		if (memcmp(zip->magic, "PK", 2) != 0) continue;

		// if its compressed or a directory, skip
		if (zip->compression_method != 0 || zip->uncompressed_size < 1 || zip->uncompressed_size != zip->compressed_size)
		{
			fseek(handle->file_handle, zip->compressed_size + zip->len_filename + zip->len_extrafield, SEEK_CUR);
			continue;
		}

		// wipe filename buff
		memset(filename, '\0', 256);

		// read in filename
		fread(filename, sizeof(char), zip->len_filename, handle->file_handle);
		fseek(handle->file_handle, zip->len_extrafield, SEEK_CUR);

		file.filename = filename;
		file.fileofs = ftell(handle->file_handle);
		file.filesize = zip->uncompressed_size;

		handle->files.push_back(file);

		fseek(handle->file_handle, zip->uncompressed_size, SEEK_CUR);
	}

	free(zip);

	return true;
}

} // namespace ZIP
