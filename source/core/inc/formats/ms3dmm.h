// ========================================================
//
// FILE:			/source/core/inc/formats/3dmm.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Microsoft 3D Movie Maker types
//
// LAST EDITED:		October 25th, 2022
//
// ========================================================

//
// Format Reference: Formats/Microsoft/ms3dmm_chunk.ksy
//

//
// MS3DMM types
//

// MS3DMM header
typedef struct
{
	rex_byte magic[4];
	rex_byte author_program[4];
	rex_ushort chunk_version_current;
	rex_ushort chunk_version_minimum;
	rex_ushort byte_order;
	rex_ushort author_system;
	rex_uint len_file;
	rex_uint ofs_file_index;
	rex_uint len_file_index;
	rex_uint ofs_free_space_map;
	rex_uint len_free_space_map;
	rex_uint reserved[23];
} ms3dmm_header_t;

// MS3DMM file index
typedef struct
{
	rex_ushort byte_order;
	rex_ushort author_system;
	rex_uint num_chunks;
	rex_uint len_chunks;
	rex_uint reserved[2];
} ms3dmm_file_index_t;

// MS3DMM chunk index
typedef struct
{
	rex_uint ofs_chunk;
	rex_uint len_chunk;
} ms3dmm_chunk_index_t;

// MS3DMM container
typedef struct
{
	ms3dmm_header_t *header;
	ms3dmm_file_index_t *file_index;
	ms3dmm_chunk_index_t *chunk_index;
} ms3dmm_t;

//
// 3DMM functions
//

// Load a Microsoft 3D Movie Maker file into memory. Returns a pointer to an MS3DMM object.
ms3dmm_t *MS3DMM_Load(rex_byte *filename);

// Free an Microsoft 3D Movie Maker object from memory.
void MS3DMM_Free(ms3dmm_t *mdl);
