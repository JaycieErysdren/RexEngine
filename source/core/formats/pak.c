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
// LAST EDITED:		October 21st, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// id pack magic
rex_byte_c pak_magic[4] = "PACK";

// id pack header
typedef struct
{
	rex_byte magic[4];
	rex_int ofs_filetable;
	rex_int len_filetable;
} pak_header_t;

// id pack file table entry
typedef struct
{
	rex_byte filename[56];
	rex_int ofs_file;
	rex_int len_file;
} pak_file_t;

// Load and process an id Software PAK file. Returns a resource container. (Formats/id Software/pak.ksy)
rex_resource_container *Rex_Formats_idSoftware_PAK(rex_int operation, rex_byte *filename)
{
	// Variables
	rex_int num_files, i;

	// Pointers
	pak_header_t *pak_header;
	pak_file_t *pak_files;
	FILE *pak_file;
	rex_resource *resources;
	rex_resource_container *resource_container;

	// Open file pointer
	pak_file = Rex_IO_FOpen(filename, "rb");

	// Allocate header
	pak_header = calloc(1, sizeof(pak_header_t));

	// Read in header
	Rex_IO_FRead(pak_header, sizeof(pak_header_t), 1, pak_file);

	// Compare magic
	if (memcmp(pak_header->magic, pak_magic, 4))
		Rex_Failure("Rex_Formats_idSoftware_PAK() Error: %s", Rex_GetError(REX_ERROR_FMT_BAD));

	num_files = pak_header->len_filetable / sizeof(pak_file_t);

	// Seek forward to file table
	Rex_IO_FSeek(pak_file, pak_header->ofs_filetable, SEEK_SET);

	// Allocate file table
	pak_files = calloc(num_files, sizeof(pak_file_t));

	// Read in file table
	Rex_IO_FRead(pak_files, sizeof(pak_file_t), num_files, pak_file);

	// Allocate resources
	resources = calloc(num_files, sizeof(rex_resource));
	resource_container = calloc(1, sizeof(rex_resource_container));

	// Generate resources
	for (i = 0; i < num_files; i++)
	{
		strncpy(resources[i].identiifer, pak_files[i].filename, sizeof(pak_files[i].filename));
		memcpy(resources[i].magic, rex_resource_magic, sizeof(rex_resource_magic));
		resources[i].len_resource_data = pak_files[i].len_file;

		resources[i].resource_data = calloc(pak_files[i].len_file, sizeof(rex_byte));

		Rex_IO_FSeek(pak_file, pak_files[i].ofs_file, SEEK_SET);
		Rex_IO_FRead(resources[i].resource_data, sizeof(rex_byte), pak_files[i].len_file, pak_file);

		// Print debug info
		//Rex_Log("\nfile id: %d\nfile name: %s\nfile offset: %d\nfile size: %d bytes\n==============", i, pak_files[i].filename, pak_files[i].ofs_file, pak_files[i].len_file);
		//Rex_Log("%d - %d bytes - %s", i, resources[i].len_resource_data, resources[i].identiifer);
	}

	strncpy(resource_container->identifier, filename, sizeof(resource_container->identifier));
	memcpy(resource_container->magic, rex_resource_container_magic, sizeof(rex_resource_container_magic));
	resource_container->num_resources = num_files;
	resource_container->resources = resources;

	// Close file pointer
	Rex_IO_FClose(pak_file);

	// Free memory
	free(pak_header);
	free(pak_files);

	return resource_container;
}

// Load and process an id Software PAK file. Returns an error code. (Formats/id Software/pak.ksy)
rex_int Rex_Formats_idTech_PAK(rex_int operation, rex_byte *filename)
{
	rex_int num_files, i;
	pak_header_t pak_header;
	pak_file_t pak_file_entry;

	FILE *file = fopen(filename, "rb");

	if (file == NULL) return REX_ERROR_FILE_NONE;

	if (!fread(&pak_header, sizeof(pak_header), 1, file)) return REX_ERROR_FILE_READ;
	if (memcmp(pak_header.magic, pak_magic, 4)) return REX_ERROR_FMT_BAD;

	num_files = pak_header.len_filetable / sizeof(pak_file_t);

	if (fseek(file, pak_header.ofs_filetable, SEEK_SET)) return REX_ERROR_FMT_MALFORMED;

	for (i = 0; i < num_files; i++)
	{
		if (!fread(&pak_file_entry, sizeof(pak_file_entry), 1, file)) return REX_ERROR_FILE_READ;

		if (operation == REX_FORMATOP_GETINFO)
			Rex_Log("%s file %d: %s", filename, i, pak_file_entry.filename);
	}

	fclose(file);

	return REX_ERROR_NONE;
}
