// ========================================================
//
// FILE:			/source/liberator/liberator.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Liberator header.
//
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Getopt
#include <getopt.h>

// Camera angle
extern rex_vector3d camera_angle;

// Camera position
extern rex_vector3d camera_position;

// Set camera position
void Liberator_Camera_SetPosition(rex_double x, rex_double y, rex_double z);

// Set camera angle
void Liberator_Camera_SetAngle(rex_double x, rex_double y, rex_double z);

// Handle mouse inputs
void Liberator_Mouse_OrbitControls(void);

// Print help text
void Liberator_PrintHelpText(void);
