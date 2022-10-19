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
// LAST EDITED:		October 19th, 2022
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

// MDL face type
typedef struct
{
	rex_uint face_type;
	rex_uint vertex_indicies[3];
} mdl_face_t;

// MDL vertex type
typedef struct
{
	rex_ubyte coordinates[3];
	rex_ubyte normal_index;
} mdl_vertex_t;

// MDL frame type
typedef struct
{
	rex_uint frame_type;
	mdl_vertex_t min;
	mdl_vertex_t max;
	rex_byte name[16];
} mdl_frame_t;

// Load and process an id MDL file. Returns an error code. (Formats/id Software/mdl.ksy)
rex_int Rex_Formats_idTech_MDL(rex_int operation, rex_byte *filename)
{
	// Allocate version header and mdl header structs
	mdl_header_t *mdl_header;
	mdl_version_t *mdl_version;
	mdl_frame_t *mdl_frame;
	mdl_vertex_t *mdl_vertices;
	mdl_face_t *mdl_faces;

	rex_uint mdl_skin_type;
	rex_buffer *mdl_skin_pixels;

	rex_int i, stride;
	rex_int num_pixels;

	// Open file pointer
	FILE *file = fopen(filename, "rb");

	// Check if file exists
	if (file == NULL) return REX_ERROR_FILE_NONE;

	// Allocate header memory
	mdl_version = calloc(1, sizeof(mdl_version_t));
	mdl_header = calloc(1, sizeof(mdl_header_t));
	mdl_frame = calloc(1, sizeof(mdl_frame_t));

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
	mdl_skin_pixels = calloc(num_pixels, sizeof(rex_ubyte));

	// Read in mdl skins
	for (i = 0; i < mdl_header->num_skins; i++)
	{
		if (!fread(&mdl_skin_type, sizeof(rex_uint), 1, file)) return REX_ERROR_FILE_READ;

		if (mdl_skin_type != 0) return REX_ERROR_FMT_UNSUPPORTED;

		if (!fread(mdl_skin_pixels, sizeof(rex_ubyte), num_pixels, file)) return REX_ERROR_FILE_READ;
	}

	// Skip mdl texcoords
	for (i = 0; i < mdl_header->num_vertices; i++)
	{
		if (fseek(file, sizeof(mdl_texcoord_t), SEEK_CUR)) return REX_ERROR_FILE_READ;
	}

	// Allocate local faces buffer
	mdl_faces = calloc(mdl_header->num_faces, sizeof(mdl_face_t));

	// Read in MDL faces
	if (!fread(mdl_faces, sizeof(mdl_face_t), mdl_header->num_faces, file)) return REX_ERROR_FILE_READ;

	// Read first MDL frame
	if (!fread(mdl_frame, sizeof(mdl_frame_t), 1, file)) return REX_ERROR_FILE_READ;

	// Allocate local vertices buffer
	mdl_vertices = calloc(mdl_header->num_vertices, sizeof(mdl_vertex_t));

	// Read in MDL vertices
	if (!fread(mdl_vertices, sizeof(mdl_vertex_t), mdl_header->num_vertices, file)) return REX_ERROR_FILE_READ;

	// Allocate global vertex buffer
	gl_vertices_f = calloc(mdl_header->num_faces * 3, sizeof(rex_coord3f));
	num_gl_vertices_f = mdl_header->num_faces * 3;

	// Assign MDL vertices to global vertex buffer
	for (i = 0, stride = 0; i < mdl_header->num_faces; i++)
	{
		// Face vertex 0
		gl_vertices_f[stride].x = mdl_vertices[mdl_faces[i].vertex_indicies[0]].coordinates[0] * mdl_header->scale[0];
		gl_vertices_f[stride].y = mdl_vertices[mdl_faces[i].vertex_indicies[0]].coordinates[1] * mdl_header->scale[1];
		gl_vertices_f[stride].z = mdl_vertices[mdl_faces[i].vertex_indicies[0]].coordinates[2] * mdl_header->scale[2];
		// Face vertex 1
		gl_vertices_f[stride + 1].x = mdl_vertices[mdl_faces[i].vertex_indicies[1]].coordinates[0] * mdl_header->scale[0];
		gl_vertices_f[stride + 1].y = mdl_vertices[mdl_faces[i].vertex_indicies[1]].coordinates[1] * mdl_header->scale[1];
		gl_vertices_f[stride + 1].z = mdl_vertices[mdl_faces[i].vertex_indicies[1]].coordinates[2] * mdl_header->scale[2];
		// Face vertex 2
		gl_vertices_f[stride + 2].x = mdl_vertices[mdl_faces[i].vertex_indicies[2]].coordinates[0] * mdl_header->scale[0];
		gl_vertices_f[stride + 2].y = mdl_vertices[mdl_faces[i].vertex_indicies[2]].coordinates[1] * mdl_header->scale[1];
		gl_vertices_f[stride + 2].z = mdl_vertices[mdl_faces[i].vertex_indicies[2]].coordinates[2] * mdl_header->scale[2];
		// Iterate stride
		stride += 3;
	}

	// Close file pointer
	fclose(file);

	// Free MDL data memory
	free(mdl_header);
	free(mdl_version);
	free(mdl_frame);
	free(mdl_vertices);
	free(mdl_faces);

	// Return no error
	return REX_ERROR_NONE;
}
