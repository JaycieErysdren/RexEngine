// ========================================================
//
// FILE:			/source/math/mathtabl.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		MathTable class header
//
// LAST EDITED:		December 7th, 2022
//
// ========================================================

//
//
// MathTable class
//
//

class MathTable
{
	public:
		scalar_t cos[360];
		scalar_t sin[360];
		scalar_t tan[360];

		// Constructor
		MathTable();
};
