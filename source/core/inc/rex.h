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
// LAST EDITED:		November 7th, 2022
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
#include <errno.h>

//
// External headers
//

// SDL2 headers
#include <SDL2/SDL.h>

// BRender headers
#include <brender.h>
#include <brglrend.h>

// tinyfiledialogs module headers
#include <tinyfiledialogs.h>

// Nuklear module headers
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

// Main engine module
#ifndef __REX_ENGINE_H__
#define __REX_ENGINE_H__
#include "engine/types.h" // MUST BE FIRST!
#include "engine/bootstrap.h"		
#include "engine/device_io.h"
#include "engine/endian.h"
#include "engine/error.h"
#include "engine/file_io.h"
#include "engine/log.h"
#include "engine/math.h"
#include "engine/resfile.h"
#endif

// Formats module
#ifndef __REX_FORMATS_H__
#define __REX_FORMATS_H__
#include "formats/lev.h"
#include "formats/mdl.h"
#include "formats/ms3dmm.h"
#include "formats/pak.h"
#include "formats/tmf.h"
#include "formats/wal.h"
#endif

// LIB I/O module
#ifndef __REX_LIBIO_H__
#define __REX_LIBIO_H__
#include "lib_io/brender.h"
#include "lib_io/nuklear.h"
#include "lib_io/tinyfd.h"
#endif

// Physics module
#ifndef __REX_PHYSICS_H__
#define __REX_PHYSICS_H__
#include "physics/physics.h"
#endif

// Rendering module
#ifndef __REX_RENDERING_H__
#define __REX_RENDERING_H__
#include "rendering/palettes.h"
#include "rendering/window.h"
#endif
