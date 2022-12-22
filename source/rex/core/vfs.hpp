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
// LAST EDITED:		December 21st, 2022
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
	VFS_FORMAT_PAK = 1,		// Quake PAK
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

		//
		// Functions
		//

		void Close();
};

//
// Functions
//

// Open or close a VFS archive
bool VFS_Open(string filename);
bool VFS_Close(string filename);

// Close all VFS archives
void VFS_CloseAll();

// Get the size of the file in the VFS. Returns -1 if the file doesn't exist
rex_int VFS_GetFileSize(string filename);

// Read file data into the specified buffer. Returns false if it failed.
bool VFS_ReadFile(string filename, size_t offset, size_t size, size_t n, void *ptr);

} // namespace Rex

#endif // __REX_VFS_H__
