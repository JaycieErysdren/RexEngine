// ========================================================
//
// FILE:			/source/core/inc/formats/wal.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		id Software WAL types.
//
// LAST EDITED:		October 23rd, 2022
//
// ========================================================

//
// Format Reference: Formats/id Software/wal.ksy
//

//
// WAL types
//

// WAL header
typedef struct
{
	rex_byte name[32];				// Texture name / identifier
	rex_uint width;					// Mipmap 0 width
	rex_uint height;				// Mipmap 0 height
	rex_int mipmap_offsets[4];		// Offsets of the 4 mipmaps
	rex_byte next_name[32];			// Name of next texture in animation
	rex_uint flags;					// Texture flags
	rex_uint contents;				// Texture contents
	rex_uint value;					// Texture user-defined value
} wal_header_t;

// WAL container
typedef struct
{
	// Saved to file
	wal_header_t *header;
	rex_buffer *mipmaps[4];

	// Not saved to file
	rex_uint widths[4];
	rex_uint heights[4];
} wal_t;

//
// WAL functions
//

// Load an id Software WAL file into memory. Returns a pointer to a WAL object.
wal_t *WAL_Load(rex_byte *filename);

// Free an id Software WAL object from memory.
void WAL_Free(wal_t *wal);

// Save an id Software WAL object to a file.
void WAL_Save(rex_byte *filename, wal_t *wal);

// Allocate an id Software WAL object.
wal_t *WAL_Allocate(rex_byte *name, rex_byte *next_name, rex_uint width, rex_uint height, rex_uint flags, rex_uint contents, rex_uint value);
