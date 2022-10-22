// ========================================================
//
// FILE:			/source/core/logic/math.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Math helper functions.
//
// LAST EDITED:		October 20th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Returns a random integer between high and low.
rex_int Rex_Random(rex_int low, rex_int high)
{
	return (rand() % (high - low + 1)) + low;
}

// Write normalized vectors into v_up, v_right and v_forward
void Rex_MakeVectors(rex_vector3f angles, rex_vector3f *v_up, rex_vector3f *v_right, rex_vector3f *v_forward)
{
	rex_float angle;
	rex_float sr, sp, sy, cr, cp, cy;

	rex_int yaw = 1;
	rex_int pitch = 0;
	rex_int roll = 2;

	angle = angles[yaw] * (PI * 2 / 360);
	sy = sinf(angle);
	cy = cosf(angle);
	angle = angles[pitch] * (PI * 2 / 360);
	sp = sinf(angle);
	cp = cosf(angle);
	angle = angles[roll] * (PI * 2 / 360);
	sr = sinf(angle);
	cr = cosf(angle);

	if (*v_up)
	{
		*v_up[0] = cr * sp * cy + -sr * -sy;
		*v_up[1] = cr * sp * sy + -sr * cy;
		*v_up[2] = cr * cp;
	}

	if (*v_right)
	{
		*v_right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
		*v_right[1] = -1 * sr * sp * sy + -1 * cr * cy;
		*v_right[2] = -1 * sr * cp;
	}

	if (*v_forward)
	{
		*v_forward[0] = cp * cy;
		*v_forward[1] = cp * sy;
		*v_forward[2] = -sp;
	}

	Rex_Log("source angles: %f %f %f", angles[0], angles[1], angles[2]);
	Rex_Log("v_forward: %f %f %f", *v_forward[0], *v_forward[1], *v_forward[2]);
	Rex_Log("v_right: %f %f %f", *v_right[0], *v_right[1], *v_right[2]);
	Rex_Log("v_up: %f %f %f\n", *v_up[0], *v_up[1], *v_up[2]);
}
