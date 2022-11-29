// ========================================================
//
// FILE:			/source/dos.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		DOS namespace implementation
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

// Rex3D header
#include "rex3d.hpp"

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
	// Variables
	//
	//

	//
	// Keyboard
	//

	// DPMI segments
	_go32_dpmi_seginfo KeyboardHandlerOld;
	_go32_dpmi_seginfo KeyboardHandlerNew;

	// Booleans
	bool keyboard_enabled;

	// Array of currently pressed keys
	char keys[256];
	char key_last_pressed;

	//
	//
	// Functions
	//
	//

	//
	// Keyboard
	//

	void KeyboardHandler();
}

//
// Bootstrap
//

// Set all interrupts
void DOS::Initialize()
{
	MouseInitialize();
	KeyboardInitialize();
}

// Free all interrupts
void DOS::Shutdown()
{
	MouseShutdown();
	KeyboardShutdown();
}

//
// Mouse
//

// Initialize mouse
void DOS::MouseInitialize()
{
	// Variables
	union REGS r;

	// Start mouse driver
	r.x.ax = 0;
	int386(0x33, &r, &r);

	// Don't show the mouse
	r.x.ax = 2;
	int386(0x33, &r, &r);
}

// Shutdown mouse
void DOS::MouseShutdown()
{

}

// Read mouse position (returns mouse buttons mask)
int DOS::MouseRead(int *x, int *y)
{
	union REGS r;
	r.x.ax = 3;
	int386(0x33, &r, &r);
	if (x) *x = r.x.cx;
	if (y) *y = r.x.dx;
	return r.x.bx;
}

//
// Keyboard
//

// called by the system if a key is pressed, stores the key in the last_keys array and the keyb array
void DOS::KeyboardHandler()
{
	char key = inportb(0x60);

	keys[key_last_pressed = key & 0x7F] = !(key & 0x80);

	outportb(0x20, 0x20);
}

// Initialize keyboard
void DOS::KeyboardInitialize()
{
	if (keyboard_enabled == false)
	{
		_go32_dpmi_get_protected_mode_interrupt_vector(9, &KeyboardHandlerOld);

		KeyboardHandlerNew.pm_offset = (int)KeyboardHandler;
		KeyboardHandlerNew.pm_selector = _go32_my_cs();

		_go32_dpmi_allocate_iret_wrapper(&KeyboardHandlerNew);

		_go32_dpmi_set_protected_mode_interrupt_vector(9, &KeyboardHandlerNew);

		keyboard_enabled = true;
	}
}

// Shutdown keyboard
void DOS::KeyboardShutdown()
{
	if (keyboard_enabled == true)
	{
		// restore old handler
		_go32_dpmi_set_protected_mode_interrupt_vector(9, &KeyboardHandlerOld);

		// free interrupt capturing
		_go32_dpmi_free_iret_wrapper(&KeyboardHandlerNew);

		keyboard_enabled = false;
	}
}

// Get key press
char DOS::KeyTest(int scancode)
{
	return keys[scancode];
}
