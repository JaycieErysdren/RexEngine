// ========================================================
//
// FILE:			/source/core/inc/rex_io.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for program I/O functions.
//
// LAST EDITED:		October 29th, 2022
//
// ========================================================

//
// Device handling (devices.c)
//

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

// Mouse button scancodes
#define MOUSE_LEFT			SDL_BUTTON_LEFT
#define MOUSE_RIGHT			SDL_BUTTON_RIGHT
#define MOUSE_MIDDLE		SDL_BUTTON_MIDDLE
#define MOUSE_X1			SDL_BUTTON_X1
#define MOUSE_X2			SDL_BUTTON_X2

// Keyboard scancodes (Function keys)
#define KEY_F1				SDL_SCANCODE_F1
#define KEY_F2				SDL_SCANCODE_F2
#define KEY_F3				SDL_SCANCODE_F3
#define KEY_F4				SDL_SCANCODE_F4
#define KEY_F5				SDL_SCANCODE_F5
#define KEY_F6				SDL_SCANCODE_F6
#define KEY_F7				SDL_SCANCODE_F7
#define KEY_F8				SDL_SCANCODE_F8
#define KEY_F9				SDL_SCANCODE_F9
#define KEY_F10				SDL_SCANCODE_F10
#define KEY_F11				SDL_SCANCODE_F11
#define KEY_F12				SDL_SCANCODE_F12

// Keyboard scancodes (0-9)
#define KEY_0				SDL_SCANCODE_0
#define KEY_1				SDL_SCANCODE_1
#define KEY_2				SDL_SCANCODE_2
#define KEY_3				SDL_SCANCODE_3
#define KEY_4				SDL_SCANCODE_4
#define KEY_5				SDL_SCANCODE_5
#define KEY_6				SDL_SCANCODE_6
#define KEY_7				SDL_SCANCODE_7
#define KEY_8				SDL_SCANCODE_8
#define KEY_9				SDL_SCANCODE_9

// Keyboard scancodes (A-Z)
#define KEY_A				SDL_SCANCODE_A
#define KEY_B				SDL_SCANCODE_B
#define KEY_C				SDL_SCANCODE_C
#define KEY_D				SDL_SCANCODE_D
#define KEY_E				SDL_SCANCODE_E
#define KEY_F				SDL_SCANCODE_F
#define KEY_G				SDL_SCANCODE_G
#define KEY_H				SDL_SCANCODE_H
#define KEY_I				SDL_SCANCODE_I
#define KEY_J				SDL_SCANCODE_J
#define KEY_K				SDL_SCANCODE_K
#define KEY_L				SDL_SCANCODE_L
#define KEY_M				SDL_SCANCODE_M
#define KEY_N				SDL_SCANCODE_N
#define KEY_O				SDL_SCANCODE_O
#define KEY_P				SDL_SCANCODE_P
#define KEY_Q				SDL_SCANCODE_Q
#define KEY_R				SDL_SCANCODE_R
#define KEY_S				SDL_SCANCODE_S
#define KEY_T				SDL_SCANCODE_T
#define KEY_U				SDL_SCANCODE_U
#define KEY_V				SDL_SCANCODE_V
#define KEY_W				SDL_SCANCODE_W
#define KEY_X				SDL_SCANCODE_X
#define KEY_Y				SDL_SCANCODE_X
#define KEY_Z				SDL_SCANCODE_Z

// Keyboard scancodes (Other keys)
#define KEY_LCTRL			SDL_SCANCODE_LCTRL
#define KEY_RCTRL			SDL_SCANCODE_RCTRL
#define KEY_SPACE			SDL_SCANCODE_SPACE

// Keyboard scancodes (Arrow keys)
#define KEY_UP				SDL_SCANCODE_UP
#define KEY_DOWN			SDL_SCANCODE_DOWN
#define KEY_LEFT			SDL_SCANCODE_LEFT
#define KEY_RIGHT			SDL_SCANCODE_RIGHT

// Reads all currently active devices (keyboard, mouse, etc)
void Rex_IO_ReadDevices(void);

//
// File handling (files.c)
//

// Wrapper for fopen() that does automatic error checking.
FILE *Rex_IO_FOpen(rex_byte *filename, rex_byte *modes);

// Wrapper for fread() that does automatic error checking.
void Rex_IO_FRead(void *ptr, size_t size, rex_int n, FILE *file);

// Wrapper for fclose() that does automatic error checking.
void Rex_IO_FClose(FILE *file);

// Wrapper for fseek() that does automatic error checking.
void Rex_IO_FSeek(FILE *file, rex_long to, rex_int from);

// Wrapper for fwrite() that does automatic error checking.
void Rex_IO_FWrite(void *ptr, size_t size, rex_int n, FILE *file);

//
// Resource file I/O functions (resources.c)
//

extern rex_byte_c rex_resource_container_magic[8];
extern rex_byte_c rex_resource_magic[8];

// Write a resource container and its resources to a file.
void Rex_IO_WriteResourceContainer(rex_byte *filename, rex_resource_container *res);

//
// tinyfiledialogs helpers (tinyfd.c)
//

rex_byte *Rex_IO_OpenFileDialog(rex_byte *title, rex_byte *default_path, rex_int num_filter_patterns, rex_byte **filter_patterns, rex_byte *filter_descriptions, rex_int allow_multiple_selects);
