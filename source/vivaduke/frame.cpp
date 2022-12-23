// ========================================================
//
// FILE:			/source/vivaduke/frame.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Viva Duke Nukem
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Main game frame tick function
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

// vivaduke header
#include "vivaduke.hpp"

// Start of frame
void VDN_FrameStart()
{
	// Get start of frame time
	frame_start = Rex::GetTicks64();

	#if (REX_COMPILER == COMPILER_DJGPP)
		cycles = CYCLES * (frame_start - frame_end) / UCLOCKS_PER_SEC;
	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)
		cycles = CYCLES * (frame_start - frame_end) / CLOCKS_PER_SEC;
	#endif
}

// End of frame
void VDN_FrameEnd()
{
	// Get end of frame time
	#if (REX_COMPILER == COMPILER_DJGPP)
		frame_end = frame_end + cycles * UCLOCKS_PER_SEC / CYCLES;
	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)
		frame_end = frame_end + cycles * CLOCKS_PER_SEC / CYCLES;
	#endif
}
