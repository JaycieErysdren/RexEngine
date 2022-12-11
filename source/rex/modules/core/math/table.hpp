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
		rex_scalar cos[360];
		rex_scalar sin[360];
		rex_scalar tan[360];

		// Constructor
		MathTable();
};
