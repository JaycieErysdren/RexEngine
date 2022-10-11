// ========================================================
//
// FILE:			/source/core/inc/io.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for program I/O functions.
//
// LAST EDITED:		October 10th, 2022
//
// ========================================================

//
// Device handling (devices.c)
//

// Keyboard scancode array
extern rex_byte _rex_keys[512];

// Mouse x and y coordinates (relative to screen resolution)
extern rex_coord _rex_mouse;

// Returns true if the specified key is currently pressed
#define KEY_DOWN(A) _rex_keys[A]

// Reads all currently active devices (keyboard, mouse, etc)
void Rex_IO_ReadDevices(void);
