// ========================================================
//
// FILE:			/source/rex/core/vfs.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: VFS implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: VFS
//
//

namespace Rex
{

//
//
// Globals
//
//

// VFS file handles
vector<VFS_Handle> vfs_handles;

//
//
// Functions
//
//

//
// VFS handles
//

// Open a new VFS handle
bool VFS_Open(string filename)
{
	if (filename.empty()) return false;

	rex_int i;

	// check if there's already a handle for this file
	for (i = 0; i < vfs_handles.size(); i++)
	{
		if (filename.compare(vfs_handles[i].filename) == 0)
		{
			return false;
		}
	}

	VFS_Handle handle;

	// open for reading and writing
	handle.file_handle = fopen(filename.c_str(), "rb");
	if (handle.file_handle == NULL) return false;

	// set handle variables
	handle.filename = filename;

	//
	// determine file type
	//

	char magic[16];
	fread(magic, sizeof(char), 16, handle.file_handle);

	// PAK
	if (memcmp(magic, "PACK", 4) == 0) handle.format = VFS_FORMAT_PAK;

	// ZIP
	else if (memcmp(magic, "PK", 2) == 0) handle.format = VFS_FORMAT_ZIP;

	// GRP
	else if (memcmp(magic, "KenSilverman", 12) == 0) handle.format = VFS_FORMAT_GRP;

	// IWAD
	else if (memcmp(magic, "IWAD", 4) == 0) handle.format = VFS_FORMAT_IWAD;

	// PWAD
	else if (memcmp(magic, "PWAD", 4) == 0) handle.format = VFS_FORMAT_PWAD;

	// WAD2
	else if (memcmp(magic, "WAD2", 4) == 0) handle.format = VFS_FORMAT_WAD2;

	// WAD3
	else if (memcmp(magic, "WAD3", 4) == 0) handle.format = VFS_FORMAT_WAD3;

	// fill up file list
	switch (handle.format)
	{
		// PAK
		case VFS_FORMAT_PAK:
		{
			if (PAK::CreateFileTable(&handle) == false)
				return false;

			vfs_handles.push_back(handle);
			return true;
		}

		// ZIP / PK3
		case VFS_FORMAT_ZIP:
		{
			if (ZIP::CreateFileTable(&handle) == false)
				return false;

			vfs_handles.push_back(handle);
			return true;
		}

		// GRP
		case VFS_FORMAT_GRP:
		{
			if (GRP::CreateFileTable(&handle) == false)
				return false;

			vfs_handles.push_back(handle);
			return true;
		}

		// WAD
		case VFS_FORMAT_IWAD:
		case VFS_FORMAT_PWAD:
		case VFS_FORMAT_WAD2:
		case VFS_FORMAT_WAD3:
		{
			if (WAD::CreateFileTable(&handle) == false)
				return false;

			vfs_handles.push_back(handle);
			return true;
		}

		// invalid
		default:
		{
			return false;
		}
	}
}

// Close a VFS handle
bool VFS_Close(string filename)
{
	rex_int i;

	for (i = 0; i < vfs_handles.size(); i++)
	{
		if (filename.compare(vfs_handles[i].filename) == 0)
		{
			vfs_handles[i].Close();
			return true;
		}
	}

	return false;
}

// Close all VFS handles
void VFS_CloseAll()
{
	rex_int i;

	for (i = 0; i < vfs_handles.size(); i++)
	{
		vfs_handles[i].Close();
	}
}

//
// VFS files
//

// Get the size of the file in the VFS. Returns -1 if the file doesn't exist
rex_int VFS_GetFileSize(string filename)
{
	rex_int i, f, size;
	bool file_found = false;

	// find out if file exists in the VFS
	for (i = vfs_handles.size() - 1; i > -1; i--)
	{
		for (f = 0; f < vfs_handles[i].files.size(); f++)
		{
			if (filename.compare(vfs_handles[i].files[f].filename) == 0)
			{
				return vfs_handles[i].files[f].filesize;
			}
		}
	}

	return -1;
}

// Read file data into the specified buffer. Returns false if it failed.
bool VFS_ReadFile(string filename, size_t offset, size_t size, size_t n, void *ptr)
{
	rex_int i, f;
	bool file_found = false;

	rex_int han;
	rex_int ofs;
	rex_int len;

	// find out if file exists in the VFS
	for (i = vfs_handles.size() - 1; i > -1; i--)
	{
		if (file_found == true) break;

		for (f = 0; f < vfs_handles[i].files.size(); f++)
		{
			if (filename.compare(vfs_handles[i].files[f].filename) == 0)
			{
				han = i;
				ofs = vfs_handles[i].files[f].fileofs;
				len = vfs_handles[i].files[f].filesize;

				file_found = true;
				break;
			}
		}
	}

	// couldn't find the file
	if (file_found == false)
		return false;

	// sanity checks
	if (offset > len) return false;
	if (offset + (size * n) > len) size = offset - len;

	// seek to file
	fseek(vfs_handles[han].file_handle, ofs + offset, SEEK_SET);

	// read file data
	fread(ptr, size, n, vfs_handles[han].file_handle);

	return true;
}

//
// VFS_Handle class
//

void VFS_Handle::Close()
{
	if (file_handle)
		fclose(file_handle);
}

} // namespace Rex
