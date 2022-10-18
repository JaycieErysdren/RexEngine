// ========================================================
//
// FILE:			/source/core/inc/utilities.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for various utilities.
//
// LAST EDITED:		October 18th, 2022
//
// ========================================================

//
// Error codes
//

enum rex_error_codes
{
	REX_ERROR_NONE,			// No error
	REX_ERROR_NOFILE,		// Couldn't find the file specified
	REX_ERROR_NOREAD,		// Couldn't read the file specified
	REX_ERROR_MALFORMED,	// The file specified appears malformed
	REX_ERROR_BADFMT		// The specified format was incorrect
};

//
// Failure handling (failure.c)
//

// Trigger a failure and hard exit the engine.
void Rex_Failure(char *s, ...);

// Return a helper string for a given error code.
rex_byte *Rex_GetError(rex_int errorcode);
