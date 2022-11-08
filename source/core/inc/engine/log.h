// ========================================================
//
// FILE:			/source/core/inc/engine/log.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the logging and message functions.
//
// LAST EDITED:		November 8th, 2022
//
// ========================================================

// Standard log filename
#define REX_LOG_FILENAME "rex.log"

// Message types.
enum rex_message_types
{
	REX_MESSAGE_NONCRITICAL,
	REX_MESSAGE_WARNING,
	REX_MESSAGE_FAILURE
};

// Display a non-critical message box.
void Rex_Message(rex_int message_type, rex_byte *s, ...);

// Log a non-critical message to the console and a log file.
void Rex_Log(rex_byte *filename, rex_byte *s, ...);
