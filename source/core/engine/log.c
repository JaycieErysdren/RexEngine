// ========================================================
//
// FILE:			/source/core/engine/log.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Log and message handling.
//
// LAST EDITED:		November 8th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Display a message box.
void Rex_Message(rex_int message_type, rex_byte *s, ...)
{
	va_list args;
	rex_byte message_buffer[1024];

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
	// Variables
	va_list args;
	rex_byte log_buffer[1024];
	rex_byte time_buffer[22];
	FILE *log_file;

	// Get current time
	time_t time_raw = time(NULL);
	struct tm *time_info = localtime(&time_raw);
	sprintf(time_buffer, "%04d-%02d-%02d %02d-%02d-%02d: ",
		time_info->tm_year + 1900,
		time_info->tm_mon,
		time_info->tm_mday,
		time_info->tm_hour,
		time_info->tm_min,
		time_info->tm_sec
	);

	// Copy time into buffer
	strncpy(log_buffer, time_buffer, sizeof(time_buffer));

	// Print args into string
	va_start(args, s);
	vsprintf(log_buffer + (sizeof(time_buffer) - 1), s, args);
	va_end(args);

	// Add newline
	strcpy(log_buffer + strlen(log_buffer), "\n");
	printf("%d", strlen(log_buffer));

	// Log to a file if filename given
	if (filename != NULL)
	{
		// Try to open it if it already exists
		log_file = Rex_IO_FOpen(filename, "a");

		// If it still doesn't exist, throw an error
		if (log_file == NULL)
			Rex_MakeError("Failed to open log file %s for reading or writing", filename);
		else
			Rex_IO_FWrite(log_buffer, sizeof(rex_byte), strlen(log_buffer), log_file);
		
		Rex_IO_FClose(log_file);
	}

	// Print the message to stdout
	printf("%s", log_buffer);
}
