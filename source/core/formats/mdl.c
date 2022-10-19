// ========================================================
//
// FILE:			/source/core/formats/mdl.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		id Software MDL files.
//
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// MDL magics
rex_byte_c mdl_magic_q1[4] = "IDPO";
rex_byte_c mdl_magic_q2[4] = "IDP2";

// MDL version header type
typedef struct
{
	rex_byte magic[4];
	rex_uint version;
} mdl_version_t;

// MDL model header type
typedef struct
{
	rex_float scale[3];
	rex_float translation[3];
	rex_float bounding_radius;
	rex_float eye_position[3];
	rex_uint num_skins;
	rex_uint skin_width;
	rex_uint skin_height;
	rex_uint num_vertices;
	rex_uint num_faces;
	rex_uint num_frames;
	rex_uint sync_type;
	rex_uint flags;
	rex_float size;
} mdl_header_t;

// MDL skin type
typedef struct
{
	rex_uint skin_type;
	rex_buffer *skin_pixels;
} mdl_skin_t;

// MDL texcoord type
typedef struct
{
	rex_int onseam;
	rex_int s;
	rex_int t;
} mdl_texcoord_t;

// Load and process an id MDL file. Returns an error code. (Formats/id Software/mdl.ksy)
rex_int Rex_Formats_idTech_MDL(rex_int operation, rex_byte *filename)
{
	// Allocate version header and mdl header structs
	mdl_header_t *mdl_header;
	mdl_version_t *mdl_version;
	
	rex_uint mdl_skin_type;
	rex_buffer *mdl_skin_pixels;

	rex_int i;
	rex_int num_pixels;

	// Open file pointer
	FILE *file = fopen(filename, "rb");

	// Check if file exists
	if (file == NULL) return REX_ERROR_FILE_NONE;

	// Allocate header memory
	mdl_version = calloc(1, sizeof(mdl_version_t));
	mdl_header = calloc(1, sizeof(mdl_header_t));

	// Read in version header
	if (!fread(mdl_version, sizeof(mdl_version_t), 1, file)) return REX_ERROR_FILE_READ;

	// Check file signature
	if (!memcmp(mdl_version->magic, mdl_magic_q2, 4)) return REX_ERROR_FMT_UNSUPPORTED;
	if (memcmp(mdl_version->magic, mdl_magic_q1, 4)) return REX_ERROR_FMT_BAD;

	// Check file version
	if (mdl_version->version != 6) return REX_ERROR_FMT_UNSUPPORTED;

	// Read in mdl header
	if (!fread(mdl_header, sizeof(mdl_header_t), 1, file)) return REX_ERROR_FILE_READ;

	// Number of pixels for pixel buffer
	num_pixels = mdl_header->skin_width * mdl_header->skin_height;

	// Allocate skin memory
	mdl_skin_pixels = malloc(sizeof(rex_ubyte) * num_pixels);

	// Read in mdl skins
	for (i = 0; i < mdl_header->num_skins; i++)
	{
		if (!fread(&mdl_skin_type, sizeof(rex_uint), 1, file)) return REX_ERROR_FILE_READ;
		if (!fread(mdl_skin_pixels, sizeof(rex_ubyte), num_pixels, file)) return REX_ERROR_FILE_READ;
	}

	// Read in mdl texcoords
	for (i = 0; i < mdl_header->num_vertices; i++)
	{

	}

	// Close file pointer
	fclose(file);

	// Return no error
	return REX_ERROR_NONE;
}
