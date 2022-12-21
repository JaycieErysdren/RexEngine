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
// LAST EDITED:		December 19th, 2022
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
#define COMPILER_GCC 1		// GCC
#define COMPILER_MINGW 2	// MINGW
#define COMPILER_DJGPP 3	// DJGPP
#define COMPILER_WATCOM 4	// Open Watcom

// Potential target & host platforms
#define PLATFORM_NIX 1		// Linux
#define PLATFORM_WIN 2		// Windows
#define PLATFORM_DOS 3		// DOS

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

// Modern target headers
#if (REX_TARGET == PLATFORM_NIX) || (REX_TARGET == PLATFORM_WIN)

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

// Integer Math
#include "core/math_i.hpp"

// Fixed Math
#include "core/math_x.hpp"

// Math Table
#include "core/mathtabl.hpp"

// Memory Pool
#include "core/mempool.hpp"

// Bootstrap
#include "core/bootstrap.hpp"

// Log
#include "core/log.hpp"

// Clock
#include "core/clock.hpp"

// Graphics
#include "core/graphics.hpp"

// Surface
#include "core/surface.hpp"

// Scene
#include "core/scene.hpp"

// Render
#include "core/render.hpp"

// Keyboard
#include "core/keyboard.hpp"

// Mouse
#include "core/mouse.hpp"

// Console
#include "core/console.hpp"

// Camera
#include "core/camera.hpp"

//
// Renderers
//

// Raycast
#if RENDERER_RAYCAST
#include "render/raycast/render.hpp"
#endif

// Sector
#if RENDERER_SECTOR
#include "render/sector/render.hpp"
#endif

// Voxel
#if RENDERER_VOXEL
#include "render/voxel/voxel.hpp"
#endif

#endif // __REX_ENGINE_H__