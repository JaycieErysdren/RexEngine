// ========================================================
//
// FILE:			/source/core/inc/formats/mdl.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		id Software MDL types.
//
// LAST EDITED:		October 22nd, 2022
//
// ========================================================

// MDL magics
extern rex_byte_c mdl_magic_quake[4];
extern rex_byte_c mdl_magic_quake2[4];

// MDL version enumerations
enum mdl_versions
{
	MDL_VERSION_QTEST = 3,
	MDL_VERSION_QUAKE = 6,
	MDL_VERSION_QUAKE2 = 7,
};

// MDL version header
typedef struct
{
	rex_byte magic[4];
	rex_uint version;
} mdl_version_t;

// MDL header
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

// MDL skin
typedef struct
{
	rex_uint skin_type;
	rex_buffer *skin_pixels;
} mdl_skin_t;

// MDL texcoord
typedef struct
{
	rex_int onseam;
	rex_int s;
	rex_int t;
} mdl_texcoord_t;

// MDL face
typedef struct
{
	rex_uint face_type;
	rex_uint vertex_indicies[3];
} mdl_face_t;

// MDL vertex
typedef struct
{
	rex_ubyte coordinates[3];
	rex_ubyte normal_index;
} mdl_vertex_t;

// MDL frame
typedef struct
{
	rex_uint frame_type;
	mdl_vertex_t min;
	mdl_vertex_t max;
	rex_byte name[16];
	mdl_vertex_t *vertices;
} mdl_frame_t;

// MDL container
typedef struct
{
	mdl_version_t *version;
	mdl_header_t *header;
	mdl_skin_t *skins;
	mdl_texcoord_t *texcoords;
	mdl_face_t *faces;
	mdl_frame_t *frames;
} mdl_t;
