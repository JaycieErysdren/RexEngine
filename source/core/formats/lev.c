// ========================================================
//
// FILE:			/source/core/formats/lev.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Lobotomy Software LEV files.
//
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Lobotomy LEV file sky texture data
typedef rex_byte lev_skydata_t[131104];

// Lobotomy LEV file header
typedef struct
{
	rex_uint unknown_01;
	rex_uint unknown_02;
	rex_uint num_sectors;
	rex_uint num_planes;
	rex_uint num_vertices;
	rex_uint num_quads;
	rex_uint len_tile_texture_data;
	rex_uint num_tiles;
	rex_uint len_tile_color_data;
	rex_uint num_entities;
	rex_uint len_entity_data;
	rex_uint num_entity_polylinks;
	rex_uint num_entity_polylink_data1_segments;
	rex_uint num_entity_polylink_data2_segments;
	rex_uint num_unknown;
} lev_header_t;

// Load and process a Lobotomy LEV file. Returns an error code. (Formats/Lobotomy Software/lev_quake.ksy)
rex_int Rex_Formats_Lobotomy_Lev(rex_int operation, rex_byte *filename)
{
	lev_header_t lev_header;

	FILE *file = fopen(filename, "rb");

	if (file == NULL)
		return REX_ERROR_FILE_NONE;

	if (fseek(file, sizeof(lev_skydata_t), SEEK_SET)) return REX_ERROR_FMT_MALFORMED;

	if (!fread(&lev_header, sizeof(lev_header_t), 1, file)) return REX_ERROR_FILE_READ;

	if (REX_LITTLE_ENDIAN)
	{
		lev_header.unknown_01 = Rex_EndianSwap_UInt(lev_header.unknown_01);
		lev_header.unknown_02 = Rex_EndianSwap_UInt(lev_header.unknown_02);
		lev_header.num_sectors = Rex_EndianSwap_UInt(lev_header.num_sectors);
		lev_header.num_planes = Rex_EndianSwap_UInt(lev_header.num_planes);
		lev_header.num_vertices = Rex_EndianSwap_UInt(lev_header.num_vertices);
		lev_header.num_quads = Rex_EndianSwap_UInt(lev_header.num_quads);
		lev_header.len_tile_texture_data = Rex_EndianSwap_UInt(lev_header.len_tile_texture_data);
		lev_header.num_tiles = Rex_EndianSwap_UInt(lev_header.num_tiles);
		lev_header.len_tile_color_data = Rex_EndianSwap_UInt(lev_header.len_tile_color_data);
		lev_header.num_entities = Rex_EndianSwap_UInt(lev_header.num_entities);
		lev_header.len_entity_data = Rex_EndianSwap_UInt(lev_header.len_entity_data);
		lev_header.num_entity_polylinks = Rex_EndianSwap_UInt(lev_header.num_entity_polylinks);
		lev_header.num_entity_polylink_data1_segments = Rex_EndianSwap_UInt(lev_header.num_entity_polylink_data1_segments);
		lev_header.num_entity_polylink_data2_segments = Rex_EndianSwap_UInt(lev_header.num_entity_polylink_data2_segments);
		lev_header.num_unknown = Rex_EndianSwap_UInt(lev_header.num_unknown);
	}

	Rex_Message(
		"unknown_01 %d\nunknown_02 %d\nnum_sectors %d\nnum_planes %d\nnum_vertices %d\nnum_quads %d\nlen_tile_texture_data %d\nnum_tiles %d\nlen_tile_color_data %d\nnum_entities %d\nlen_entity_data %d\nnum_entity_polylinks %d\nnum_entity_polylink_data1_segments %d\nnum_entity_polylink_data2_segments %d\nnum_unknown %d\n",
		lev_header.unknown_01,
		lev_header.unknown_02,
		lev_header.num_sectors,
		lev_header.num_planes,
		lev_header.num_vertices,
		lev_header.num_quads,
		lev_header.len_tile_texture_data,
		lev_header.num_tiles,
		lev_header.len_tile_color_data,
		lev_header.num_entities,
		lev_header.len_entity_data,
		lev_header.num_entity_polylinks,
		lev_header.num_entity_polylink_data1_segments,
		lev_header.num_entity_polylink_data2_segments,
		lev_header.num_unknown
	);

	fclose(file);

	return REX_ERROR_NONE;
}
