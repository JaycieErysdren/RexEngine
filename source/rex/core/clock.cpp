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
// Global variables
//
//

rex_uint64 frame_start_time;
rex_uint64 frame_end_time;
rex_uint32 frame_cycles;
rex_uint32 frame_target_cycles;

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
rex_int64 GetTicks64()
{
	#if ((REX_TARGET == PLATFORM_DOS) && (MODULE_DOS))

		return DOS::TimerGet64();

	#elif (MODULE_SDL)

		return SDL_GetPerformanceCounter();

	#else

		return 0;

	#endif
}

void SetCycleRate(rex_int target)
{
	frame_target_cycles = target;
}

void CycleStart()
{
	// Get start of frame time
	frame_start_time = GetTicks64();

	#if (REX_PLATFORM == PLATFORM_DOS)

		#if (REX_COMPILER == COMPILER_DJGPP)
			frame_cycles = frame_target_cycles * (frame_start_time - frame_end_time) / UCLOCKS_PER_SEC;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			frame_cycles = frame_target_cycles * (frame_start_time - frame_end_time) / CLOCKS_PER_SEC;
		#endif

	#elif (MODULE_SDL)

		frame_cycles = frame_target_cycles * (frame_start_time - frame_end_time) / SDL_GetPerformanceFrequency();

	#endif
}

void CycleEnd()
{
	// Get end of frame time
	#if (REX_PLATFORM == PLATFORM_DOS)

		#if (REX_COMPILER == COMPILER_DJGPP)
			frame_end_time = frame_end_time + frame_cycles * UCLOCKS_PER_SEC / frame_target_cycles;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			frame_end_time = frame_end_time + frame_cycles * CLOCKS_PER_SEC / frame_target_cycles;
		#endif

	#elif (MODULE_SDL)

		frame_end_time = frame_end_time + frame_cycles * SDL_GetPerformanceFrequency() / frame_target_cycles;

	#endif
}

rex_uint32 Cycles()
{
	return frame_cycles;
}

} // namespace Rex
