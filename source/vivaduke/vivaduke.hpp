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
// LAST EDITED:		December 19th, 2022
//
// ========================================================

#ifndef __VIVADUKE_H__
#define __VIVADUKE_H__

// Rex Engine modules
#define RENDERER_VOXEL 1
#define RENDERER_RAYCAST 1
#define RENDERER_SECTOR 1

// Rex Engine headers
#include "rex/rex.hpp"

// Level.cpp
void Generate_Level01(Raycast::RaycastModel *tilemodel, Voxel::VoxelModel *voxelmodel);
void Tilemap_Load(Raycast::RaycastModel *tilemodel, Voxel::VoxelModel *voxelmodel, string filename);

#endif // __VIVADUKE_H__
