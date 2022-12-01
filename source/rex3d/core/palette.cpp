// ========================================================
//
// FILE:			/source/core/palette.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Palette namespace implementation
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

// Palette namespace (private)
namespace Palette
{

//
//
// Functions
//
//

int Search(palette_t palette, int r, int g, int b)
{
	int i, pen, dist = INT_MAX;

	for (i = 256; i--;)
	{
		int rank = isqr(palette[i][0] - r) + isqr(palette[i][1] - g) + isqr(palette[i][2] - b);

		if (rank < dist)
		{
			pen  = i;
			dist = rank;
		}
	}

	return pen;
}

} // namespace Palette
