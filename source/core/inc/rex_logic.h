// ========================================================
//
// FILE:			/source/core/inc/rex_logic.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the logic module.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

//
// Math helpers (math.c)
//

// Returns a random integer between high and low.
rex_int Rex_Random(rex_int low, rex_int high);

//
// Startup handling (startup.c)
//

// Startup everything
void Rex_Startup(void);

//
// Shutdown handling (shutdown.c)
//

// Shutdown everything and return exit code
void Rex_Shutdown(rex_int exit_code);
