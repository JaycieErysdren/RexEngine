// ========================================================
//
// FILE:		/source/duke4evr/main.cpp
//
// AUTHORS:		Jaycie Ewald
//
// PROJECT:		Duke4Ever
//
// LICENSE:		TBD
//
// DESCRIPTION:		Duke4Ever program entry point
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

// Duke4Ever header
#include "duke4evr.hpp"

// Main entry point
int main(int argc, char *argv[])
{
	// Variables
	int i, x, y;
	scalar_t move_speed = SCALAR(0.2f);
	scalar_t rot_speed = SCALAR(0.1f);
	Raycaster::Camera camera;
	int mb, mx, my;

	// Setup camera defaults
	camera.origin[0] = SCALAR(22.0f);
	camera.origin[1] = SCALAR(12.0f);
	camera.origin[2] = SCALAR(0.5f * 200);
	camera.angle[0] = SCALAR(-1.0f);
	camera.angle[1] = SCALAR(0.0f);
	camera.plane[0] = SCALAR(0.0f);
	camera.plane[1] = SCALAR(0.66f);

	// Initialize DOS
	DOS::Initialize();

	// Initialize VGA
	VGA::Initialize();
	VGA::SetPalette("wolf.pal");

	// Load textures
	Raycaster::LoadTextures();

	// Clear the screen
	VGA::Clear();

	// Main loop
	while (!DOS::KeyTest(KB_ESC))
	{
		//
		// Input handling
		//

		// Read mouse
		mb = DOS::MouseRead(&mx, &my);

		// Move forward
		if(DOS::KeyTest(KB_W))
		{
			if(world_map[int(camera.origin[0] + camera.angle[0] * move_speed)][int(camera.origin[1])] == false) camera.origin[0] += camera.angle[0] * move_speed;
			if(world_map[int(camera.origin[0])][int(camera.origin[1] + camera.angle[1] * move_speed)] == false) camera.origin[1] += camera.angle[1] * move_speed;
		}
		
		// Move backward
		if(DOS::KeyTest(KB_S))
		{
			if(world_map[int(camera.origin[0] - camera.angle[0] * move_speed)][int(camera.origin[1])] == false) camera.origin[0] -= camera.angle[0] * move_speed;
			if(world_map[int(camera.origin[0])][int(camera.origin[1] - camera.angle[1] * move_speed)] == false) camera.origin[1] -= camera.angle[1] * move_speed;
		}

		// Rotate right
		if(DOS::KeyTest(KB_D))
		{
			scalar_t old_dir0 = camera.angle[0];
			camera.angle[0] = camera.angle[0] * cos(-rot_speed) - camera.angle[1] * sin(-rot_speed);
			camera.angle[1] = old_dir0 * sin(-rot_speed) + camera.angle[1] * cos(-rot_speed);
			scalar_t old_plane0 = camera.plane[0];
			camera.plane[0] = camera.plane[0] * cos(-rot_speed) - camera.plane[1] * sin(-rot_speed);
			camera.plane[1] = old_plane0 * sin(-rot_speed) + camera.plane[1] * cos(-rot_speed);
		}

		// Rotate left
		if(DOS::KeyTest(KB_A))
		{
			scalar_t old_dir0 = camera.angle[0];
			camera.angle[0] = camera.angle[0] * cos(rot_speed) - camera.angle[1] * sin(rot_speed);
			camera.angle[1] = old_dir0 * sin(rot_speed) + camera.angle[1] * cos(rot_speed);
			scalar_t old_plane0 = camera.plane[0];
			camera.plane[0] = camera.plane[0] * cos(rot_speed) - camera.plane[1] * sin(rot_speed);
			camera.plane[1] = old_plane0 * sin(rot_speed) + camera.plane[1] * cos(rot_speed);
		}

		// Clear back buffer
		VGA::Clear();

		// Render to back buffer
		Raycaster::Render(camera, 320, 200, true);

		// Flip buffer
		VGA::Flip();
	}

	// Shutdown VGA
	VGA::Shutdown();

	// Shutdown DOS
	DOS::Shutdown();

	// Exit gracefully
	return EXIT_SUCCESS;
}
