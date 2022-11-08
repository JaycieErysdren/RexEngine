// ========================================================
//
// FILE:			/source/core/utilities/error.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Error handling.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Global error text buffer. Use Rex_GetError() to access this.
rex_byte _rex_error_buffer[512];

// Display a message box.
void Rex_Message(rex_int message_type, rex_byte *s, ...)
{
	va_list args;
	rex_byte message_buffer[512];

	va_start(args, s);
	vsprintf(message_buffer, s, args);
	va_end(args);

	switch(message_type)
	{
		case REX_MESSAGE_WARNING:
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning", message_buffer, NULL);
			break;

		case REX_MESSAGE_FAILURE:
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failure", message_buffer, NULL);
			break;

		case REX_MESSAGE_NONCRITICAL:
		default: 
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Message", message_buffer, NULL);
			break;
	}

	printf("%s\n", message_buffer);
}

// Log a non-critical message to the console and a log file. Pass NULL as the filename to skip file logging.
void Rex_Log(rex_byte *filename, rex_byte *s, ...)
{
	va_list args;
	rex_byte log_buffer[512];
	FILE *log_file;

	va_start(args, s);
	vsprintf(log_buffer, s, args);
	va_end(args);

	// Log to a file if filename given
	if (filename != NULL)
	{
		// Try to open it if it already exists
		log_file = Rex_IO_FOpen(filename, "a");

		// If it doesn't exist, try to make a new one
		if (log_file == NULL)
		{
			log_file = Rex_IO_FOpen(filename, "w");

			if (log_file == NULL)
			{
				Rex_MakeError("Failed to open log file %s for reading or writing", filename);
			}
			else
			{
				Rex_IO_FWrite(log_buffer, sizeof(rex_byte), strlen(log_buffer), log_file);
			}
		}
	}

	// Print the message to stdout
	printf("%s\n", log_buffer);
}

// Generate an error message that can be caught with Rex_GetError()
void Rex_MakeError(rex_byte *s, ...)
{
	va_list args;

	// Null it out
	snprintf(_rex_error_buffer, sizeof(_rex_error_buffer), '\0');

	// Copy args into it
	va_start(args, s);
	vsprintf(_rex_error_buffer, s, args);
	va_end(args);
}

// Catches and returns an error generated with Rex_MakeError()
rex_byte *Rex_GetError(void)
{
	// Just return the error.
	return _rex_error_buffer;
}
