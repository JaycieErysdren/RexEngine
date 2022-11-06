// ========================================================
//
// FILE:			/source/core/io/files.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		File I/O functions.
//
// LAST EDITED:		November 6th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Wrapper for fopen() that does automatic error checking.
FILE *Rex_IO_FOpen(rex_byte *filename, rex_byte *modes)
{
	// Open file pointer
	FILE *file = fopen(filename, modes);

	// Check if file exists
	if (file == NULL)
		Rex_Failure("Rex_IO_FOpen() failed with error \"%s\".", strerror(errno));
	else
		return file;
}

// Wrapper for fread() that does automatic error checking.
void Rex_IO_FRead(void *ptr, size_t size, rex_int n, FILE *file)
{
	if (!fread(ptr, size, n, file))
		Rex_Failure("Rex_IO_FRead() failed with error \"%s\".", strerror(errno));
}

// Wrapper for fclose() that does automatic error checking.
void Rex_IO_FClose(FILE *file)
{
	if (fclose(file))
		Rex_Failure("Rex_IO_FClose() failed with error \"%s\".", strerror(errno));
}

// Wrapper for fseek() that does automatic error checking.
void Rex_IO_FSeek(FILE *file, rex_long to, rex_int from)
{
	if (fseek(file, to, from))
		Rex_Failure("Rex_IO_FSeek() failed with error \"%s\".", strerror(errno));
}

// Wrapper for fwrite() that does automatic error checking.
void Rex_IO_FWrite(void *ptr, size_t size, rex_int n, FILE *file)
{
	if (!fwrite(ptr, size, n, file))
		Rex_Failure("Rex_IO_FWrite() failed with error \"%s\".", strerror(errno));
}
