// ========================================================
//
// FILE:			/source/vivaduke/bootstrap.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Viva Duke Nukem
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Bootstrap functions for VDN
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

// vivaduke header
#include "vivaduke.hpp"

// Camera controller
void VDN_Control_Camera(rex_int32 mb, rex_vec2i mp, rex_vec2i md)
{
	// Mouse look
	if (md.x != 0) actor3d_camera->angles.y -= md.x;

	//
	// Keyboard look
	//

	// Rotate leftwards
	if (Rex::KeyTest(REX_SC_LEFT)) actor3d_camera->angles.y += actor3d_camera->anglespeedkey;

	// Rotate rightwards
	if (Rex::KeyTest(REX_SC_RIGHT)) actor3d_camera->angles.y -= actor3d_camera->anglespeedkey;

	// Yaw angle sanity checks
	if (actor3d_camera->angles.y < 0) actor3d_camera->angles.y += 360;
	if (actor3d_camera->angles.y > 359) actor3d_camera->angles.y -= 360;

	// Set velocity
	actor3d_camera->velocity.x = REX_MUL(Rex::math_table->sin[actor3d_camera->angles.y], REX_SCALAR(0.125f));
	actor3d_camera->velocity.y = REX_MUL(Rex::math_table->cos[actor3d_camera->angles.y], REX_SCALAR(0.125f));
	actor3d_camera->velocity.z = REX_SCALAR(0.125f);

	// Move forwards
	if (Rex::KeyTest(REX_SC_W) || Rex::KeyTest(REX_SC_UP))
	{
		actor3d_camera->origin.x += actor3d_camera->velocity.x;
		actor3d_camera->origin.y += actor3d_camera->velocity.y;
	}

	// Move backwards
	if (Rex::KeyTest(REX_SC_S) || Rex::KeyTest(REX_SC_DOWN))
	{
		actor3d_camera->origin.x -= actor3d_camera->velocity.x;
		actor3d_camera->origin.y -= actor3d_camera->velocity.y;
	}

	// Move leftwards
	if (Rex::KeyTest(REX_SC_A))
	{
		actor3d_camera->origin.x += actor3d_camera->velocity.y;
		actor3d_camera->origin.y -= actor3d_camera->velocity.x;
	}

	// Move rightwards
	if (Rex::KeyTest(REX_SC_D))
	{
		actor3d_camera->origin.x -= actor3d_camera->velocity.y;
		actor3d_camera->origin.y += actor3d_camera->velocity.x;
	}
}
