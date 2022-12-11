// ========================================================
//
// FILE:			/source/rex/modules/core/math/table.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		MathTable class header
//
// LAST EDITED:		December 11th, 2022
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
