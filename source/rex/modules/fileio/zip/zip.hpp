// ========================================================
//
// FILE:			/source/rex/modules/fileio/zip/zip.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		ZIP namespace: header
//
// LAST EDITED:		December 21st, 2022
//
// ========================================================

#ifndef __ZIP_H__
#define __ZIP_H__

//
//
// ZIP namespace: Header
//
//

namespace ZIP
{

//
//
// Functions
//
//

// Generate file table for the specified VFS handle
bool CreateFileTable(Rex::VFS_Handle *handle);

} // namespace ZIP

#endif // __ZIP_H__
