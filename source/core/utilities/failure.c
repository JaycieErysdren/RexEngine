// ========================================================
//
// FILE:			/source/core/utilities/failure.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Failure handling.
//
// LAST EDITED:		October 19th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Trigger a failure and hard exit the engine.
void Rex_Failure(char *s, ...)
{
	va_list args;
	char failure_header[] = "Failure: ";
	char failure_message[512];

	strcpy(failure_message, failure_header);

	va_start(args, s);
	vsprintf(failure_message + (sizeof(failure_header) - 1), s, args);
	va_end(args);

	if (rex_videomode != REX_VIDEOMODE_TERMINAL)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failure", failure_message, NULL);

	SDL_Quit();
	printf("%s\n", failure_message);
	exit(EXIT_FAILURE);
}

// Display a warning message.
void Rex_Warning(char *s, ...)
{
	va_list args;
	char warning_header[] = "Warning: ";
	char warning_message[512];

	strcpy(warning_message, warning_header);

	va_start(args, s);
	vsprintf(warning_message + (sizeof(warning_header) - 1), s, args);
	va_end(args);

	if (rex_videomode != REX_VIDEOMODE_TERMINAL)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning", warning_message, NULL);

	printf("%s\n", warning_message);
}

// Display a non-critical message.
void Rex_Message(char *s, ...)
{
	va_list args;
	char info_header[] = "Message: ";
	char info_message[512];

	strcpy(info_message, info_header);

	va_start(args, s);
	vsprintf(info_message + (sizeof(info_header) - 1), s, args);
	va_end(args);

	if (rex_videomode != REX_VIDEOMODE_TERMINAL)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Message", info_message, NULL);

	printf("%s\n", info_message);
}

// Display a non-critical log message.
void Rex_Log(char *s, ...)
{
	va_list args;
	char log_header[] = "Log: ";
	char log_message[512];

	strcpy(log_message, log_header);

	va_start(args, s);
	vsprintf(log_message + (sizeof(log_header) - 1), s, args);
	va_end(args);

	printf("%s\n", log_message);
}

// Return a helper string for a given error code.
rex_byte *Rex_GetError(rex_int errorcode)
{
	switch (errorcode)
	{
		case REX_ERROR_NONE:
			return "No error.";

		case REX_ERROR_FILE_NONE:
			return "Couldn't find the file specified.";

		case REX_ERROR_FILE_READ:
			return "Couldn't read the file specified.";

		case REX_ERROR_MEMALLOC:
			return "Couldn't allocate the specified memory";

		case REX_ERROR_FMT_MALFORMED:
			return "The file specified appears malformed.";

		case REX_ERROR_FMT_BAD:
			return "The specified format was incorrect.";

		case REX_ERROR_FMT_DETERMINE:
			return "Couldn't determine the format of the specified file.";

		case REX_ERROR_FMT_UNSUPPORTED:
			return "This format is currently unsupported.";

		default:
			return "Invalid error code";
	}
}
