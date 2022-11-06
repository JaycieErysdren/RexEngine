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
// LAST EDITED:		November 6th, 2022
//
// ========================================================

//
// Format Reference: Formats/id Software/pak.ksy
//

//
// PAK types
//

// PAK magic
extern const rex_byte pak_magic[4];

// PAK header
typedef struct
{
	rex_byte magic[4];
	rex_int ofs_filetable;
	rex_int len_filetable;
} pak_header_t;

// PAK file
typedef struct
{
	rex_byte filename[56];
	rex_int ofs_file;
	rex_int len_file;
} pak_file_t;

// PAK container
typedef struct
{
	rex_byte *filename;
	FILE *file_pointer;
	rex_uint num_files;
	pak_file_t *file_table;
} pak_t;

//
// PAK functions
//

// Open an id Software PAK file for reading. Returns a pointer to an open PAK object.
pak_t *PAK_Open(rex_byte *filename);

// Traverse a currently open PAK file and search for a file by its name. Returns a pointer to the file's data.
rex_int PAK_GetFileByFilename(pak_t *pak, rex_byte *filename, void **file_buffer);

// Traverse a currently open PAK file and search for a file by its index. Returns a pointer to the file's data.
rex_int PAK_GetFileByIndex(pak_t *pak, rex_int index, void **file_buffer);

// Close an id Software PAK file and free the associated memory.
void PAK_Close(pak_t *pak);
