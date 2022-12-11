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
// DESCRIPTION:		Rex Engine main header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

#ifndef __REX_ENGINE_H__
#define __REX_ENGINE_H__

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

// Potential compilers
#define COMPILER_GCC 0
#define COMPILER_MINGW 1
#define COMPILER_DJGPP 2
#define COMPILER_WATCOM 3

// Potential target & host platforms
#define PLATFORM_NIX 0	// Linux
#define PLATFORM_WIN 1	// Windows
#define PLATFORM_DOS 2	// DOS

#if (REX_TARGET == PLATFORM_DOS)

	// Standard DOS headers
	#include <dos.h>

	// DJGPP headers
	#if (REX_COMPILER == COMPILER_DJGPP)

		#include <dpmi.h>
		#include <go32.h>
		#include <sys/nearptr.h>
		#include <sys/farptr.h>

	#endif

	// Open Watcom headers
	#if (REX_COMPILER == COMPILER_WATCOM)

		// add me

	#endif

#endif

// Use standard namespace
using namespace std;

//
//
// Thirdparty headers
//
//

//
//
// Rex3D forward definitions
//
//

//
// Math type (fixed or floating point)
//

// Default to fixed point if not defined
#if !defined(MATH_FIXED) && !defined(MATH_FLOAT)
#define MATH_FIXED 1
#define MATH_FLOAT 0
#endif

//
//
// Rex3D Headers
//
//

// Types
#include "rextypes.hpp"

//
// Core modules
//

// Colormaps
#if MODULE_COLORMAP
#include "modules/core/colormap/colormap.hpp"
#endif

// Palette
#if MODULE_PALETTE
#include "modules/core/palette/palette.hpp"
#endif

// Surface
#if MODULE_SURFACE
#include "modules/core/surface/picture.hpp"
#endif

// Console
#if MODULE_CONSOLE
#include "modules/core/console/console.hpp"
#endif

// Utils
#if MODULE_UTILS
#include "modules/core/utils/utils.hpp"
#endif

// Math
#if MODULE_MATH
#include "modules/core/math/math.hpp"
#endif

//
// Hardware I/O modules
//

// DOS
#if MODULE_DOS
#include "modules/hwio/dos/dos.hpp"
#endif

// VGA
#if MODULE_VGA
#include "modules/hwio/vga/vga.hpp"
#endif

// VESA
#if MODULE_VESA
#include "modules/hwio/vesa/vesa.hpp"
#endif

//
// Renderers
//

// Raycast
#if MODULE_RAYCAST
#include "modules/render/raycast/raycast.hpp"
#endif

// Portal2D
#if MODULE_PORTAL2D
#include "modules/render/portal2d/portal2d.hpp"
#endif

#endif // __REX_ENGINE_H__
