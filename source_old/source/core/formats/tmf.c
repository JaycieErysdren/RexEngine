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
// LAST EDITED:		October 23rd, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Load a Tank Engine TMF file into memory. Returns a pointer to a TMF object.
tmf_t *TMF_Load(rex_byte *filename)
{
	// Define variables
	rex_int i, v, q;
	tmf_t *tmf;
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
			Rex_EndianSwap_UShort(&tmf->meshes[i].num_vertices);
			Rex_EndianSwap_UShort(&tmf->meshes[i].num_quads);
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
				Rex_EndianSwap_Int(&tmf->meshes[i].vertices[v].coords[0]);
				Rex_EndianSwap_Int(&tmf->meshes[i].vertices[v].coords[1]);
				Rex_EndianSwap_Int(&tmf->meshes[i].vertices[v].coords[2]);
			}

			for (q = 0; q < tmf->meshes[i].num_quads; q++)
			{
				Rex_EndianSwap_Int(&tmf->meshes[i].quads[q].normal[0]);
				Rex_EndianSwap_Int(&tmf->meshes[i].quads[q].normal[1]);
				Rex_EndianSwap_Int(&tmf->meshes[i].quads[q].normal[2]);

				Rex_EndianSwap_UShort(&tmf->meshes[i].quads[q].indices[0]);
				Rex_EndianSwap_UShort(&tmf->meshes[i].quads[q].indices[1]);
				Rex_EndianSwap_UShort(&tmf->meshes[i].quads[q].indices[2]);
				Rex_EndianSwap_UShort(&tmf->meshes[i].quads[q].indices[3]);
			}
		}
	}

	Rex_IO_FClose(file);

	return tmf;
}

// Free a Tank Engine TMF object from memory.
void TMF_Free(tmf_t *tmf)
{
	// Variables
	rex_int i;

	// Free mesh data
	for (i = 0; i < tmf->header->num_meshes; i++)
	{
		free(tmf->meshes[i].quads);
		free(tmf->meshes[i].vertices);
	}

	// Free header
	free(tmf->header);

	// Free textures
	free(tmf->textures);

	// Free meshes
	free(tmf->meshes);

	// Free container
	free(tmf);
}
