// ========================================================
//
// FILE:			/source2/arch/dos.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Rex Engine DOS interface.
//
// LAST EDITED:		November 18th, 2022
//
// ========================================================

// Rex Engine header
#include "rex.h"

//
// Mouse functions
//

void DOS_Mouse_Install(void)
{
	__dpmi_regs regs;
	regs.x.ax = 0;
	__dpmi_int(0x33, &regs);
}

void DOS_Mouse_Read(rex_int_16 *buttons, rex_int_16 *x, rex_int_16 *y)
{
	__dpmi_regs regs;
	regs.x.ax = 3;
	__dpmi_int(0x33, &regs);

	if (buttons) *buttons = regs.x.bx;
	if (x) *x = regs.x.cx;
	if (y) *y = regs.x.dx;
}

void DOS_Mouse_Set(rex_int_16 *x, rex_int_16 *y)
{
	__dpmi_regs regs;
	regs.x.ax = 4;
	regs.x.cx = *x;
	regs.x.dx = *y;
	__dpmi_int(0x33, &regs);
}

//
// Keyboard functions
//

