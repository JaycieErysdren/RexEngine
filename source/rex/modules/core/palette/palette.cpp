// ========================================================
//
// FILE:			/source/rex/modules/core/palette/palette.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Palette namespace implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine headers
#include "rex.hpp"

// Palette namespace (private)
namespace Palette
{

//
//
// Globals
//
//

clut_t blender;

//
//
// Functions
//
//

uint8_t Search(palette_t palette, uint8_t r, uint8_t g, uint8_t b)
{
	int i, dist = INT_MAX;
	uint8_t pen;

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

//
// CLUT
//

void GenerateCLUT(palette_t palette)
{
	int x, y, r;

	for (x = 255; x--;)
	{
		for (y = 255; y--;)
		{
			r = (palette[x][0] + palette[y][0]) >> 1;

			blender[y][x] = Search(palette,
				(palette[x][0] + palette[y][0]) >> 1,
				(palette[x][1] + palette[y][1]) >> 1,
				(palette[x][2] + palette[y][2]) >> 1);
		}
	}
}

} // namespace Palette
