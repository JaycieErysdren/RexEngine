// ========================================================
//
// FILE:			/source/rex/modules/core/io/clock.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Clock implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Clock
//
//

namespace Rex
{

//
//
// Functions
//
//

// Returns the number of ticks since the program started running
int64_t GetTicks64()
{
	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_DOS)

		return DOS::TimerGet64();

	#else

		return -1;

	#endif
}

} // namespace Rex
