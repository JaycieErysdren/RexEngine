// ========================================================
//
// FILE:			/source/applications/liberator/liberator.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Liberator header.
//
// LAST EDITED:		October 21st, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Camera angle
extern rex_vector3i camera_angle;

// Camera position
extern rex_vector3i camera_position;

// Set camera position
void Liberator_Camera_SetPosition(rex_int x, rex_int y, rex_int z);

// Set camera angle
void Liberator_Camera_SetAngle(rex_int x, rex_int y, rex_int z);

// Handle mouse inputs
void Liberator_Mouse_OrbitControls(void);

// Print help text
void Liberator_PrintHelpText(void);
