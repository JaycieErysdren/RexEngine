// ========================================================
//
// FILE:			/source/core/inc/engine/file_io.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the file I/O functions.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Wrapper for fopen() that does automatic error reporting.
FILE *Rex_IO_FOpen(rex_byte *filename, rex_byte *modes);

// Wrapper for fread() that does automatic error reporting.
void Rex_IO_FRead(void *ptr, size_t size, rex_int n, FILE *file);

// Wrapper for fclose() that does automatic error reporting.
void Rex_IO_FClose(FILE *file);

// Wrapper for fseek() that does automatic error reporting.
void Rex_IO_FSeek(FILE *file, rex_long to, rex_int from);

// Wrapper for fwrite() that does automatic error reporting.
void Rex_IO_FWrite(void *ptr, size_t size, rex_int n, FILE *file);
