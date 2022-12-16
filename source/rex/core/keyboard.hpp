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
	#define REX_KB_ESCAPE			0x01
	#define REX_KB_1				0x02
	#define REX_KB_2				0x03
	#define REX_KB_3				0x04
	#define REX_KB_4				0x05
	#define REX_KB_5				0x06
	#define REX_KB_6				0x07
	#define REX_KB_7				0x08
	#define REX_KB_8				0x09
	#define REX_KB_9				0x0A
	#define REX_KB_0				0x0B
	#define REX_KB_MINUS			0x0C
	#define REX_KB_EQUALS			0x0D
	#define REX_KB_BACKSPACE		0x0E
	#define REX_KB_TAB				0x0F
	#define REX_KB_Q				0x10
	#define REX_KB_W				0x11
	#define REX_KB_E				0x12
	#define REX_KB_R				0x13
	#define REX_KB_T				0x14
	#define REX_KB_Y				0x15
	#define REX_KB_U				0x16
	#define REX_KB_I				0x17
	#define REX_KB_O				0x18
	#define REX_KB_P				0x19
	#define REX_KB_LEFTBRACKET		0x1A
	#define REX_KB_RIGHTBRACKET		0x1B
	#define REX_KB_ENTER			0x1C
	#define REX_KB_CONTROL			0x1D
	#define REX_KB_A				0x1E
	#define REX_KB_S				0x1F
	#define REX_KB_D				0x20
	#define REX_KB_F				0x21
	#define REX_KB_G				0x22
	#define REX_KB_H				0x23
	#define REX_KB_J				0x24
	#define REX_KB_K				0x25
	#define REX_KB_L				0x26
	#define REX_KB_SEMICOLON		0x27
	#define REX_KB_QUOTE			0x28
	#define REX_KB_TILDE			0x29
	#define REX_KB_LSHIFT			0x2A
	#define REX_KB_BACKSLASH		0x2B
	#define REX_KB_Z				0x2C
	#define REX_KB_X				0x2D
	#define REX_KB_C				0x2E
	#define REX_KB_V				0x2F
	#define REX_KB_B				0x30
	#define REX_KB_N				0x31
	#define REX_KB_M				0x32
	#define REX_KB_COMMA			0x33
	#define REX_KB_PERIOD			0x34
	#define REX_KB_SLASH			0x35
	#define REX_KB_RSHIFT			0x36
	#define REX_KB_MULTIPLY			0x37
	#define REX_KB_ALT				0x38
	#define REX_KB_SPACE			0x39
	#define REX_KB_CAPSLOCK			0x3A
	#define REX_KB_F1				0x3B
	#define REX_KB_F2				0x3C
	#define REX_KB_F3				0x3D
	#define REX_KB_F4				0x3E
	#define REX_KB_F5				0x3F
	#define REX_KB_F6				0x40
	#define REX_KB_F7				0x41
	#define REX_KB_F8				0x42
	#define REX_KB_F9				0x43
	#define REX_KB_F10				0x44
	#define REX_KB_NUMLOCK			0x45
	#define REX_KB_SCROLLLOCK		0x46
	#define REX_KB_HOME				0x47
	#define REX_KB_UP				0x48
	#define REX_KB_PAGEUP			0x49
	#define REX_KB_LEFT				0x4B
	#define REX_KB_RIGHT			0x4D
	#define REX_KB_PLUS				0x4E
	#define REX_KB_END				0x4F
	#define REX_KB_DOWN				0x50
	#define REX_KB_PAGEDOWN			0x51
	#define REX_KB_INSERT			0x52
	#define REX_KB_DELETE			0x53
	#define REX_KB_F11				0x57
	#define REX_KB_F12				0x58

#endif

