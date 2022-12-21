// ========================================================
//
// FILE:			/source/core/inc/engine/device_io.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the device I/O functions.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Keyboard scancode array
extern rex_byte rex_keys[512];

// Mouse button scancode array
extern rex_byte rex_mouse_buttons[32];

// Mouse x and y coordinates (relative to screen resolution)
extern rex_coord2i rex_mouse;

// Mouse x and y delta since last read
extern rex_vector2i rex_mouse_delta;

// Mouse scroll wheel x and y since last read
extern rex_vector2i rex_mouse_scroll;

// Mouse scroll wheel x and y delta since last read
extern rex_vector2i rex_mouse_scroll_delta;

// The current desktop resolution
extern rex_vector2i rex_desktop_size;

// Returns true if the specified key is currently pressed
#define KEY_PRESSED(A) rex_keys[A]

// Returns true if the specified mouse button is currently pressed.
#define MOUSE_PRESSED(A) rex_mouse_buttons[A]

// Boolean that is equal to REX_TRUE if the program is supposed to be running.
extern rex_bool rex_running;

// Mouse button scancodes
#define REX_MOUSE_LEFT			SDL_BUTTON_LEFT
#define REX_MOUSE_RIGHT			SDL_BUTTON_RIGHT
#define REX_MOUSE_MIDDLE		SDL_BUTTON_MIDDLE
#define REX_MOUSE_X1			SDL_BUTTON_X1
#define REX_MOUSE_X2			SDL_BUTTON_X2

// Keyboard scancodes (Function keys)
#define REX_KEY_F1				SDL_SCANCODE_F1
#define REX_KEY_F2				SDL_SCANCODE_F2
#define REX_KEY_F3				SDL_SCANCODE_F3
#define REX_KEY_F4				SDL_SCANCODE_F4
#define REX_KEY_F5				SDL_SCANCODE_F5
#define REX_KEY_F6				SDL_SCANCODE_F6
#define REX_KEY_F7				SDL_SCANCODE_F7
#define REX_KEY_F8				SDL_SCANCODE_F8
#define REX_KEY_F9				SDL_SCANCODE_F9
#define REX_KEY_F10				SDL_SCANCODE_F10
#define REX_KEY_F11				SDL_SCANCODE_F11
#define REX_KEY_F12				SDL_SCANCODE_F12

// Keyboard scancodes (0-9)
#define REX_KEY_0				SDL_SCANCODE_0
#define REX_KEY_1				SDL_SCANCODE_1
#define REX_KEY_2				SDL_SCANCODE_2
#define REX_KEY_3				SDL_SCANCODE_3
#define REX_KEY_4				SDL_SCANCODE_4
#define REX_KEY_5				SDL_SCANCODE_5
#define REX_KEY_6				SDL_SCANCODE_6
#define REX_KEY_7				SDL_SCANCODE_7
#define REX_KEY_8				SDL_SCANCODE_8
#define REX_KEY_9				SDL_SCANCODE_9

// Keyboard scancodes (A-Z)
#define REX_KEY_A				SDL_SCANCODE_A
#define REX_KEY_B				SDL_SCANCODE_B
#define REX_KEY_C				SDL_SCANCODE_C
#define REX_KEY_D				SDL_SCANCODE_D
#define REX_KEY_E				SDL_SCANCODE_E
#define REX_KEY_F				SDL_SCANCODE_F
#define REX_KEY_G				SDL_SCANCODE_G
#define REX_KEY_H				SDL_SCANCODE_H
#define REX_KEY_I				SDL_SCANCODE_I
#define REX_KEY_J				SDL_SCANCODE_J
#define REX_KEY_K				SDL_SCANCODE_K
#define REX_KEY_L				SDL_SCANCODE_L
#define REX_KEY_M				SDL_SCANCODE_M
#define REX_KEY_N				SDL_SCANCODE_N
#define REX_KEY_O				SDL_SCANCODE_O
#define REX_KEY_P				SDL_SCANCODE_P
#define REX_KEY_Q				SDL_SCANCODE_Q
#define REX_KEY_R				SDL_SCANCODE_R
#define REX_KEY_S				SDL_SCANCODE_S
#define REX_KEY_T				SDL_SCANCODE_T
#define REX_KEY_U				SDL_SCANCODE_U
#define REX_KEY_V				SDL_SCANCODE_V
#define REX_KEY_W				SDL_SCANCODE_W
#define REX_KEY_X				SDL_SCANCODE_X
#define REX_KEY_Y				SDL_SCANCODE_X
#define REX_KEY_Z				SDL_SCANCODE_Z

// Keyboard scancodes (Other keys)
#define REX_KEY_LCTRL			SDL_SCANCODE_LCTRL
#define REX_KEY_RCTRL			SDL_SCANCODE_RCTRL
#define REX_KEY_SPACE			SDL_SCANCODE_SPACE

// Keyboard scancodes (Arrow keys)
#define REX_KEY_UP				SDL_SCANCODE_UP
#define REX_KEY_DOWN			SDL_SCANCODE_DOWN
#define REX_KEY_LEFT			SDL_SCANCODE_LEFT
#define REX_KEY_RIGHT			SDL_SCANCODE_RIGHT

// Reads all currently active devices (keyboard, mouse, etc)
void Rex_IO_ReadDevices(void);
