// ========================================================
//
// FILE:			/source/core/colormap.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Colormap namespace header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

//
//
// Colormap namespace
//
//

// Colormap namespace definition (public)
namespace Colormap
{
	//
	//
	// Types
	//
	//


	//
	//
	// Functions
	//
	//

	uint8_t Lookup(uint8_t index, int light);
	void Generate(string palette_filename, int num_fullbrights);
	void Save(string filename);
	void Load(string filename);
}
