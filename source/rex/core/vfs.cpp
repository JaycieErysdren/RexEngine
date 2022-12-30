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

// Add a new VFS handle
bool VFS_Add(string filename, vfs_format format)
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

	// create handle
	VFS_Handle handle;

	// open for reading
	handle.file_handle = fopen(filename.c_str(), "rb");
	if (handle.file_handle == NULL) return false;

	// set handle variables
	handle.filename = filename;
	handle.format = format;

	//
	// determine file type
	//

	char magic[16];
	fread(magic, sizeof(char), 16, handle.file_handle);

	if (handle.format == VFS_FORMAT_DETERMINE)
	{
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
	}

	//
	// fill up file list
	//

	switch (handle.format)
	{

		#ifdef WAIT_A_WHILE

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

		// ART
		case VFS_FORMAT_ART:
		{
			if (ART::CreateFileTable(&handle) == false)
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

		#endif

		// failed
		default:
			return false;
	}
}

// Close a VFS handle
bool VFS_Remove(string filename)
{
	rex_int i;

	for (i = 0; i < vfs_handles.size(); i++)
	{
		if (filename.compare(vfs_handles[i].filename) == 0)
		{
			fclose(vfs_handles[i].file_handle);
			vfs_handles.erase(vfs_handles.begin() + i);
			return true;
		}
	}

	// failed
	return false;
}

// Close all VFS handles
void VFS_Shutdown()
{
	rex_int i;

	for (i = 0; i < vfs_handles.size(); i++)
	{
		fclose(vfs_handles[i].file_handle);
	}

	vfs_handles.erase(vfs_handles.begin(), vfs_handles.end());
}

//
// File class
//

// Open file
bool File::Open(string fname)
{
	// assign variables
	filename = fname;
	ptr_offset = 0;

	//
	// check vfs
	//

	rex_int i, f;

	// loop through VFS handles
	for (i = vfs_handles.size() - 1; i > -1; i--)
	{
		for (f = 0; f < vfs_handles[i].files.size(); f++)
		{
			if (filename.compare(vfs_handles[i].files[f].filename) == 0)
			{
				filesize = vfs_handles[i].files[f].filesize;
				file_handle = NULL;

				return true;
			}
		}
	}

	//
	// well, it wasn't in the VFS, so try the HDD
	//

	file_handle = fopen(fname.c_str(), "rb");
	if (file_handle == NULL) return false;

	// get length of file
	fseek(file_handle, 0L, SEEK_END);
	filesize = ftell(file_handle);
	fseek(file_handle, 0L, SEEK_SET);

	return true;
}

// Close file
void File::Close()
{
	if (file_handle)
		fclose(file_handle);
}

// Get current offset in file
rex_int File::Tell()
{
	// HDD
	if (file_handle != NULL)
		return ftell(file_handle);

	// VFS
	return ptr_offset;
}

// Read data from file
bool File::Read(size_t size, size_t n, void *ptr)
{
	//
	// check VFS
	//

	rex_int bytes_read = size * n;

	// inside the bounds of the file
	if (Tell() + bytes_read <= filesize)
	{
		rex_int i, f;

		// check VFS handles for file
		for (i = vfs_handles.size() - 1; i > -1; i--)
		{
			for (f = 0; f < vfs_handles[i].files.size(); f++)
			{
				if (filename.compare(vfs_handles[i].files[f].filename) == 0)
				{
					rex_int ofs = vfs_handles[i].files[f].fileofs;

					// read file data
					fseek(vfs_handles[i].file_handle, ofs + ptr_offset, SEEK_SET);
					fread(ptr, size, n, vfs_handles[i].file_handle);

					ptr_offset += bytes_read;

					return true;
				}
			}
		}
	}
	else
	{
		return false;
	}

	//
	// check HDD
	//

	if (file_handle != NULL)
	{
		if (fread(ptr, size, n, file_handle) == n)
			return true;
	}

	// failed
	return false;
}

// Seek to position in file
bool File::Seek(rex_int offset, rex_int whence)
{
	// check HDD
	if (file_handle != NULL)
	{
		// check HDD
		if (fseek(file_handle, offset, whence) == 0)
			return true;
		else
			return false;
	}

	// check VFS
	switch (whence)
	{
		case SEEK_SET:
			ptr_offset = offset;
			return true;

		case SEEK_CUR:
			ptr_offset += offset;
			return true;

		case SEEK_END:
			ptr_offset = filesize;
			return true;

		default:
			return false;
	}
}

} // namespace Rex
