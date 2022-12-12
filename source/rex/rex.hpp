// ========================================================
//
// FILE:			/source/rex/rex.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Rex Engine public header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

#ifndef __REX_ENGINE_H__
#define __REX_ENGINE_H__

//
//
// Host, target, and compiler options
//
//

// Potential compilers (Sorry clang)
#define COMPILER_GCC 0		// GCC
#define COMPILER_MINGW 1	// MINGW
#define COMPILER_DJGPP 2	// DJGPP
#define COMPILER_WATCOM 3	// Open Watcom

// Potential target & host platforms
#define PLATFORM_NIX 0		// Linux
#define PLATFORM_WIN 1		// Windows
#define PLATFORM_DOS 2		// DOS

//
// External headers
//

// Standard C++ headers
#include <iostream>
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

// DOS-specific standard headers
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

// Use standard namespace for convenience
using namespace std;

//
//
// Rex Engine public headers
//
//

// Types
#include "core/types.hpp"

// Math Table
#include "core/mathtabl.hpp"

// Bootstrap
#include "core/bootstrap.hpp"

// Clock
#include "core/clock.hpp"

// Graphics
#include "core/graphics.hpp"

// Surface
#include "core/surface.hpp"

// Keyboard
#include "core/keyboard.hpp"

// Mouse
#include "core/mouse.hpp"

// Console
#include "core/console.hpp"

#endif // __REX_ENGINE_H__
