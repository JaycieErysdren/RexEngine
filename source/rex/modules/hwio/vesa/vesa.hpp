// ========================================================
//
// FILE:			/source/rex/modules/hwio/vesa/vesa.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		VESA namespace header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

//
//
// Text macros
//
//

//
//
// VESA namespace
//
//

// VESA namespace definition (public)
namespace VESA
{
	//
	//
	// Functions
	//
	//

	//
	// Bootstrap
	//

	bool Initialize(rex_int32 w, rex_int32 h, rex_int32 bpp);
	void Shutdown();

	//
	// Helpers
	//

	void SetPalette(string filename);
	void GetVidInfo(rex_int32 *w, rex_int32 *h, rex_int32 *bpp, rex_int32 *bytes_per_row);
	void PlaceBuffer(rex_uint8 *buffer, rex_int32 buffer_size);

}
