// ========================================================
//
// FILE:			/source/core/palette.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Palette namespace header
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

//
//
// Palette namespace
//
//

// Palette namespace definition (public)
namespace Palette
{
	//
	//
	// Types
	//
	//

	typedef uint8_t clut_t[256][256];
	typedef uint8_t palette_t[256][3];

	//
	//
	// Functions
	//
	//

	int Search(palette_t palette, int r, int g, int b);
}
