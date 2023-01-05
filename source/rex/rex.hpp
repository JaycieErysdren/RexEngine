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
// LAST EDITED:		January 2nd, 2023
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
#define COMPILER_WATCOM		4 // Watcom
#define COMPILER_MSVC		5 // MSVC

// Platforms
#define PLATFORM_NULL		0 // Null
#define PLATFORM_SDL		1 // SDL
#define PLATFORM_DOS		2 // DOS
#define PLATFORM_WIN32		3 // Win32
#define PLATFORM_OS2		4 // OS/2
#define PLATFORM_LINUX32	5 // Linux32

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

	// Watcom headers
	#if (REX_COMPILER == COMPILER_WATCOM)

		#include <i86.h>
		#include <graph.h>

	#endif

#endif

// SDL target headers
#if (REX_TARGET == PLATFORM_SDL)

	// SDL2
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>

	// OpenGL
	#include <GLFW/glfw3.h>
	#include <GL/glut.h>

#endif

// Win32 target headers
#if (REX_TARGET == PLATFORM_WIN32)

	// Windows
	#include <windows.h>

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

// String
#include "core/string.hpp"

// Bootstrap
#include "core/bootstrp.hpp"

// Session
#include "core/session.hpp"

// Graphics
#include "core/graphics.hpp"

//
//
// RexMain
//
//

// The main program entry point. This needs to be defined by the end user.
int RexMain(int argc, char *argv[]);

#endif // __REX_ENGINE_H__
