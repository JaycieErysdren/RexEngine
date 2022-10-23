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
	wal_header_t *header;			// File header
	rex_uint widths[4];				// Precalculated mipmap widths
	rex_uint heights[4];			// Precalculated mipmap heights
	rex_buffer *pixels[4];			// Mipmap pixel buffers
} wal_t;

//
// WAL functions
//

// Load an id Software WAL file into memory. Returns a pointer to a WAL container.
wal_t *WAL_Load(rex_byte *filename);

// Free an id Software WAL file from memory.
void WAL_Free(wal_t *wal);
