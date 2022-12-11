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
	// Classes
	//
	//

	class VidInfo
	{
		public:
			int width;
			int height;
			int bpp;
			int bytes_per_row;
	};

	//
	//
	// Functions
	//
	//

	//
	// Bootstrap
	//

	bool Initialize(int w, int h, int bpp);
	void Shutdown();

	//
	// Helpers
	//

	void SetPalette(string filename);
	VidInfo GetVidInfo();
	void PlaceBuffer(int8_t *buffer, int buffer_size);

}
