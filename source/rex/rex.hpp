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
// Handle math type (fixed or floating point)
//

// Default to fixed point math if not defined
#if !defined(BASED_FIXED) && !defined(BASED_FLOAT)
#define BASED_FIXED true
#define BASED_FLOAT false
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
#include "modules/core/colormap/colormap.hpp"

// Palettes
#include "modules/core/palette/palette.hpp"

// Surface
#include "modules/core/surface/picture.hpp"

// Console
#include "modules/core/console/console.hpp"

// Utils
#include "modules/core/utils/utils.hpp"

// Math
#include "modules/core/math/math.hpp"

//
// Hardware i/o modules
//

// DOS
#include "modules/hwio/dos/dos.hpp"

// VGA
#include "modules/hwio/vga/vga.hpp"

// VESA
#include "modules/hwio/vesa/vesa.hpp"

//
// Renderers
//

// Raycaster
#include "modules/render/raycast/raycast.hpp"

// Portals (2D)
#include "modules/render/portal2d/portal2d.hpp"

#endif // __REX_ENGINE_H__
