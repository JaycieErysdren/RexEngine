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
// LAST EDITED:		December 7th, 2022
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

	uint8_t Search(palette_t palette, uint8_t r, uint8_t g, uint8_t b);
	void GenerateCLUT(palette_t palette);
}
