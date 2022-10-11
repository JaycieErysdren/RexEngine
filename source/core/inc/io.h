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

extern char _rex_keys[512];

#define KEY_DOWN(A) _rex_keys[A]

void Rex_IO_ReadDevices(void);
