// ========================================================
//
// FILE:			/source/core/inc/formats/pak.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		id Software PAK types.
//
// LAST EDITED:		October 22nd, 2022
//
// ========================================================

// PAK magic
extern rex_byte_c pak_magic[4];

// PAK header
typedef struct
{
	rex_byte magic[4];
	rex_int ofs_filetable;
	rex_int len_filetable;
} pak_header_t;

// PAK file table entry
typedef struct
{
	rex_byte filename[56];
	rex_int ofs_file;
	rex_int len_file;
} pak_file_t;

// PAK container
typedef struct
{
	pak_header_t *header;
	rex_uint num_files;
	pak_file_t *files;
} pak_t;
