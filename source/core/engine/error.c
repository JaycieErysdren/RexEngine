// ========================================================
//
// FILE:			/source/core/engine/error.c
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
