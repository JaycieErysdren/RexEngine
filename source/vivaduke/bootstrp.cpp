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

// VDN intiialization function
void VDN_Initialize()
{
	// Initialize Rex Engine
	Rex::Initialize();

	// Load VFS
	if (Rex::AddVFS("vivaduke.zip", Rex::VFS_FORMAT_DETERMINE) == false)
	{
		cout << "failed to open \"vivaduke.zip\"" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "successfully mounted virtual file system" << endl;

	// Initialize graphics
	if (Rex::InitializeGraphics(320, 200, 8) == false)
	{
		cout << "failed to initialize graphics driver" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "successfully initialized graphics driver" << endl;

	vidinfo = Rex::GetVidInfo();

	// Create picture buffers
	Rex::SurfaceLoadBMP(&pic_font, "gfx/font8x8.bmp");
	Rex::SurfaceLoadBMP(&pic_loading, "gfx/loading.bmp");
	Rex::SurfaceCreate(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);
	Rex::SurfaceCreate(&pic_zbuffer, vidinfo.width, vidinfo.height, 8, 0, 0);

	Rex::SetGraphicsPalette("gfx/duke3d.pal");
	Rex::ColormapLoad("gfx/duke3d.tab");

	// show a loading screen so the user knows we're doing something
	Rex::SurfaceToFrontBuffer(&pic_loading);

	// 2D Actors (menu)
	actor2d_root = Rex::AddActor2D(NULL, Rex::ACTOR2D_NONE);

	actor2d_window = Rex::AddActor2D(actor2d_root, Rex::ACTOR2D_WINDOW, "gfx/bg2.bmp");
	actor2d_button_exit = Rex::AddActor2D(actor2d_window, Rex::ACTOR2D_BUTTON, "gfx/btn_exit.bmp");
	actor2d_button_return = Rex::AddActor2D(actor2d_window, Rex::ACTOR2D_BUTTON, "gfx/btn_clck.bmp");

	actor2d_mouse = Rex::AddActor2D(actor2d_root, Rex::ACTOR2D_CURSOR, "gfx/cursor.bmp");
	actor2d_mouse->color_blit_mode = Rex::COLORKEY;

	// 2D Actors (HUD)
	actor2d_hud_root = Rex::AddActor2D(NULL, Rex::ACTOR2D_NONE);

	actor2d_hud_gun = Rex::AddActor2D(actor2d_hud_root, Rex::ACTOR2D_VIEWMODEL, "gfx/shotgun.bmp");
	actor2d_hud_gun->color_blit_mode = Rex::COLORKEY;

	//
	// 3D actors
	//

	// scene root
	actor3d_root = Rex::AddActor3D(NULL, Rex::ACTOR3D_NONE);

	// camera
	actor3d_camera = Rex::AddActor3D(actor3d_root, Rex::ACTOR3D_CAMERA);

	// tile model
	actor3d_geo_tiles = Rex::AddActor3D(actor3d_root, Rex::ACTOR3D_RAYCASTMODEL);
	actor3d_geo_tiles->model = Raycast::AddRaycastModel(128, 128);

	char fname_c[16];
	string fname;
	Rex::Surface surf;

	// load in wall surfaces
	for (rex_int i = 1; i < 11; i++)
	{
		sprintf(fname_c, "tex/w%03d.bmp", i);
		fname = fname_c;

		((Raycast::RaycastModel *)actor3d_geo_tiles->model)->wall_surfaces.push_back(surf);
		Rex::SurfaceLoadBMP(&((Raycast::RaycastModel *)actor3d_geo_tiles->model)->wall_surfaces[i - 1], fname);
	}

	// load in floor surfaces
	for (rex_int i = 1; i < 7; i++)
	{
		sprintf(fname_c, "tex/f%03d.bmp", i);
		fname = fname_c;

		((Raycast::RaycastModel *)actor3d_geo_tiles->model)->floor_surfaces.push_back(surf);
		Rex::SurfaceLoadBMP(&((Raycast::RaycastModel *)actor3d_geo_tiles->model)->floor_surfaces[i - 1], fname);
	}

	// voxel model
	actor3d_geo_voxels = Rex::AddActor3D(actor3d_root, Rex::ACTOR3D_VOXELMODEL);
	actor3d_geo_voxels->model = Voxel::AddVoxelModel(128, 128, 128);

	// load a tilemap
	Tilemap_Load((Raycast::RaycastModel *)actor3d_geo_tiles->model, (Voxel::VoxelModel *)actor3d_geo_voxels->model, "maps/casino.tmj");

	//
	// Initialize camera info
	//

	actor3d_camera->draw_distance = REX_SCALAR(64);
	actor3d_camera->anglespeedkey = 4;

	actor3d_camera->origin.x = REX_SCALAR(34.5f);
	actor3d_camera->origin.y = REX_SCALAR(34.5f);
	actor3d_camera->origin.z = REX_SCALAR(0.5f);

	actor3d_camera->angles.x = 0;
	actor3d_camera->angles.y = 0;
	actor3d_camera->angles.z = 0;

	// cursor boundary area
	mouse_bounds = {0, 0, pic_bbuffer.width - actor2d_mouse->color.width, pic_bbuffer.height - actor2d_mouse->color.height};
}

void VDN_Shutdown()
{
	// Cleanup picture buffers
	Rex::SurfaceDestroy(&pic_font);
	Rex::SurfaceDestroy(&pic_bbuffer);
	Rex::SurfaceDestroy(&pic_zbuffer);
	Rex::SurfaceDestroy(&pic_loading);

	// Free 3D scene hierarchy
	Rex::FreeActor3D(actor3d_root);

	// Free 2D scene hierarchy
	Rex::FreeActor2D(actor2d_root);

	// Free 2D HUD hierarchy
	Rex::FreeActor2D(actor2d_hud_root);

	// Shutdown Graphics
	Rex::ShutdownGraphics();

	// Shutdown Rex Engine
	Rex::Shutdown();
}
