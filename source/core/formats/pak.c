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
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// id pack magic
rex_byte_c magic_idpak[4] = "PACK";

// id pack header
typedef struct
{
	rex_byte magic[4];
	rex_int ofs_filetable;
	rex_int len_filetable;
} idpak_header_t;

// id pack file table entry
typedef struct
{
	rex_byte filename[56];
	rex_int ofs_file;
	rex_int len_file;
} idpak_file_t;

// Load and process an id Software PAK file. Returns an error code. (Formats/id Software/pak.ksy)
rex_int Rex_Formats_idTech_PAK(rex_int operation, rex_byte *filename)
{
	rex_int num_files, i;
	idpak_header_t pak_header;
	idpak_file_t pak_file;

	FILE *file = fopen(filename, "rb");

	if (file == NULL) return REX_ERROR_FILE_NONE;

	if (!fread(&pak_header, sizeof(pak_header), 1, file)) return REX_ERROR_FILE_READ;
	if (memcmp(pak_header.magic, magic_idpak, 4)) return REX_ERROR_FMT_BAD;

	num_files = pak_header.len_filetable / sizeof(idpak_file_t);

	if (fseek(file, pak_header.ofs_filetable, SEEK_SET)) return REX_ERROR_FMT_MALFORMED;

	for (i = 0; i < num_files; i++)
	{
		if (!fread(&pak_file, sizeof(pak_file), 1, file)) return REX_ERROR_FILE_READ;

		if (operation == REX_FORMATOP_GETINFO)
			Rex_Log("%s file %d: %s", filename, i, pak_file.filename);
	}

	fclose(file);

	return REX_ERROR_NONE;
}
