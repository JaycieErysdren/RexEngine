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
// LAST EDITED:		October 10th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

//
// Public functions
//

// Trigger a failure and hard exit the engine.
void Rex_Failure(char *s, ...)
{
	va_list args;
	char failure_header[] = "Rex Engine Failure: ";
	char failure_message[512];

	strcpy(failure_message, failure_header);

	va_start(args, s);
	vsprintf(failure_message + (sizeof(failure_header) - 1), s, args);
	va_end(args);

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Rex Engine Failure", failure_message, NULL);

	SDL_Quit();
	printf("%s\n", failure_message);
	exit(EXIT_FAILURE);
}
