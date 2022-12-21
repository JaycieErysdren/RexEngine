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
// LAST EDITED:		October 23rd, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Load a Lobotomy Software LEV file into memory. Returns a pointer to a LEV object.
lev_t *LEV_Load(rex_byte *filename)
{
	// Define variables
	rex_int i;
	lev_t *lev;
	FILE *file;

	// Open file pointer
	file = Rex_IO_FOpen(filename, "rb");

	// Skip sky data, we can't parse it yet
	Rex_IO_FSeek(file, 131104, SEEK_SET);

	// Allocate memory
	lev = calloc(1, sizeof(lev_t));
	lev->header = calloc(1, sizeof(lev_header_t));

	// Read in header
	Rex_IO_FRead(lev->header, sizeof(lev_header_t), 1, file);

	// Correct header endianness
	if (REX_LITTLE_ENDIAN)
	{
		Rex_EndianSwap_UInt(&lev->header->unknown_01);
		Rex_EndianSwap_UInt(&lev->header->unknown_02);
		Rex_EndianSwap_UInt(&lev->header->num_sectors);
		Rex_EndianSwap_UInt(&lev->header->num_planes);
		Rex_EndianSwap_UInt(&lev->header->num_vertices);
		Rex_EndianSwap_UInt(&lev->header->num_quads);
		Rex_EndianSwap_UInt(&lev->header->len_tile_texture_data);
		Rex_EndianSwap_UInt(&lev->header->num_tiles);
		Rex_EndianSwap_UInt(&lev->header->len_tile_color_data);
		Rex_EndianSwap_UInt(&lev->header->num_entities);
		Rex_EndianSwap_UInt(&lev->header->len_entity_data);
		Rex_EndianSwap_UInt(&lev->header->num_entity_polylinks);
		Rex_EndianSwap_UInt(&lev->header->num_entity_polylink_data1_segments);
		Rex_EndianSwap_UInt(&lev->header->num_entity_polylink_data2_segments);
		Rex_EndianSwap_UInt(&lev->header->num_unknown);
	}

	// Allocate memory
	lev->sectors = calloc(lev->header->num_sectors, sizeof(lev_sector_t));

	// Read in sectors
	Rex_IO_FRead(lev->sectors, sizeof(lev_sector_t), lev->header->num_sectors, file);

	// Correct sector endianness
	if (REX_LITTLE_ENDIAN)
	{
		for (i = 0; i < lev->header->num_sectors; i++)
		{
			Rex_EndianSwap_UShort(&lev->sectors[i].position[0]);
			Rex_EndianSwap_UShort(&lev->sectors[i].position[1]);
			Rex_EndianSwap_UShort(&lev->sectors[i].position[2]);
			Rex_EndianSwap_UShort(&lev->sectors[i].distance);
			Rex_EndianSwap_UShort(&lev->sectors[i].plane_start_index);
			Rex_EndianSwap_UShort(&lev->sectors[i].plane_end_index);
		}
	}

	// Close file pointer
	Rex_IO_FClose(file);

	// Return pointer
	return lev;
}

// Free a Lobotomy Software LEV object from memory.
void LEV_Free(lev_t *lev)
{
	// Free header
	free(lev->header);

	// Free sectors
	free(lev->sectors);

	// Free container
	free(lev);
}
