// ========================================================
//
// FILE:			/source/rex/modules/core/utils/utils.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Utils namespace implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex ENgine headers
#include "rex.hpp"

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
