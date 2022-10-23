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
// LAST EDITED:		October 22nd, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Lobotomy LEV format max sectors
#define LEV_MAX_SECTORS 1024

// Load and process a Lobotomy LEV file. Returns an error code. (Formats/Lobotomy Software/lev_quake.ksy)
rex_int Rex_Formats_Lobotomy_LEV(rex_int operation, rex_byte *filename)
{
	lev_header_t lev_header;
	lev_sector_t lev_sectors[LEV_MAX_SECTORS];
	rex_int i;

	FILE *file = fopen(filename, "rb");

	if (file == NULL)
		return REX_ERROR_FILE_NONE;

	// Skip LEV sky texture data
	if (fseek(file, sizeof(lev_skydata_t), SEEK_SET)) return REX_ERROR_FMT_MALFORMED;

	// Read LEV header
	if (!fread(&lev_header, sizeof(lev_header_t), 1, file)) return REX_ERROR_FILE_READ;

	// Correct LEV header endianness
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

	// Read LEV sectors
	for (i = 0; i < lev_header.num_sectors; i++)
	{
		if (!fread(&lev_sectors[i], sizeof(lev_sector_t), 1, file)) return REX_ERROR_FILE_READ;

		// Correct LEV sector endianness
		if (REX_LITTLE_ENDIAN)
		{
			lev_sectors[i].position[0] = Rex_EndianSwap_UShort(lev_sectors[i].position[0]);
			lev_sectors[i].position[1] = Rex_EndianSwap_UShort(lev_sectors[i].position[1]);
			lev_sectors[i].position[2] = Rex_EndianSwap_UShort(lev_sectors[i].position[2]);
			lev_sectors[i].distance = Rex_EndianSwap_UShort(lev_sectors[i].distance);
			lev_sectors[i].plane_start_index = Rex_EndianSwap_UShort(lev_sectors[i].plane_start_index);
			lev_sectors[i].plane_end_index = Rex_EndianSwap_UShort(lev_sectors[i].plane_end_index);
		}
	}

	fclose(file);

	return REX_ERROR_NONE;
}
