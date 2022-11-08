// ========================================================
//
// FILE:			/source/core/lib_io/brender.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		BRender helpers.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Generic primitive heap for z-buffer renderer
rex_ubyte brender_primitive_heap[1500 * 1024];
