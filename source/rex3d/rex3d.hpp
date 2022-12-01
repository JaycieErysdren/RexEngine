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
// LAST EDITED:		November 30th, 2022
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

// Standard C headers
#include <unistd.h>
#include <stdint.h>

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

// Math
#include "core/math.hpp"

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
// Renderers 
//

#ifdef RAYCASTER

// Raycasting
#include "rend/raycastr.hpp"

#endif

#ifdef EXPERIMENTAL

// Span rendering
#include "rend/spanner.hpp"

//
// World construction 
//

// Planes
#include "world/plane.hpp"

// Sectors
#include "world/sector.hpp"

// BSP
#include "world/bsp.hpp"

#endif

#endif // __REX3D_H__
