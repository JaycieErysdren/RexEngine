// ========================================================
//
// FILE:			/source/core/inc/engine/resfile.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the resource file I/O functions.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

extern rex_byte_c rex_resource_container_magic[8];
extern rex_byte_c rex_resource_magic[8];

// Write a resource container and its resources to a file.
void Rex_IO_WriteResourceContainer(rex_byte *filename, rex_resource_container *res);
