// ========================================================
//
// FILE:			/source/rex/platform/platform.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine: Platform header
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

#ifndef __REX_PLATFORM_H__
#define __REX_PLATFORM_H__

//
//
// Rex Engine: Platform
//
//

namespace Rex
{

//
// Bootstrap
//

// Initialize platform-specific handlers
bool Platform_Init();

// Shutdown platform-specific handlers
bool Platform_Quit();

// Returns true if the main loop should continue
bool Platform_DoMainLoop(void *context);

//
// Devices
//

// Read data from all currently connected input devices (keyboards, mice, etc)
bool Platform_ReadInputDevices();

// Get mouse data
bool Platform_MouseGet(rex_int *mx, rex_int *my, rex_int *mb);

// Set mouse data
bool Platform_MouseSet(rex_int mx, rex_int my, rex_int mb);

// Returns true if the specified key is pressed
bool Platform_KeyboardGet(rex_int scancode);

//
// Timer
//

// Returns the number of ticks elapsed since program start
rex_int64 Platform_GetTicks64();

//
// Graphics
//

// Initialize a platform-specific graphics context
void *Platform_Init_Graphics(rex_int width, rex_int height, rex_int bpp, const char *title);

// Shutdown a platform-specific graphics context
void Platform_Quit_Graphics(void *context);

// Display a pixel buffer on the screen
bool Platform_Display_PixelBuffer(void *context, rex_int width, rex_int height, rex_int bpp, void *pixels);

//
// Message Handling
//

// Handle printing to the user
bool Platform_PrintHandler(const char *message, message_type type);

// Handle messages to the user
bool Platform_MessageHandler(const char *title, const char *message, message_type type, time_t time);

} // namespace Rex

#endif // __REX_PLATFORM_H__
