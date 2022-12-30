// ========================================================
//
// FILE:			/source/rex/core/vfs.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: VFS header
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

#ifndef __REX_VFS_H__
#define __REX_VFS_H__

//
//
// Rex namespace: VFS
//
//

namespace Rex
{

//
//
// Enums
//
//

typedef enum
{
	VFS_FORMAT_DETERMINE,	// Determine format for me
	VFS_FORMAT_PAK,			// Quake PAK
	VFS_FORMAT_ZIP,			// ZIP / Quake PK3
	VFS_FORMAT_GRP,			// BUILD GRP
	VFS_FORMAT_ART,			// BUILD ART
	VFS_FORMAT_IWAD,		// DOOM IWAD
	VFS_FORMAT_PWAD,		// DOOM PWAD
	VFS_FORMAT_WAD2,		// Quake WAD2
	VFS_FORMAT_WAD3,		// Half-Life WAD3
} vfs_format;

//
//
// Classes
//
//

// File class
class File
{
	public:

		//
		// Variables
		//

		// File name
		rex_string filename;

		// File size (bytes)
		rex_int32 filesize;

		//
		// Functions
		//

		// Open file
		bool Open(string fname);

		// Close file
		void Close();

		// Get current offset in file
		rex_int Tell();

		// Read bytes from file
		bool Read(size_t size, size_t n, void *ptr);

		// Seek
		bool Seek(rex_int offset, rex_int whence);

	private:

		//
		// Variables
		//

		// Where to read from in the file
		rex_int32 ptr_offset;

		// FILE handle
		FILE *file_handle;
};

// VFS file
class VFS_File
{
	public:

		//
		// Variables
		//

		rex_string filename;
		rex_int32 filesize;
		rex_int32 fileofs;
};

// VFS handle
class VFS_Handle
{
	public:

		//
		// Variables
		//

		rex_string filename;
		vfs_format format;
		vector<VFS_File> files;
		FILE *file_handle;
};

//
// Functions
//

// Add a new VFS handle
bool VFS_Add(string filename, vfs_format format);

// Close a VFS handle
bool VFS_Remove(string filename);

// Close all VFS handles
void VFS_Shutdown();

} // namespace Rex

#endif // __REX_VFS_H__
