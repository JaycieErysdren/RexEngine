// ========================================================
//
// FILE:			/source/rex/rex.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine public header
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

#ifndef __REX_ENGINE_H__
#define __REX_ENGINE_H__

//
//
// Host, target, and compiler options
//
//

// Compilers
#define COMPILER_GCC		1 // GCC
#define COMPILER_MINGW		2 // MINGW
#define COMPILER_DJGPP		3 // DJGPP
#define COMPILER_WATCOM		4 // Open Watcom
#define COMPILER_MSVC		5 // MSVC

// Platforms
#define PLATFORM_NULL		0 // Null
#define PLATFORM_SDL		1 // SDL
#define PLATFORM_DOS		2 // DOS

//
// External headers
//

// Standard C++ headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdarg>

// Standard C headers
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// DOS target headers
#if (REX_TARGET == PLATFORM_DOS)

	// Standard DOS headers
	#include <dos.h>
	#include <io.h>
	#include <conio.h>

	// DJGPP headers
	#if (REX_COMPILER == COMPILER_DJGPP)

		#include <dpmi.h>
		#include <go32.h>
		#include <sys/nearptr.h>
		#include <sys/farptr.h>

	#endif

	// Open Watcom headers
	#if (REX_COMPILER == COMPILER_WATCOM)

		#include <i86.h>
		#include <graph.h>

	#endif

#endif

// SDL target headers
#if (REX_TARGET == PLATFORM_SDL)

	// SDL2
	#include <SDL2/SDL.h>

#endif

// Use standard namespace for convenience
using namespace std;

//
//
// Rex Engine thirdparty headers
//
//

#include "thrdprty/cjson/cJSON.h"

//
//
// Rex Engine public headers
//
//

// Types
#include "core/types.hpp"

// Bootstrap
#include "core/bootstrp.hpp"

#endif // __REX_ENGINE_H__
