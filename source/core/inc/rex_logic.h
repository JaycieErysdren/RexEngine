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
// LAST EDITED:		October 29th, 2022
//
// ========================================================

//
// Math helpers (math.c)
//

// Returns a random integer between high and low.
rex_int Rex_Random(rex_int low, rex_int high);

// Write normalized vectors into v_up, v_right and v_forward
void Rex_MakeVectors(rex_vector3f angles, rex_vector3f *v_up, rex_vector3f *v_right, rex_vector3f *v_forward);

//
// Startup handling (startup.c)
//

// Startup everything
void Rex_Startup(void);

//
// Shutdown handling (shutdown.c)
//

// Shutdown everything
void Rex_Shutdown(void);
