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
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// MDL magics
rex_byte_c mdl_magic_quake[4] = "IDPO";
rex_byte_c mdl_magic_quake2[4] = "IDP2";

// Load an id Software MDL file into memory. Returns a pointer to an MDL object.
mdl_t *MDL_Load(rex_byte *filename)
{
	// Define variables
	rex_int i, num_pixels;
	mdl_t *mdl;
	FILE *file;

	// Open file pointer
	file = Rex_IO_FOpen(filename, "rb");

	// Allocate memory
	mdl = calloc(1, sizeof(mdl_t));
	mdl->version = calloc(1, sizeof(mdl_version_t));
	mdl->header = calloc(1, sizeof(mdl_header_t));

	// Read in version header
	Rex_IO_FRead(mdl->version, sizeof(mdl_version_t), 1, file);

	// Check file signature
	if (!memcmp(mdl->version->magic, mdl_magic_quake2, 4))
	{
		free(mdl->version);
		free(mdl->header);
		free(mdl);
		Rex_IO_FClose(file);
		Rex_MakeError("%s is a Quake 2 model file, which is currently not supported.", filename);
		return NULL;
	}

	// Check file signature
	if (memcmp(mdl->version->magic, mdl_magic_quake, 4))
	{
		free(mdl->version);
		free(mdl->header);
		free(mdl);
		Rex_IO_FClose(file);
		Rex_MakeError("%s has an unrecognized file signature %s.", filename, mdl->version->magic);
		return NULL;
	}

	// Check file version
	if (mdl->version->version == MDL_VERSION_QTEST)
	{
		free(mdl->version);
		free(mdl->header);
		free(mdl);
		Rex_IO_FClose(file);
		Rex_MakeError("%s is a QTest model file, which is currently not supported.", filename);
		return NULL;
	}
	else if (mdl->version->version == MDL_VERSION_QUAKE2)
	{
		free(mdl->version);
		free(mdl->header);
		free(mdl);
		Rex_IO_FClose(file);
		Rex_MakeError("%s is a Quake 2 model file, which is currently not supported.", filename);
		return NULL;
	}
	else if (mdl->version->version != MDL_VERSION_QUAKE)
	{
		free(mdl->version);
		free(mdl->header);
		free(mdl);
		Rex_IO_FClose(file);
		Rex_MakeError("%s has an unrecognized file version.", filename);
		return NULL;
	}

	// Read in header
	Rex_IO_FRead(mdl->header, sizeof(mdl_header_t), 1, file);

	// Calculate number of pixels
	num_pixels = mdl->header->skin_width * mdl->header->skin_height;

	// Allocate more memory
	mdl->skins = calloc(mdl->header->num_skins, sizeof(mdl_skin_t));
	mdl->texcoords = calloc(mdl->header->num_vertices, sizeof(mdl_texcoord_t));
	mdl->faces = calloc(mdl->header->num_faces, sizeof(mdl_face_t));
	mdl->frames = calloc(mdl->header->num_frames, sizeof(mdl_frame_t));

	// Read in skins
	for (i = 0; i < mdl->header->num_skins; i++)
	{
		Rex_IO_FRead(&mdl->skins[i].skin_type, sizeof(rex_uint), 1, file);

		if (mdl->skins[i].skin_type != 0)
		{
			Rex_IO_FClose(file);
			Rex_MakeError("%s skin %d has an unsupported type.", i);
			return NULL;
		}

		mdl->skins[i].skin_pixels = calloc(num_pixels, sizeof(rex_ubyte));
		Rex_IO_FRead(mdl->skins[i].skin_pixels, sizeof(rex_ubyte), num_pixels, file);
	}

	// Read in texcoords
	Rex_IO_FRead(mdl->texcoords, sizeof(mdl_texcoord_t), mdl->header->num_vertices, file);

	// Read in faces
	Rex_IO_FRead(mdl->faces, sizeof(mdl_face_t), mdl->header->num_faces, file);

	// Read in frames
	for (i = 0; i < mdl->header->num_frames; i++)
	{
		Rex_IO_FRead(&mdl->frames[i].frame_type, sizeof(rex_uint), 1, file);
		Rex_IO_FRead(&mdl->frames[i].min, sizeof(mdl_vertex_t), 1, file);
		Rex_IO_FRead(&mdl->frames[i].max, sizeof(mdl_vertex_t), 1, file);
		Rex_IO_FRead(&mdl->frames[i].name, sizeof(rex_byte), 16, file);

		if (mdl->frames[i].frame_type != 0)
		{
			Rex_IO_FClose(file);
			Rex_MakeError("%s frame %d has an unsupported type.", i);
			return NULL;
		}

		mdl->frames[i].vertices = calloc(mdl->header->num_vertices, sizeof(mdl_vertex_t));
		Rex_IO_FRead(mdl->frames[i].vertices, sizeof(mdl_vertex_t), mdl->header->num_vertices, file);
	}

	// Close file pointer
	Rex_IO_FClose(file);

	// Return pointer to MDL container
	return mdl;
}

// Free an id Software MDL object from memory.
void MDL_Free(mdl_t *mdl)
{
	rex_uint i, num_skins, num_frames;

	num_skins = mdl->header->num_skins;
	num_frames = mdl->header->num_frames;

	// Free version header
	free(mdl->version);

	// Free MDL header
	free(mdl->header);

	// Free skin pixels
	for (i = 0; i < num_skins; i++) free(mdl->skins[i].skin_pixels);

	// Free skins
	free(mdl->skins);

	// Free texcoords
	free(mdl->texcoords);

	// Free faces
	free(mdl->faces);

	// Free frame vertices
	for (i = 0; i < num_frames; i++) free(mdl->frames[i].vertices);

	// Free frames
	free(mdl->frames);

	free(mdl);
}
