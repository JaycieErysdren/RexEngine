// ========================================================
//
// FILE:			/source/core/picture.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Utils namespace implementation
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

// Utils namespace (private)
namespace Utils
{

//
//
// Functions
//
//

void FileSkip(FILE *fp, int num_bytes)
{
	int i;
	for (i = 0; i < num_bytes; i++)
		fgetc(fp);
}

} // namespace Utils
