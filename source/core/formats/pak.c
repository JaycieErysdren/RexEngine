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
// DESCRIPTION:		Loading id Software pak files.
//
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

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

rex_int Rex_Formats_Load_idPakV1(rex_byte *filename)
{
	rex_int num_files, i;
	idpak_header_t pak_header;
	idpak_file_t pak_file;

	FILE *file = fopen(filename, "rb");

	if (file == NULL)
		return REX_ERROR_NOFILE;

	if (!fread(&pak_header, sizeof(pak_header), 1, file)) return REX_ERROR_NOREAD;
	if (memcmp(pak_header.magic, "PACK", 4)) return REX_ERROR_BADFMT;

	num_files = pak_header.len_filetable / sizeof(idpak_file_t);

	if (fseek(file, pak_header.ofs_filetable, SEEK_SET)) return REX_ERROR_MALFORMED;

	for (i = 0; i < num_files; i++)
	{
		if (!fread(&pak_file, sizeof(pak_file), 1, file)) return REX_ERROR_NOREAD;

		SDL_Log("File %d: %s", i, pak_file.filename);
	}

	fclose(file);

	return REX_ERROR_NONE;
}