// Linux or Windows target
#if (REX_TARGET == PLATFORM_NIX) || (REX_TARGET == PLATFORM_WIN)

	// Keyboard scancodes
	#define REX_KB_ESCAPE			SDL_SCANCODE_ESCAPE
	#define REX_KB_1				SDL_SCANCODE_1
	#define REX_KB_2				SDL_SCANCODE_2
	#define REX_KB_3				SDL_SCANCODE_3
	#define REX_KB_4				SDL_SCANCODE_4
	#define REX_KB_5				SDL_SCANCODE_5
	#define REX_KB_6				SDL_SCANCODE_6
	#define REX_KB_7				SDL_SCANCODE_7
	#define REX_KB_8				SDL_SCANCODE_8
	#define REX_KB_9				SDL_SCANCODE_9
	#define REX_KB_0				SDL_SCANCODE_0
	#define REX_KB_MINUS			SDL_SCANCODE_MINUS
	#define REX_KB_EQUALS			SDL_SCANCODE_EQUALS
	#define REX_KB_BACKSPACE		SDL_SCANCODE_BACKSPACE
	#define REX_KB_TAB				SDL_SCANCODE_TAB
	#define REX_KB_Q				SDL_SCANCODE_Q
	#define REX_KB_W				SDL_SCANCODE_W
	#define REX_KB_E				SDL_SCANCODE_E
	#define REX_KB_R				SDL_SCANCODE_R
	#define REX_KB_T				SDL_SCANCODE_T
	#define REX_KB_Y				SDL_SCANCODE_Y
	#define REX_KB_U				SDL_SCANCODE_U
	#define REX_KB_I				SDL_SCANCODE_I
	#define REX_KB_O				SDL_SCANCODE_O
	#define REX_KB_P				SDL_SCANCODE_P
	#define REX_KB_LEFTBRACKET		SDL_SCANCODE_LEFTBRACKET
	#define REX_KB_RIGHTBRACKET		SDL_SCANCODE_RIGHTBRACKET
	#define REX_KB_ENTER			SDL_SCANCODE_RETURN
	#define REX_KB_CONTROL			SDL_SCANCODE_LCTRL
	#define REX_KB_A				SDL_SCANCODE_A
	#define REX_KB_S				SDL_SCANCODE_S
	#define REX_KB_D				SDL_SCANCODE_D
	#define REX_KB_F				SDL_SCANCODE_F
	#define REX_KB_G				SDL_SCANCODE_G
	#define REX_KB_H				SDL_SCANCODE_H
	#define REX_KB_J				SDL_SCANCODE_J
	#define REX_KB_K				SDL_SCANCODE_K
	#define REX_KB_L				SDL_SCANCODE_L
	#define REX_KB_SEMICOLON		SDL_SCANCODE_SEMICOLON
	#define REX_KB_QUOTE			0 // FIXME: no SDL scancode available??!
	#define REX_KB_TILDE			SDL_SCANCODE_GRAVE
	#define REX_KB_LSHIFT			SDL_SCANCODE_LSHIFT
	#define REX_KB_BACKSLASH		SDL_SCANCODE_BACKSLASH
	#define REX_KB_Z				SDL_SCANCODE_Z
	#define REX_KB_X				SDL_SCANCODE_X
	#define REX_KB_C				SDL_SCANCODE_C
	#define REX_KB_V				SDL_SCANCODE_V
	#define REX_KB_B				SDL_SCANCODE_B
	#define REX_KB_N				SDL_SCANCODE_N
	#define REX_KB_M				SDL_SCANCODE_M
	#define REX_KB_COMMA			SDL_SCANCODE_COMMA
	#define REX_KB_PERIOD			SDL_SCANCODE_PERIOD
	#define REX_KB_SLASH			SDL_SCANCODE_SLASH
	#define REX_KB_RSHIFT			SDL_SCANCODE_RSHIFT
	#define REX_KB_MULTIPLY			0 // FIXME: no SDL scancode available??!
	#define REX_KB_ALT				SDL_SCANCODE_LALT
	#define REX_KB_SPACE			SDL_SCANCODE_SPACE
	#define REX_KB_CAPSLOCK			SDL_SCANCODE_CAPSLOCK
	#define REX_KB_F1				SDL_SCANCODE_F1
	#define REX_KB_F2				SDL_SCANCODE_F2
	#define REX_KB_F3				SDL_SCANCODE_F3
	#define REX_KB_F4				SDL_SCANCODE_F4
	#define REX_KB_F5				SDL_SCANCODE_F5
	#define REX_KB_F6				SDL_SCANCODE_F6
	#define REX_KB_F7				SDL_SCANCODE_F7
	#define REX_KB_F8				SDL_SCANCODE_F8
	#define REX_KB_F9				SDL_SCANCODE_F9
	#define REX_KB_F10				SDL_SCANCODE_F10
	#define REX_KB_NUMLOCK			SDL_SCANCODE_NUMLOCKCLEAR
	#define REX_KB_SCROLLLOCK		SDL_SCANCODE_SCROLLLOCK
	#define REX_KB_HOME				SDL_SCANCODE_HOME
	#define REX_KB_UP				SDL_SCANCODE_UP
	#define REX_KB_PAGEUP			SDL_SCANCODE_PAGEUP
	#define REX_KB_LEFT				SDL_SCANCODE_LEFT
	#define REX_KB_RIGHT			SDL_SCANCODE_RIGHT
	#define REX_KB_PLUS				SDL_SCANCODE_EQUALS
	#define REX_KB_END				SDL_SCANCODE_END
	#define REX_KB_DOWN				SDL_SCANCODE_DOWN
	#define REX_KB_PAGEDOWN			SDL_SCANCODE_PAGEDOWN
	#define REX_KB_INSERT			SDL_SCANCODE_INSERT
	#define REX_KB_DELETE			SDL_SCANCODE_DELETE
	#define REX_KB_F11				SDL_SCANCODE_F11
	#define REX_KB_F12				SDL_SCANCODE_F12

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
