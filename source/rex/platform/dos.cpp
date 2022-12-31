// ========================================================
//
// FILE:			/source/rex/platform/dos.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine: DOS Platform implementation
//
// LAST EDITED:		December 31st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex Engine: DOS Platform
//
//

namespace Rex
{

//
//
// Global variables
//
//

//
// DOS
//

// Regs
union REGS regs;

//
// Mouse
//

rex_int16 mouse_x, mouse_y, mouse_b;

//
// Keyboard
//

#if (REX_COMPILER == COMPILER_DJGPP)
	_go32_dpmi_seginfo Keyboard_Handler_Old;
	_go32_dpmi_seginfo Keyboard_Handler_New;
#endif

char keys[256];
char key_last_pressed;

//
//
// Functions
//
//

//
// DOS
//

// Called by DOS whenever a key is pressed
void Keyboard_Handler()
{
	char key = (char)inp(0x60);
	keys[key_last_pressed = key & 0x7F] = !(key & 0x80);
	outp(0x20, 0x20);
}

//
// Bootstrap
//

// Initialize all interrupts
bool Platform_Init()
{
	//
	// Memory access
	//

	#if (REX_COMPILER == COMPILER_DJGPP)
		__djgpp_nearptr_enable();
	#endif

	//
	// Mouse
	//

	// Initialize mouse driver
	regs.w.ax = 0x00;
	int386(0x33, &regs, &regs);

	// Hide DOS mouse cursor
	regs.w.ax = 0x02;
	int386(0x33, &regs, &regs);

	//
	// Keyboard
	//

	memset(keys, 0, sizeof(keys));

	#if (REX_COMPILER == COMPILER_DJGPP)
		_go32_dpmi_get_protected_mode_interrupt_vector(9, &Keyboard_Handler_Old);
		Keyboard_Handler_New.pm_offset = (int)Keyboard_Handler;
		Keyboard_Handler_New.pm_selector = _go32_my_cs();
		_go32_dpmi_allocate_iret_wrapper(&Keyboard_Handler_New);
		_go32_dpmi_set_protected_mode_interrupt_vector(9, &Keyboard_Handler_New);
	#endif

	//
	// Timer
	//

	#if (REX_COMPILER == COMPILER_DJGPP)
		uclock();
	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)
		clock();
	#endif

	return true;
}

// Free all interrupts
bool Platform_Quit()
{
	//
	// Keyboard
	//

	#if (REX_COMPILER == COMPILER_DJGPP)
		_go32_dpmi_set_protected_mode_interrupt_vector(9, &Keyboard_Handler_Old);
		_go32_dpmi_free_iret_wrapper(&Keyboard_Handler_New);
	#endif

	//
	// Memory access
	//

	#if (REX_COMPILER == COMPILER_DJGPP)
		__djgpp_nearptr_disable();
	#endif

	return true;
}

//
// Devices
//

// Read data from all currently connected input devices (keyboards, mice, etc)
bool Platform_ReadInputDevices()
{
	//
	// Mouse
	//

	regs.w.ax = 0x03;
	int386(0x33, &regs, &regs);
	mouse_b = regs.w.bx;
	mouse_x = regs.w.cx;
	mouse_y = regs.w.dx;

	return true;
}

// Get mouse data
bool Platform_MouseGet(rex_int *mx, rex_int *my, rex_int *mb)
{
	if (mx) *mx = (rex_int)mouse_x;
	if (my) *my = (rex_int)mouse_y;
	if (mb) *mb = (rex_int)mouse_b;

	return true;
}

// Set mouse data
bool Platform_MouseSet(rex_int mx, rex_int my, rex_int mb)
{
	regs.w.ax = 0x04;
	regs.w.bx = (rex_int16)mb;
	regs.w.cx = (rex_int16)mx;
	regs.w.dx = (rex_int16)my;
	int386(0x33, &regs, &regs);

	return true;
}

// Returns true if the specified key is pressed
bool Platform_KeyboardGet(rex_int scancode)
{
	return keys[scancode] ? true : false;
}

//
// Timer
//

// Returns the number of ticks elapsed since program start
rex_int64 Platform_GetTicks64()
{
	#if (REX_COMPILER == COMPILER_DJGPP)
		return (rex_int64)uclock();
	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)
		return (rex_int64)clock();
	#endif
}

//
// Graphics
//

// Show a simple message box
bool Platform_MessageBox(const char *title, const char *message)
{
	cout << title << endl;
	cout << message << endl;

	return true;
}

} // namespace Rex

//
//
// MAIN PLATFORM ENTRY POINT
//
//

int main(int argc, char *argv[])
{
	return RexMain(argc, argv);
}
