// ========================================================
//
// FILE:			/source/core/formats/tmf.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Tank Engine TMF files.
//
// LAST EDITED:		October 22nd, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Load and process a Tank Engine TMF file. Returns a TMF container. (Formats/Tank Engine/tmf.ksy)
tmf_t *Rex_Formats_TankEngine_TMF(rex_int operation, rex_byte *filename)
{
	// Define variables
	rex_int i, v, q;

	// Define TMF struct
	tmf_t *tmf;

	// Define file pointer
	FILE *file;

	// Open file pointer
	file = Rex_IO_FOpen(filename, "rb");

	// Allocate memory
	tmf = calloc(1, sizeof(tmf_t));
	tmf->header = calloc(1, sizeof(tmf_header_t));

	// Read in header
	Rex_IO_FRead(tmf->header, sizeof(tmf_header_t), 1, file);

	// Allocate texture & mesh memory
	tmf->textures = calloc(tmf->header->num_textures, sizeof(tmf_texture_t));
	tmf->meshes = calloc(tmf->header->num_meshes, sizeof(tmf_mesh_t));

	// Read in textures
	Rex_IO_FRead(tmf->textures, sizeof(tmf_texture_t), tmf->header->num_textures, file);

	// Read in meshes
	for (i = 0; i < tmf->header->num_meshes; i++)
	{
		// Read in mesh header
		Rex_IO_FRead(&tmf->meshes[i].num_vertices, sizeof(rex_ushort), 1, file);
		Rex_IO_FRead(&tmf->meshes[i].num_quads, sizeof(rex_ushort), 1, file);

		// Fix endianness
		if (REX_LITTLE_ENDIAN)
		{
			tmf->meshes[i].num_vertices = Rex_EndianSwap_UShort(tmf->meshes[i].num_vertices);
			tmf->meshes[i].num_quads = Rex_EndianSwap_UShort(tmf->meshes[i].num_quads);
		}

		// Allocate memory for vertices and quads
		tmf->meshes[i].vertices = calloc(tmf->meshes[i].num_vertices, sizeof(tmf_vertex_t));
		tmf->meshes[i].quads = calloc(tmf->meshes[i].num_quads, sizeof(tmf_quad_t));

		// Read in mesh data
		Rex_IO_FRead(tmf->meshes[i].vertices, sizeof(tmf_vertex_t), tmf->meshes[i].num_vertices, file);
		Rex_IO_FRead(tmf->meshes[i].quads, sizeof(tmf_quad_t), tmf->meshes[i].num_quads, file);

		// Fix endianness
		if (REX_LITTLE_ENDIAN)
		{
			for (v = 0; v < tmf->meshes[i].num_vertices; v++)
			{
				tmf->meshes[i].vertices[v].coords[0] = Rex_EndianSwap_Int(tmf->meshes[i].vertices[v].coords[0]);
				tmf->meshes[i].vertices[v].coords[1] = Rex_EndianSwap_Int(tmf->meshes[i].vertices[v].coords[1]);
				tmf->meshes[i].vertices[v].coords[2] = Rex_EndianSwap_Int(tmf->meshes[i].vertices[v].coords[2]);
			}

			for (q = 0; q < tmf->meshes[i].num_quads; q++)
			{
				tmf->meshes[i].quads[q].normal[0] = Rex_EndianSwap_Int(tmf->meshes[i].quads[q].normal[0]);
				tmf->meshes[i].quads[q].normal[1] = Rex_EndianSwap_Int(tmf->meshes[i].quads[q].normal[1]);
				tmf->meshes[i].quads[q].normal[2] = Rex_EndianSwap_Int(tmf->meshes[i].quads[q].normal[2]);

				tmf->meshes[i].quads[q].indices[0] = Rex_EndianSwap_UShort(tmf->meshes[i].quads[q].indices[0]);
				tmf->meshes[i].quads[q].indices[1] = Rex_EndianSwap_UShort(tmf->meshes[i].quads[q].indices[1]);
				tmf->meshes[i].quads[q].indices[2] = Rex_EndianSwap_UShort(tmf->meshes[i].quads[q].indices[2]);
				tmf->meshes[i].quads[q].indices[3] = Rex_EndianSwap_UShort(tmf->meshes[i].quads[q].indices[3]);
			}
		}
	}

	Rex_IO_FClose(file);

	return tmf;
}
