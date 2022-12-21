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

// load a tilemap
void Tilemap_Load(Raycast::RaycastModel *tilemodel, Voxel::VoxelModel *voxelmodel, string filename)
{
	// variables
	cJSON *json, *json_array, *json_arrayitem, *json_child, *json_mapdata;
	FILE *file;
	rex_int file_len, map_w, map_h, mapdata, mapdata_len, x, y, x1, y1;
	char *buffer;

	// open file
	file = fopen(filename.c_str(), "rt");
	if (file == NULL) return;

	// seek to end
	fseek(file, 0L, SEEK_END);

	// get file length
	file_len = ftell(file);

	// seek to start
	fseek(file, 0L, SEEK_SET);

	// allocate char buffer
	buffer = (char *)calloc(1, file_len);
	if (buffer == NULL) return;

	// read file into buffer
	fread(buffer, sizeof(char), file_len, file);

	// close file
	fclose(file);

	// parse json string
	json = cJSON_ParseWithLength(buffer, file_len);
	if (json == NULL) return;

	// get layer array
	json_array = cJSON_GetObjectItem(json, "layers");
	if (json_array == NULL) return;

	// get first item
	json_arrayitem = cJSON_GetArrayItem(json_array, 0);
	if (json_arrayitem == NULL) return;

	// get width
	json_child = cJSON_GetObjectItem(json_arrayitem, "width");
	if (json_child == NULL) return;

	map_w = json_child->valueint;

	// get height
	json_child = cJSON_GetObjectItem(json_arrayitem, "height");
	if (json_child == NULL) return;

	map_h = json_child->valueint;

	// get map data
	json_mapdata = cJSON_GetObjectItem(json_arrayitem, "data");
	if (json_mapdata == NULL) return;

	// get length of map data array
	mapdata_len = cJSON_GetArraySize(json_mapdata);
	if (mapdata_len < 1) return;
	if (mapdata_len != map_w * map_h) return;

	// create slabs for map data array
	for (y = 0; y < map_h; y++)
	{
		for (x = 0; x < map_w; x++)
		{
			json_child = cJSON_GetArrayItem(json_mapdata, (y * map_h) + x);
			if (json_child == NULL) return;

			mapdata = json_child->valueint;

			if (mapdata > 0)
			{
				Generate_Tile(tilemodel, x, y, mapdata);
			}
		}
	}

	cJSON_Delete(json);
	free(buffer);
}
