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
// LAST EDITED:		October 21st, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// TMF header type
typedef struct
{
	rex_ubyte type;
	rex_ubyte num_textures;
	rex_ubyte num_meshes;
	rex_ubyte reserved[5];
} tmf_header_t;

// TMF texture type
typedef struct
{
	rex_byte filename[13];
	rex_ubyte color[3];
} tmf_texture_t;

// TMF vertex type
typedef struct
{
	rex_int coords[3];
} tmf_vertex_t;

// TMF quad type
typedef struct
{
	rex_int normal[3];
	rex_ushort indices[4];
	rex_ubyte flags;
	rex_ubyte texture_id;
	rex_ubyte reserved[2];
} tmf_quad_t;

// TMF mesh type
typedef struct
{
	rex_ushort num_vertices;
	rex_ushort num_quads;
	rex_int _reserved;
	tmf_vertex_t *vertices;
	tmf_quad_t *quads;
} tmf_mesh_t;

// Load and process a Tank Engine TMF file. Returns an error code. (Formats/ReyeMe/tankengine_tmf.ksy)
br_actor *Rex_Formats_ReyeMe_TMF(rex_int operation, rex_byte *filename, br_actor *parent)
{
	// Define variables
	rex_int i, stride;

	// Define TMF structs
	tmf_header_t *tmf_header;
	tmf_mesh_t *tmf_mesh;
	tmf_texture_t *tmf_textures;

	// Define BRender structs
	br_actor *actor;
	br_model *model;

	// Open file pointer
	FILE *file = fopen(filename, "rb");

	// Check if file exists
	if (file == NULL) return NULL;

	// Allocate header memory
	tmf_header = calloc(1, sizeof(tmf_header_t));

	// Read in version header
	if (!fread(tmf_header, sizeof(tmf_header_t), 1, file)) return NULL;

	// Allocate texture & mesh memory
	tmf_textures = calloc(tmf_header->num_textures, sizeof(tmf_texture_t));
	tmf_mesh = calloc(1, sizeof(tmf_mesh_t));

	// Read in textures
	if (!fread(tmf_textures, sizeof(tmf_texture_t), tmf_header->num_textures, file)) return NULL;

	// Read in first mesh
	if (!fread(tmf_mesh, offsetof(tmf_mesh_t, _reserved), 1, file)) return NULL;

	// Fix endianness
	if (REX_LITTLE_ENDIAN)
	{
		tmf_mesh->num_vertices = Rex_EndianSwap_UShort(tmf_mesh->num_vertices);
		tmf_mesh->num_quads = Rex_EndianSwap_UShort(tmf_mesh->num_quads);
	}

	// Allocate first mesh data
	tmf_mesh->vertices = calloc(tmf_mesh->num_vertices, sizeof(tmf_vertex_t));
	tmf_mesh->quads = calloc(tmf_mesh->num_quads, sizeof(tmf_quad_t));

	// Read in first mesh data
	if (!fread(tmf_mesh->vertices, sizeof(tmf_vertex_t), tmf_mesh->num_vertices, file)) return NULL;
	if (!fread(tmf_mesh->quads, sizeof(tmf_quad_t), tmf_mesh->num_quads, file)) return NULL;

	// Fix endianness
	if (REX_LITTLE_ENDIAN)
	{
		for (i = 0; i < tmf_mesh->num_vertices; i++)
		{
			tmf_mesh->vertices[i].coords[0] = Rex_EndianSwap_Int(tmf_mesh->vertices[i].coords[0]);
			tmf_mesh->vertices[i].coords[1] = Rex_EndianSwap_Int(tmf_mesh->vertices[i].coords[1]);
			tmf_mesh->vertices[i].coords[2] = Rex_EndianSwap_Int(tmf_mesh->vertices[i].coords[2]);
		}

		for (i = 0; i < tmf_mesh->num_quads; i++)
		{
			tmf_mesh->quads[i].normal[0] = Rex_EndianSwap_Int(tmf_mesh->quads[i].normal[0]);
			tmf_mesh->quads[i].normal[1] = Rex_EndianSwap_Int(tmf_mesh->quads[i].normal[1]);
			tmf_mesh->quads[i].normal[2] = Rex_EndianSwap_Int(tmf_mesh->quads[i].normal[2]);

			tmf_mesh->quads[i].indices[0] = Rex_EndianSwap_UShort(tmf_mesh->quads[i].indices[0]);
			tmf_mesh->quads[i].indices[1] = Rex_EndianSwap_UShort(tmf_mesh->quads[i].indices[1]);
			tmf_mesh->quads[i].indices[2] = Rex_EndianSwap_UShort(tmf_mesh->quads[i].indices[2]);
			tmf_mesh->quads[i].indices[3] = Rex_EndianSwap_UShort(tmf_mesh->quads[i].indices[3]);
		}
	}

	// Allocate actor and model
	actor = BrActorAdd(parent, BrActorAllocate(BR_ACTOR_MODEL, NULL));
	model = BrModelAllocate(filename, 0, 0);
	model->flags |= BR_MODF_KEEP_ORIGINAL | BR_MODF_UPDATEABLE;

	// Allocate vertices
	model->nvertices = tmf_mesh->num_vertices;
	model->vertices = calloc(model->nvertices, sizeof(br_vertex));

	// Assign vertices
	for (i = 0; i < model->nvertices; i++)
	{
		model->vertices[i].p.v[0] = BR_SCALAR(tmf_mesh->vertices[i].coords[0] * 0.0001);
		model->vertices[i].p.v[1] = BR_SCALAR(tmf_mesh->vertices[i].coords[1] * 0.0001);
		model->vertices[i].p.v[2] = BR_SCALAR(tmf_mesh->vertices[i].coords[2] * 0.0001);

		//Rex_Log("%f %f %f", model->vertices[i].p.v[0], model->vertices[i].p.v[1], model->vertices[i].p.v[2]);
	}

	// Allocate faces
	model->nfaces = tmf_mesh->num_quads * 2;
	model->faces = calloc(model->nfaces, sizeof(br_face));

	// Assign faces
	for (i = 0, stride = 0; i < tmf_mesh->num_quads; i++)
	{
		// x, y, z and x, z, a

		// first triangle
		model->faces[stride].vertices[0] = tmf_mesh->quads->indices[0];
		model->faces[stride].vertices[1] = tmf_mesh->quads->indices[1];
		model->faces[stride].vertices[2] = tmf_mesh->quads->indices[2];
		model->faces[stride].smoothing = 0xFFFF;
		model->faces[stride].flags = 0;

		// second triangle
		model->faces[stride + 1].vertices[0] = tmf_mesh->quads->indices[0];
		model->faces[stride + 1].vertices[1] = tmf_mesh->quads->indices[2];
		model->faces[stride + 1].vertices[2] = tmf_mesh->quads->indices[3];
		model->faces[stride + 1].smoothing = 0xFFFF;
		model->faces[stride + 1].flags = 0;

		stride += 2;
	}

	// Assign model
	actor->model = model;
	BrModelAdd(actor->model);

	// Close file pointer
	fclose(file);

	// Free memory
	free(tmf_header);
	free(tmf_mesh->vertices);
	free(tmf_mesh->quads);
	free(tmf_mesh);

	return actor;
}
