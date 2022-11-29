// ========================================================
//
// FILE:			/source/rex3d/rex3d.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Rex3D main engine header
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

#ifndef __REX3D_H__
#define __REX3D_H__

// Standard C++ headers
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>

// Standard C headers
#include <unistd.h>
#include <stdint.h>

// DJGPP headers
#include <dpmi.h>
#include <dos.h>
#include <go32.h>
#include <sys/nearptr.h>
#include <sys/farptr.h>

// Use standard namespace
using namespace std;

// Rex3D headers
#include "dos.hpp"
#include "raycastr.hpp"
#include "vga.hpp"

// Keyboard scancodes
#define KB_ESC				0x01
#define KB_DNARROW			0x50
#define KB_LTARROW			0x4B
#define KB_RTARROW			0x4D
#define KB_UPARROW			0x48
#define KB_ENTER			28
#define KB_CTRL				29
#define KB_LTSHIFT			42
#define KB_RTSHIFT			54
#define KB_HOME				71
#define KB_PAGEUP			73
#define KB_END				79
#define KB_PAGEDN			81
#define KB_INSERT			82
#define KB_DELETE			83
#define KB_W				17
#define KB_A				30
#define KB_S				31
#define KB_D				32
#define KB_SPACE			57
#define KB_C				46

#endif // __REX3D_H__
