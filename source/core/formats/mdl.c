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
	mdl_texcoord_t *mdl_texcoords;

	rex_uint mdl_skin_type;
	rex_ubyte *mdl_skin_pixels;

	rex_int i, v, x, y, stride;
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

	// Allocate the global pixelmap
	global_model_test_texture = BrPixelmapAllocate(BR_PMT_RGB_888, mdl_header->skin_width, mdl_header->skin_height, NULL, 0);

	// This is a shit method.
	for (y = 0; y < mdl_header->skin_height; y++)
	{
		for (x = 0; x < mdl_header->skin_width; x++)
		{
			rex_int pos = (y * mdl_header->skin_width) + x;
			rex_rgb color = palette_quake[mdl_skin_pixels[pos]];

			BrPixelmapPixelSet(global_model_test_texture, x, y, BR_COLOUR_RGB(color.b, color.g, color.r));
		}
	}

	global_model_test_texture->identifier = "quake_model_texture";

	BrMapAdd(global_model_test_texture);

	BrPixelmapSave("checkerboard.pix", global_model_test_texture);

	// Allocate local texcoords buffer
	mdl_texcoords = calloc(mdl_header->num_vertices, sizeof(mdl_texcoord_t));

	// Read in MDL texcoords
	if (!fread(mdl_texcoords, sizeof(mdl_texcoord_t), mdl_header->num_vertices, file)) return REX_ERROR_FILE_READ;

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

	//
	// MAKE ROOM EVERYONE, I'M GOING TO TRY AND MAKE A BRENDER MODEL
	//

	// Allocate the global model (this will be removed shortly)
	global_model_test = BrModelAllocate("quake_model", 0, 0);
	global_model_test->flags |= BR_MODF_KEEP_ORIGINAL;
	global_model_test->flags |= BR_MODF_UPDATEABLE;

	// Allocate vertices
	global_model_test->vertices = calloc(mdl_header->num_vertices, sizeof(br_vertex));
	global_model_test->nvertices = mdl_header->num_vertices;

	// Allocate faces
	global_model_test->faces = calloc(mdl_header->num_faces, sizeof(br_face));
	global_model_test->nfaces = mdl_header->num_faces;

	// Shove in the vertices
	for (i = 0; i < mdl_header->num_vertices; i++)
	{
		// Assign scaled and translated vertex positions to the vectors
		global_model_test->vertices[i].p.v[0] = BR_SCALAR(mdl_vertices[i].coordinates[0] * mdl_header->scale[0] + mdl_header->translation[0]);
		global_model_test->vertices[i].p.v[1] = BR_SCALAR(mdl_vertices[i].coordinates[1] * mdl_header->scale[1] + mdl_header->translation[1]);
		global_model_test->vertices[i].p.v[2] = BR_SCALAR(mdl_vertices[i].coordinates[2] * mdl_header->scale[2] + mdl_header->translation[2]);
	}

	// Shove in the faces
	for (i = 0; i < mdl_header->num_faces; i++)
	{
		rex_ushort vi0 = (rex_ushort)mdl_faces[i].vertex_indicies[0];
		rex_ushort vi1 = (rex_ushort)mdl_faces[i].vertex_indicies[1];
		rex_ushort vi2 = (rex_ushort)mdl_faces[i].vertex_indicies[2];

		// Put the vertex indices in it
		global_model_test->faces[i].vertices[0] = vi0;
		global_model_test->faces[i].vertices[1] = vi1;
		global_model_test->faces[i].vertices[2] = vi2;

		// Max smoothing, no flags
		global_model_test->faces[i].smoothing = 0xFFFF;
		global_model_test->faces[i].flags = 0;

		//
		// AGKHSDHGSD'GSDG
		//

		br_vertex v0 = global_model_test->vertices[vi0];
		br_vertex v1 = global_model_test->vertices[vi1];
		br_vertex v2 = global_model_test->vertices[vi2];

		br_vector2 uv0 = {mdl_texcoords[vi0].s, mdl_texcoords[vi0].t};
		br_vector2 uv1 = {mdl_texcoords[vi1].s, mdl_texcoords[vi1].t};
		br_vector2 uv2 = {mdl_texcoords[vi2].s, mdl_texcoords[vi2].t};

		if (mdl_faces[i].face_type == 0 && mdl_texcoords[vi0].onseam != 0) uv0.v[0] += mdl_header->skin_width * 0.5f;
		if (mdl_faces[i].face_type == 0 && mdl_texcoords[vi1].onseam != 0) uv1.v[0] += mdl_header->skin_width * 0.5f;
		if (mdl_faces[i].face_type == 0 && mdl_texcoords[vi2].onseam != 0) uv2.v[0] += mdl_header->skin_width * 0.5f;

		uv0.v[0] = (uv0.v[0] + 0.5f) / mdl_header->skin_width;
		uv0.v[1] = (uv0.v[1] + 0.5f) / mdl_header->skin_height;

		uv1.v[0] = (uv1.v[0] + 0.5f) / mdl_header->skin_width;
		uv1.v[1] = (uv1.v[1] + 0.5f) / mdl_header->skin_height;

		uv2.v[0] = (uv2.v[0] + 0.5f) / mdl_header->skin_width;
		uv2.v[1] = (uv2.v[1] + 0.5f) / mdl_header->skin_height;

		Rex_Log("%f %f", uv0.v[0], uv0.v[1]);
		Rex_Log("%f %f", uv1.v[0], uv1.v[1]);
		Rex_Log("%f %f", uv2.v[0], uv2.v[1]);
		Rex_Log("====================");

		v0.map = uv0;
		v1.map = uv1;
		v2.map = uv2;

		global_model_test->vertices[vi0] = v0;
		global_model_test->vertices[vi1] = v1;
		global_model_test->vertices[vi2] = v2;

		// this sucks
	}

	BrModelAdd(global_model_test);

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
