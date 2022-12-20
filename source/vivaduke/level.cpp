// ========================================================
//
// FILE:			/source/vivaduke/level.cpp
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

void Generate_Tile_Rect(Raycast::RaycastModel *tilemodel, rex_int x, rex_int y, rex_int w, rex_int h, rex_uint8 color, bool filled)
{
	rex_int fx, fy;

	for (fy = y; fy < y + h; fy++)
	{
		for (fx = x; fx < x + w; fx++)
		{
			if (filled == false && (fx == x || fx == x + w - 1|| fy == y || fy == y + h - 1))
				tilemodel->SetTile(fx, fy, color);
			else if (filled == true)
				tilemodel->SetTile(fx, fy, color);
		}
	}
}

void Generate_Tile_Line_H(Raycast::RaycastModel *tilemodel, rex_int x, rex_int y, rex_int w, rex_uint8 c)
{
	for (rex_int fx = x; fx < x + w; fx++)
	{
		tilemodel->SetTile(fx, y, c);
	}
}

void Generate_Tile_Line_V(Raycast::RaycastModel *tilemodel, rex_int x, rex_int y, rex_int h, rex_uint8 c)
{
	for (rex_int fy = y; fy < y + h; fy++)
	{
		tilemodel->SetTile(x, fy, c);
	}
}

void Generate_Tile(Raycast::RaycastModel *tilemodel, rex_int x, rex_int y, rex_uint8 c)
{
	tilemodel->SetTile(x, y, c);
}

// make a level in code
void Generate_Level01(Raycast::RaycastModel *tilemodel, Voxel::VoxelModel *voxelmodel)
{
	// variables
	rex_int x, y;

	// start room
	Generate_Tile_Rect(tilemodel, 32, 32, 5, 6, 8, false); // 47
	Generate_Tile_Line_H(tilemodel, 33, 37, 3, 0);
	Generate_Tile(tilemodel, 34, 37, 8);

	// next room
}
