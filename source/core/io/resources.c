// ========================================================
//
// FILE:			/source/core/io/resources.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Resource file I/O functions.
//
// LAST EDITED:		October 21st, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Resource file magics
rex_byte_c rex_resource_container_magic[8] = "REXRESFL";
rex_byte_c rex_resource_magic[8] = "REXRESRC";

// Write a resource container and its resources to a file.
void Rex_IO_WriteResourceContainer(rex_byte *filename, rex_resource_container *res)
{
	// Variables
	rex_int i;

	// Open file pointer
	FILE *resfile = Rex_IO_FOpen(filename, "wb");

	// Write header
	Rex_IO_FWrite(res, offsetof(rex_resource_container, resources), 1, resfile);

	for (i = 0; i < res->num_resources; i++)
	{
		Rex_IO_FWrite(res->resources[i].magic, sizeof(res->resources[i].magic), 1, resfile);
		Rex_IO_FWrite(res->resources[i].identiifer, sizeof(res->resources[i].identiifer), 1, resfile);
		Rex_IO_FWrite(&res->resources[i].format, sizeof(rex_uint), 1, resfile);
		Rex_IO_FWrite(&res->resources[i].len_resource_data, sizeof(rex_uint), 1, resfile);
		Rex_IO_FWrite(res->resources[i].resource_data, sizeof(rex_byte), res->resources[i].len_resource_data, resfile);
	}

	Rex_IO_FClose(resfile);
}
