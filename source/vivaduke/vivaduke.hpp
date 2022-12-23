// ========================================================
//
// FILE:			/source/game/game.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Viva Duke Nukem
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Main game header
//
// LAST EDITED:		December 21st, 2022
//
// ========================================================

#ifndef __VIVADUKE_H__
#define __VIVADUKE_H__

// Rex Engine modules
#define RENDERER_VOXEL 1
#define RENDERER_RAYCAST 1

#define MODULE_ZIP 1
#define MODULE_IMF 1

// Rex Engine headers
#include "rex/rex.hpp"

#define CYCLES 30

// Potential game states
typedef enum
{
	GS_EXITING,
	GS_CONSOLE,
	GS_MENU_MAIN,
	GS_MENU_PAUSE,
	GS_INGAME,
} vdn_game_states;

//
// Globals
//

// Picture buffers
extern Rex::Surface pic_font;
extern Rex::Surface pic_bbuffer;
extern Rex::Surface pic_zbuffer;
extern Rex::Surface pic_loading;

// Video info
extern Rex::VidInfo vidinfo;

// 3D Actors
extern Rex::Actor3D *actor3d_root;
extern Rex::Actor3D *actor3d_geo_voxels;
extern Rex::Actor3D *actor3d_geo_tiles;
extern Rex::Actor3D *actor3d_camera;

// 2D Actors (HUD)
extern Rex::Actor2D *actor2d_hud_root;
extern Rex::Actor2D *actor2d_hud_gun;

// 2D Actors (menu)
extern Rex::Actor2D *actor2d_root;
extern Rex::Actor2D *actor2d_mouse;
extern Rex::Actor2D *actor2d_window;
extern Rex::Actor2D *actor2d_button_exit;
extern Rex::Actor2D *actor2d_button_return;

// Game state
extern rex_int vdn_game_state;

// Mouse state
extern rex_int32 mouse_buttons;
extern rex_vec2i mouse_delta;
extern rex_rect mouse_bounds;

// Cycles
extern rex_int64 frame_start, frame_end;
extern rex_int32 cycles, c;

//
// Forward declarations
//

// bootstrp.cpp
void VDN_Initialize();
void VDN_Shutdown();

// camera.cpp
void CameraController(rex_int32 mb, rex_vec2i mp, rex_vec2i md);

// endscreen.cpp
void VDN_EndScreen();

// frame.cpp
void VDN_FrameStart();
void VDN_FrameEnd();

// level.cpp
void Generate_Level01(Raycast::RaycastModel *tilemodel, Voxel::VoxelModel *voxelmodel);
void Tilemap_Load(Raycast::RaycastModel *tilemodel, Voxel::VoxelModel *voxelmodel, string filename);

// mouse.cpp
void ReadMouse(rex_int32 *buttons, rex_vec2i *pos, rex_vec2i *delta, rex_int32 speedlimit, rex_rect area);

// tick.cpp
void VDN_MainMenuTick();
void VDN_GameTick();

#endif // __VIVADUKE_H__
