// ========================================================
//
// FILE:			/source/vivaduke/tick.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Viva Duke Nukem
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Main game tick function
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

// vivaduke header
#include "vivaduke.hpp"

// Main menu tick
void VDN_MainMenuTick()
{
	// Mouse click
	if (mouse_buttons == 1) actor2d_mouse->events |= Rex::ACTOR2D_EVENT01;

	//
	// Return-to-game button
	//

	// Position logic
	actor2d_button_return->origin.x = 96;
	actor2d_button_return->origin.y = 32;
	actor2d_button_return->draw_area.x1 = 0;
	actor2d_button_return->draw_area.y1 = 0;
	actor2d_button_return->draw_area.x2 = 48;
	actor2d_button_return->draw_area.y2 = 24;

	// Mouse hover logic
	if (actor2d_mouse->OriginInside(actor2d_button_return))
	{
		actor2d_button_return->draw_area.x1 = 48;
		actor2d_button_return->draw_area.y1 = 0;
		actor2d_button_return->draw_area.x2 = 96;
		actor2d_button_return->draw_area.y2 = 24;

		if (actor2d_mouse->events && Rex::ACTOR2D_EVENT01)
		{
			actor2d_button_return->draw_area.x1 = 96;
			actor2d_button_return->draw_area.y1 = 0;
			actor2d_button_return->draw_area.x2 = 144;
			actor2d_button_return->draw_area.y2 = 24;

			vdn_game_state = GS_INGAME;
		}
	}

	//
	// Exit button
	//

	// Position logic
	actor2d_button_exit->origin.x = 32;
	actor2d_button_exit->origin.y = 32;
	actor2d_button_exit->draw_area.x1 = 0;
	actor2d_button_exit->draw_area.y1 = 0;
	actor2d_button_exit->draw_area.x2 = 48;
	actor2d_button_exit->draw_area.y2 = 24;

	// Mouse hover logic
	if (actor2d_mouse->OriginInside(actor2d_button_exit))
	{
		actor2d_button_exit->draw_area.x1 = 48;
		actor2d_button_exit->draw_area.y1 = 0;
		actor2d_button_exit->draw_area.x2 = 96;
		actor2d_button_exit->draw_area.y2 = 24;

		if (actor2d_mouse->events && Rex::ACTOR2D_EVENT01)
		{
			actor2d_button_exit->draw_area.x1 = 96;
			actor2d_button_exit->draw_area.y1 = 0;
			actor2d_button_exit->draw_area.x2 = 144;
			actor2d_button_exit->draw_area.y2 = 24;

			vdn_game_state = GS_EXITING;
		}
	}

	//
	// Mouse
	//

	// Clear mouse events
	actor2d_mouse->ClearEvents();

	// Render 2D scene
	Rex::RenderScene2D(&pic_bbuffer, &pic_zbuffer, actor2d_root, REX_SCALAR(vidinfo.width / 2));
}

// Game tick
void VDN_GameTick()
{
	// Return to pause menu
	if (Rex::KeyTest(REX_SC_ESCAPE)) vdn_game_state = GS_MENU_PAUSE;

	// Cycles
	for (c = 0; c < cycles; c++)
	{
		// Camera controller
		VDN_Control_Camera(mouse_buttons, actor2d_mouse->origin, mouse_delta);
	}

	//
	// Rendering
	//

	// Render 3D scene
	Rex::RenderScene3D(&pic_bbuffer, &pic_zbuffer, actor3d_root, actor3d_camera, REX_SCALAR(vidinfo.width / 2));

	// Render 2D scene
	actor2d_hud_gun->origin.x = pic_bbuffer.width / 2;
	actor2d_hud_gun->origin.y = pic_bbuffer.height - actor2d_hud_gun->color.height;

	Rex::RenderScene2D(&pic_bbuffer, &pic_zbuffer, actor2d_hud_root, REX_SCALAR(160));

	// Render onscreen text
	Rex::ConsoleTextF(&pic_bbuffer, &pic_font, 8, 0, 0, "x: %d y: %d z %d", RexScalarToInteger(actor3d_camera->origin.x), RexScalarToInteger(actor3d_camera->origin.y), RexScalarToInteger(actor3d_camera->origin.z));
	Rex::ConsoleTextF(&pic_bbuffer, &pic_font, 8, 0, 1, "pitch: %d yaw: %d roll %d", actor3d_camera->angles.x, actor3d_camera->angles.y, actor3d_camera->angles.z);
}
