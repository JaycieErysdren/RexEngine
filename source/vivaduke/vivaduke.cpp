// ========================================================
//
// FILE:			/source/vivaduke/vivaduke.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Viva Duke Nukem
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Main game entry point
//
// LAST EDITED:		December 19th, 2022
//
// ========================================================

// vivaduke header
#include "vivaduke.hpp"

//
// Globals
//

// Picture buffers
Rex::Surface pic_font;
Rex::Surface pic_bbuffer;
Rex::Surface pic_zbuffer;
Rex::Surface pic_loading;

// Video info
Rex::VidInfo vidinfo;

// 3D Actors
Rex::Actor3D *actor3d_root;
Rex::Actor3D *actor3d_geo_voxels;
Rex::Actor3D *actor3d_geo_tiles;
Rex::Actor3D *actor3d_camera;

// 2D Actors (HUD)
Rex::Actor2D *actor2d_hud_root;
Rex::Actor2D *actor2d_hud_gun;

// 2D Actors (menu)
Rex::Actor2D *actor2d_root;
Rex::Actor2D *actor2d_mouse;
Rex::Actor2D *actor2d_window;
Rex::Actor2D *actor2d_button_exit;
Rex::Actor2D *actor2d_button_return;

// Game state
rex_int vdn_game_state;

// Mouse state
rex_int32 mouse_buttons;
rex_vec2i mouse_delta;
rex_rect mouse_bounds;

// Cycles
rex_int64 frame_start, frame_end;
rex_int32 cycles, c;

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// initialize engine, allocate memory, etc
	VDN_Initialize();

	// Start counting time
	frame_end = Rex::GetTicks64();

	// start up
	vdn_game_state = GS_MENU_MAIN;

	// Main loop
	while (vdn_game_state != GS_EXITING)
	{
		// Start frame logic
		VDN_FrameStart();

		// Read mouse
		VDN_ReadMouse(&mouse_buttons, &actor2d_mouse->origin, &mouse_delta, 16, mouse_bounds);

		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 6);

		// Clear z buffer
		Rex::SurfaceClear(&pic_zbuffer, 255);

		// Run the appropriate game tick
		switch (vdn_game_state)
		{
			// game is exiting to DOS
			case GS_EXITING:
				break;

			// full screen console
			case GS_CONSOLE:
				break;

			// main menu, seen on startup
			case GS_MENU_MAIN:
				VDN_MainMenuTick();
				break;

			// pause menu
			case GS_MENU_PAUSE:
				VDN_MainMenuTick();
				break;

			// in game
			case GS_INGAME:
				VDN_GameTick();
				break;

			// unknown value
			default:
				break;
		}

		// Flip the rendering buffers
		Rex::SurfaceToFrontBuffer(&pic_bbuffer);

		// End frame logic
		VDN_FrameEnd();
	}

	// Shutdown engine, free up memory, etc
	VDN_Shutdown();

	// Show the ANSI text end screen
	VDN_EndScreen();

	// Exit gracefully
	return EXIT_SUCCESS;
}
