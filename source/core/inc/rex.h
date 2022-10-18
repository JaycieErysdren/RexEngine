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
// LAST EDITED:		October 18th, 2022
//
// ========================================================

//
// Standard headers
//

#include <stdio.h>
#include <stdlib.h>

//
// SDL2 module headers
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//
// OpenGL module headers
//

#include <GL/gl.h>
#include <GL/glut.h>

//
// Rex Engine headers
//

#ifndef __TYPES_H__
#define __TYPES_H__
#include "types.h"
#endif

#ifndef __FORMATS_H__
#define __FORMATS_H__
#include "formats.h"
#endif

#ifndef __LOGIC_H__
#define __LOGIC_H__
#include "logic.h"
#endif

#ifndef __IO_H__
#define __IO_H__
#include "io.h"
#endif

#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include "utilities.h"
#endif

#ifndef __RENDERING_H__
#define __RENDERING_H__
#include "rendering.h"
#endif
