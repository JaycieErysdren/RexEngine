// ========================================================
//
// FILE:			/source/core/inc/formats/lev.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Lobotomy Software LEV types.
//
// LAST EDITED:		October 22nd, 2022
//
// ========================================================

// LEV sky texture data
typedef rex_byte lev_skydata_t[131104];

// LEV header
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

// LEV sector
typedef struct
{
	rex_ushort reserved[2];
	rex_ushort position[3];
	rex_ushort distance;
	rex_ushort plane_start_index;
	rex_ushort plane_end_index;
	rex_ushort unknown[6];
} lev_sector_t;
