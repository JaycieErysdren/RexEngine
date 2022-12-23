
#ifndef __DECKER_DOS_H__
#define __DECKER_DOS_H__

//
// standard includes
//

#include <stdint.h>

// SDL types
typedef int8_t Sint8;
typedef uint8_t Uint8;

typedef int16_t Sint16;
typedef uint16_t Uint16;

typedef int32_t Sint32;
typedef uint32_t Uint32;

// SDL rect
typedef struct SDL_Rect {
	Sint16 x, y;
	Uint16 w, h;
} SDL_Rect;

// SDL structs
typedef struct SDL_Cursor SDL_Cursor;
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_Event SDL_Event;
typedef struct SDL_UserEvent SDL_UserEvent;
typedef struct SDL_AudioSpec SDL_AudioSpec;
typedef struct SDL_Surface SDL_Surface;

#endif