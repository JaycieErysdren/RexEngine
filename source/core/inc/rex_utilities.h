// ========================================================
//
// FILE:			/source/core/inc/rex_utilities.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for various utilities.
//
// LAST EDITED:		November 7th, 2022
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

// Endian swap signed short
void Rex_EndianSwap_Short(rex_short *val);

// Endian swap unsigned short
void Rex_EndianSwap_UShort(rex_ushort *val);

// Endian swap signed int
void Rex_EndianSwap_Int(rex_int *val);

// Endian swap unsigned int
void Rex_EndianSwap_UInt(rex_uint *val);

// Endian swap signed long
void Rex_EndianSwap_Long(rex_long *val);

// Endian swap unsigned long
void Rex_EndianSwap_ULong(rex_ulong *val);

//
// Error handling (error.c)
//

// MessageBox types.
enum rex_messagebox_types
{
	REX_MESSAGE_NONCRITICAL,
	REX_MESSAGE_WARNING,
	REX_MESSAGE_FAILURE
};

// Display a non-critical message box.
void Rex_Message(rex_int message_type, rex_byte *s, ...);

// Log a non-critical message to the console and a log file.
void Rex_Log(rex_byte *filename, rex_byte *s, ...);

// Generate an error message that can be caught with Rex_GetError().
void Rex_MakeError(rex_byte *s, ...);

// Catches and returns an error generated with Rex_MakeError().
rex_byte *Rex_GetError(void);
