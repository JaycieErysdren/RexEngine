// ========================================================
//
// FILE:			/source2/arch/vga.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Rex Engine VGA interface.
//
// LAST EDITED:		November 18th, 2022
//
// ========================================================

// Rex Engine header
#include "rex.h"

void VGA_SetMode(rex_int_16 mode)
{
	__dpmi_regs r;
	memset(&r, 0, sizeof(r));
	r.x.ax = mode;
	__dpmi_int(0x10, &r);
}
