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
// Endian-swapping (endian.c)
//

// System endian check (hack)
extern const rex_int _rex_endian_check;

// Returns true is system is little endian
#define REX_LITTLE_ENDIAN (*(char *)&_rex_endian_check)

// Returns true if system is big endian
#define REX_BIG_ENDIAN !REX_LITTLE_ENDIAN

// Endian swap unsigned short
rex_ushort Rex_EndianSwap_UShort(rex_ushort val);

// Endian swap signed short
rex_short Rex_EndianSwap_Short(rex_short val);

// Endian swap unsigned int
rex_uint Rex_EndianSwap_UInt(rex_uint val);

// Endian swap signed int
rex_int Rex_EndianSwap_Int(rex_int val);

// Endian swap unsigned long
rex_ulong Rex_EndianSwap_ULong(rex_ulong val);

// Endian swap signed long
rex_long Rex_EndianSwap_Long(rex_long val);

//
// Failure handling (failure.c)
//

// Error codes
enum rex_error_codes
{
	REX_ERROR_NONE,					// No error
	REX_ERROR_FILE_NONE,			// Couldn't find the file specified
	REX_ERROR_FILE_READ,			// Couldn't read the file specified
	REX_ERROR_MEMALLOC,				// Couldn't allocate the specified memory
	REX_ERROR_FMT_MALFORMED,		// The file specified appears malformed
	REX_ERROR_FMT_BAD,				// The specified format was incorrect
	REX_ERROR_FMT_DETERMINE,		// Couldn't determine the format of the specified file
	REX_ERROR_FMT_UNSUPPORTED,		// This format is currently unsupported
};

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
