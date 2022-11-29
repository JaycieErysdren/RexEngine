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
// LAST EDITED:		November 29th, 2022
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
// Handle math type (fixed or floating point)
//

// Default to fixed point
#if !defined(BASED_FIXED) && !defined(BASED_FLOAT)
#define BASED_FIXED true
#define BASED_FLOAT false
#endif

// Fixed or floating point types
#if BASED_FIXED

typedef int64_t				scalar_t;
typedef int16_t				fraction_t;
typedef uint16_t			ufraction_t;

#elif BASED_FLOAT

typedef float				scalar_t;
typedef float				fraction_t;
typedef float				ufraction_t;

#endif

//
// Rex3D Headers
//

// Math
#include "math.hpp"

// System I/O
#include "dos.hpp"
#include "vga.hpp"

// Graphics
#include "raycastr.hpp"

#endif // __REX3D_H__
