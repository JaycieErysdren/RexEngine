// ========================================================
//
// FILE:			/source/core/inc/engine/error.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the error handling functions.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Global error text buffer. Use Rex_GetError() to access this.
extern rex_byte _rex_error_buffer[512];

// Generate an error message that can be caught with Rex_GetError().
void Rex_MakeError(rex_byte *s, ...);

// Catches and returns an error generated with Rex_MakeError().
rex_byte *Rex_GetError(void);
