// ========================================================
//
// FILE:			/source/rex/modules/core/io/clock.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Clock header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

#ifndef __REX_CLOCK_H__
#define __REX_CLOCK_H__

//
//
// Rex namespace: Clock
//
//

namespace Rex
{

//
//
// Function prototypes
//
//

// Returns the number of ticks since the program started running
rex_int64 GetTicks64();

} // namespace Rex

#endif // __REX_CLOCK_H__
