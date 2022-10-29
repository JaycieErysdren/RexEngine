// ========================================================
//
// FILE:			/source/core/inc/rex.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Main Rex Engine header.
//
// LAST EDITED:		October 29th, 2022
//
// ========================================================

//
// Standard headers
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

//
// SDL2 module headers
//

#include <SDL2/SDL.h>

//
// BRender module headers
//

#include <brender.h>
#include <brglrend.h>

//
// tinyfiledialogs module headers
//

#include <tinyfiledialogs.h>

//
// Nuklear module headers
//

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_SOFTWARE_FONT
#include <nuklear.h>

//
// Rex Engine headers
//

#ifndef __REX_TYPES_H__
#define __REX_TYPES_H__
#include "rex_types.h"
#endif

#ifndef __REX_FORMATS_H__
#define __REX_FORMATS_H__
#include "rex_formats.h"
#endif

#ifndef __REX_LOGIC_H__
#define __REX_LOGIC_H__
#include "rex_logic.h"
#endif

#ifndef __REX_IO_H__
#define __REX_IO_H__
#include "rex_io.h"
#endif

#ifndef __REX_UTILITIES_H__
#define __REX_UTILITIES_H__
#include "rex_utilities.h"
#endif

#ifndef __REX_PHYSICS_H__
#define __REX_PHYSICS_H__
#include "rex_physics.h"
#endif

#ifndef __REX_RENDERING_H__
#define __REX_RENDERING_H__
#include "rex_rendering.h"
#endif
