// ========================================================
//
// FILE:			/source/rex3d/rex3d.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Rex3D main engine header
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

#ifndef __REX3D_H__
#define __REX3D_H__

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
#include <time.h>

// DJGPP headers
#include <dpmi.h>
#include <dos.h>
#include <go32.h>
#include <sys/nearptr.h>
#include <sys/farptr.h>

// Use standard namespace
using namespace std;

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

//
// Core
//

// Types
#include "core/types.hpp"

// Palettes
#include "core/palette.hpp"

// Pictures
#include "core/picture.hpp"

// Console
#include "core/console.hpp"

// Utilities
#include "core/utils.hpp"

//
// Math
//

// Integer math
#include "math/math_i.hpp"

// Fixed-point math
#include "math/math_x.hpp"

// Matrix
#include "math/matrix.hpp"

// Fixed-point sin table
#include "math/sintable.hpp"

//
// Hardware drivers
//

// DOS
#include "hwdrv/dos.hpp"

//
// Video drivers
//

// VGA
#include "viddrv/vga.hpp"

//
// Thirdparty I/O 
//

// raycastlib
#include "thrdprty/rcl.hpp"

#endif // __REX3D_H__
