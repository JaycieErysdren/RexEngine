// ========================================================
//
// FILE:			/source/rex3d/thrdprty/tiled.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Tiled namespace implementation
//
// LAST EDITED:		December 2nd, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

//
//
// Tiled namespace
//
//

// Tiled namespace definition (private)
namespace Tiled
{

//
//
// Functions
//
//

TileMap *LoadTMJ(string filename)
{
	TileMap *map;
	cJSON *json;
	cJSON *json_child;
	FILE *file;
	int file_len;
	char *file_buffer;

	map = new TileMap;

	file = fopen(filename.c_str(), "rt");
	if (file == NULL) return NULL;
	fseek(file, 0, SEEK_END);
	file_len = ftell(file);
	fseek(file, 0, SEEK_SET);

	file_buffer = (char *)calloc(file_len, sizeof(char));

	fread(file_buffer, sizeof(char), file_len, file);

	json = cJSON_ParseWithLength(file_buffer, file_len);
	if (json == NULL) return NULL;

	json_child = cJSON_GetObjectItemCaseSensitive(json, "width");
	if (json_child == NULL) return NULL;
	
	map->width = json_child->valueint;

	json_child = cJSON_GetObjectItemCaseSensitive(json, "height");
	if (json_child == NULL) return NULL;
	
	map->height = json_child->valueint;

	free(file_buffer);

	return map;
}

void FreeTileMap(TileMap *map)
{
	if (map->floor_height_data) free(map->floor_height_data);
	if (map->floor_texture_data) free(map->floor_texture_data);
	if (map->wall_texture_data) free(map->wall_texture_data);
	if (map->ceiling_height_data) free(map->ceiling_height_data);
	if (map->ceiling_texture_data) free(map->ceiling_texture_data);
	if (map) free(map);
}

} // namespace Tiled
