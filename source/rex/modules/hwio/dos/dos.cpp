// ========================================================
//
// FILE:			/source/rex/modules/hwio/dos/dos.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		DOS namespace implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// DOS namespace
//
//

// DOS namespace definition (private)
namespace DOS
{

//
//
// Global variables
//
//

//
// Keyboard
//

#if (REX_COMPILER == COMPILER_DJGPP)

// DPMI segments
_go32_dpmi_seginfo KeyboardHandlerOld;
_go32_dpmi_seginfo KeyboardHandlerNew;

#endif

// Booleans
bool keyboard_enabled;

// Array of currently pressed keys
char keys[256];
char key_last_pressed;

//
// Timer
//

int64_t start_time;

//
//
// Functions
//
//

//
// Bootstrap
//

// Initialize all interrupts
void Initialize()
{
	#if (REX_COMPILER == COMPILER_DJGPP)

	// Enable access to the memory we use
	__djgpp_nearptr_enable();

	#endif

	// Initialize mouse & kb
	MouseInitialize();
	KeyboardInitialize();
	TimerInitialize();
}

// Free all interrupts
void Shutdown()
{
	// Shutdown mouse & kb
	MouseShutdown();
	KeyboardShutdown();

	#if (REX_COMPILER == COMPILER_DJGPP)

	// Reset memory access
	__djgpp_nearptr_disable();

	#endif
}

//
// Mouse
//

// Initialize mouse
void MouseInitialize()
{
	// Variables
	union REGS r;

	#if (REX_COMPILER == COMPILER_DJGPP)

		// Start mouse driver
		r.x.ax = 0x00;
		int86(0x33, &r, &r);

		// Don't show the mouse
		r.x.ax = 0x02;
		int86(0x33, &r, &r);

	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)

		// Start mouse driver
		r.x.eax = 0x00;
		int386(0x33, &r, &r);

		// Don't show the mouse
		r.x.eax = 0x02;
		int386(0x33, &r, &r);

	#endif
}

// Shutdown mouse
void MouseShutdown()
{

}

// Read mouse position (returns mouse buttons mask)
int16_t MouseRead(int16_t *x, int16_t *y)
{
	union REGS r;
	
	#if (REX_COMPILER == COMPILER_DJGPP)

		r.x.ax = 0x03;

		int86(0x33, &r, &r);

		if (x) *x = r.x.cx;
		if (y) *y = r.x.dx;

		return r.x.bx;

	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)

		r.x.eax = 0x03;

		int386(0x33, &r, &r);

		if (x) *x = r.x.ecx;
		if (y) *y = r.x.edx;

		return r.x.ebx;

	#endif
}

// Set mouse position
void MouseSet(int16_t x, int16_t y)
{
	union REGS r;

	#if (REX_COMPILER == COMPILER_DJGPP)

		r.x.ax = 0x04;
		r.x.cx = x;
		r.x.dx = y;
		int86(0x33, &r, &r);

	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)

		r.x.eax = 0x04;
		r.x.ecx = x;
		r.x.edx = y;
		int386(0x33, &r, &r);

	#endif
}

//
// Keyboard
//

// called by the system if a key is pressed, stores the key in the last_keys array and the keyb array
void KeyboardHandler()
{
	#if (REX_COMPILER == COMPILER_DJGPP)

		char key = inportb(0x60);

		keys[key_last_pressed = key & 0x7F] = !(key & 0x80);

		outportb(0x20, 0x20);

	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)

		char key = (char)inp(0x60);

		keys[key_last_pressed = key & 0x7F] = !(key & 0x80);

		outp(0x20, 0x20);

	#endif

}

// Initialize keyboard
void KeyboardInitialize()
{
	if (keyboard_enabled == false)
	{
		#if (REX_COMPILER == COMPILER_DJGPP)

			_go32_dpmi_get_protected_mode_interrupt_vector(9, &KeyboardHandlerOld);

			KeyboardHandlerNew.pm_offset = (int)KeyboardHandler;
			KeyboardHandlerNew.pm_selector = _go32_my_cs();

			_go32_dpmi_allocate_iret_wrapper(&KeyboardHandlerNew);

			_go32_dpmi_set_protected_mode_interrupt_vector(9, &KeyboardHandlerNew);

		#endif

		keyboard_enabled = true;
	}
}

// Shutdown keyboard
void KeyboardShutdown()
{
	if (keyboard_enabled == true)
	{
		#if (REX_COMPILER == COMPILER_DJGPP)

			// restore old handler
			_go32_dpmi_set_protected_mode_interrupt_vector(9, &KeyboardHandlerOld);

			// free interrupt capturing
			_go32_dpmi_free_iret_wrapper(&KeyboardHandlerNew);

		#endif

		keyboard_enabled = false;
	}
}

// Get key press
char KeyTest(int scancode)
{
	return keys[scancode];
}

//
// Timer
//

// Initialize uclock timer
void TimerInitialize()
{
	#if (REX_COMPILER == COMPILER_DJGPP)

		start_time = uclock();

	#endif

	#if (REX_COMPILER == COMPILER_wATCOM)

		start_time = clock();

	#endif
}

// Fetch the current time since initialization
int64_t TimerGet64()
{
	#if (REX_COMPILER == COMPILER_DJGPP)

		return (uclock() - start_time);

	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)

		return (clock() - start_time);

	#endif
}

} // namespace DOS
