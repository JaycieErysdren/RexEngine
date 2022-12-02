// ========================================================
//
// FILE:			/source/rex3d/rend/spanner.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Spanner namespace header
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

//
//
// Spanner namespace
//
//

// Spanner namespace definition (public)
namespace Spanner
{
	//
	//
	// Functions
	//
	//

	// Reset
	void ClearClippingBuffers();
	void ClearSpanBuffer();

	// Rendering
	int InsertSpan(Picture::pic_t *dst, int16_t x1, int16_t x2, int16_t y1a, int16_t ny1a, int16_t y2a, int16_t ny2a, int16_t y1b, int16_t ny1b, int16_t y2b, int16_t ny2b,
					uint8_t ceil_col, uint8_t upper_col, uint8_t wall_col, uint8_t lower_col, uint8_t floor_col,
					uint8_t insert, uint8_t dither_wall, uint8_t dither_floor);
}
