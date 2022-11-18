// ========================================================
//
// FILE:			/source2/include/rex.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Rex Engine header.
//
// LAST EDITED:		November 18th, 2022
//
// ========================================================

//
// Standard Headers
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//
// Standard Headers (platform specific)
//

// DOS
#if defined __DJGPP__
#include <conio.h>
#include <dos.h>
#include <io.h>
#include <pc.h>
#include <dpmi.h>
#endif

//
// Rex Engine Headers
//

// Rex Engine Types
#ifndef __REX_TYPES_H__
#define __REX_TYPES_H__
#include "rextypes.h"
#endif

//
// Rex Engine DOS interface
//

#if defined __DJGPP__
#ifndef __REX_DOS_H__
#define __REX_DOS_H__
#include "rexdos.h"
#endif
#endif
