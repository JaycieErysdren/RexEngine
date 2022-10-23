// ========================================================
//
// FILE:			/source/core/inc/formats/tmf.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Tank Engine TMF types.
//
// LAST EDITED:		October 22nd, 2022
//
// ========================================================

// TMF header
typedef struct
{
	rex_ubyte type;
	rex_ubyte num_textures;
	rex_ubyte num_meshes;
	rex_ubyte reserved[5];
} tmf_header_t;

// TMF texture
typedef struct
{
	rex_byte filename[13];
	rex_ubyte color[3];
} tmf_texture_t;

// TMF vertex
typedef struct
{
	rex_int coords[3];
} tmf_vertex_t;

// TMF quad
typedef struct
{
	rex_int normal[3];
	rex_ushort indices[4];
	rex_ubyte flags;
	rex_ubyte texture_id;
	rex_ubyte reserved[2];
} tmf_quad_t;

// TMF mesh
typedef struct
{
	rex_ushort num_vertices;
	rex_ushort num_quads;
	tmf_vertex_t *vertices;
	tmf_quad_t *quads;
} tmf_mesh_t;

// TMF container
typedef struct
{
	tmf_header_t *header;
	tmf_texture_t *textures;
	tmf_mesh_t *meshes;
} tmf_t;
