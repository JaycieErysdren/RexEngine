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

// Error codes
enum rex_error_codes
{
	REX_ERROR_NONE,					// No error
	REX_ERROR_FILE_NONE,			// Couldn't find the file specified
	REX_ERROR_FILE_READ,			// Couldn't read the file specified
	REX_ERROR_FMT_MALFORMED,		// The file specified appears malformed
	REX_ERROR_FMT_BAD,				// The specified format was incorrect
	REX_ERROR_FMT_DETERMINE,		// Couldn't determine the format of the specified file
};

//
// Failure handling (failure.c)
//

// Trigger a failure and hard exit the engine.
void Rex_Failure(char *s, ...);

// Display a warning message.
void Rex_Warning(char *s, ...);

// Display a non-critical message.
void Rex_Message(char *s, ...);

// Display a non-critical log message.
void Rex_Log(char *s, ...);

// Return a helper string for a given error code.
rex_byte *Rex_GetError(rex_int errorcode);
