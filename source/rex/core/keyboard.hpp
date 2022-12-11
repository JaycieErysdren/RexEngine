// ========================================================
//
// FILE:			/source/rex/modules/core/io/keyboard.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Keyboard header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

#ifndef __REX_KEYBOARD_H__
#define __REX_KEYBOARD_H__

//
//
// Rex namespace: Keyboard
//
//

namespace Rex
{

//
//
// Text macros
//
//

// DOS target
#if (REX_TARGET == PLATFORM_DOS)

	// Keyboard scancodes
	#define REX_KB_ESC				0x01
	#define REX_KB_DNARROW			0x50
	#define REX_KB_LTARROW			0x4B
	#define REX_KB_RTARROW			0x4D
	#define REX_KB_UPARROW			0x48
	#define REX_KB_ENTER			28
	#define REX_KB_CTRL				29
	#define REX_KB_LTSHIFT			42
	#define REX_KB_RTSHIFT			54
	#define REX_KB_HOME				71
	#define REX_KB_PAGEUP			73
	#define REX_KB_END				79
	#define REX_KB_PAGEDN			81
	#define REX_KB_INSERT			82
	#define REX_KB_DELETE			83
	#define REX_KB_Q				16
	#define REX_KB_W				17
	#define REX_KB_E				18
	#define REX_KB_A				30
	#define REX_KB_S				31
	#define REX_KB_D				32
	#define REX_KB_F				33
	#define REX_KB_G				34
	#define REX_KB_SPACE			57
	#define REX_KB_C				46

#endif

//
//
// Function prototypes
//
//

// Returns true if the key is currently held down
bool KeyTest(int scancode);

} // namespace Rex

#endif // __REX_KEYBOARD_H__
