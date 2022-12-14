// ========================================================
//
// FILE:			/source/rex/platform/sdl.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine: SDL Platform implementation
//
// LAST EDITED:		December 31st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex Engine: SDL Platform
//
//

namespace Rex
{

//
//
// Global Variables
//
//

// Mouse
rex_int32 mouse_x, mouse_y, mouse_b;

// Keyboard
bool keys[256];

//
//
// Functions
//
//

//
// Bootstrap
//

// Initialize all interrupts
bool Platform_Init()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return false;

	return true;
}

// Free all interrupts
bool Platform_Quit()
{
	// Quit SDL
	SDL_Quit();

	return true;
}

// Returns true if the main loop should continue
bool Platform_DoMainLoop(void *context)
{
	if (context == NULL || context == nullptr)
		return false;

	Platform_ReadInputDevices();

	return glfwWindowShouldClose((GLFWwindow *)context) > 0 ? false : true;
}

//
// Devices
//

// Read data from all currently connected input devices (keyboards, mice, etc)
bool Platform_ReadInputDevices()
{
	SDL_Event event;

	// Poll GLFW events
	glfwPollEvents();

	// Pump events
	SDL_PumpEvents();

	// Get mouse info
	mouse_b = SDL_GetMouseState(&mouse_x, &mouse_y);

	// Poll events
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				keys[event.key.keysym.scancode] = true;
				break;

			case SDL_KEYUP:
				keys[event.key.keysym.scancode] = false;
				break;

			default:
				break;
		}
	}

	return true;
}

// Get mouse data
bool Platform_MouseGet(rex_int *mx, rex_int *my, rex_int *mb)
{
	if (mx) *mx = (rex_int)mouse_x;
	if (my) *my = (rex_int)mouse_y;
	if (mb) *mb = (rex_int)mouse_b;

	return true;
}

// Set mouse data
bool Platform_MouseSet(rex_int mx, rex_int my, rex_int mb)
{
	return false;
}

// Returns true if the specified key is pressed
bool Platform_KeyboardGet(rex_int scancode)
{
	return keys[scancode];
}

//
// Timer
//

// Returns the number of ticks elapsed since program start
rex_int64 Platform_GetTicks64()
{
	return (rex_int64)SDL_GetTicks64();
}

//
// Graphics
//

// Display a pixel buffer on the screen
bool Platform_Display_PixelBuffer(void *context, rex_int width, rex_int height, rex_int bpp, void *pixels)
{
	if (bpp != 32) return false;

	GLuint texture;

	// clear screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2i(0, 0);
		glTexCoord2f(1, 0); glVertex2i(0 + width, 0);
		glTexCoord2f(1, 1); glVertex2i(0 + width, 0 + height);
		glTexCoord2f(0, 1); glVertex2i(0, 0 + height);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Swap buffers
	glfwSwapBuffers((GLFWwindow *)context);

	return true;
}

// Initialize a platform-specific graphics context
void *Platform_Init_Graphics(rex_int width, rex_int height, rex_int bpp, const char *title)
{
	// Init GLFW
	if (!glfwInit()) return nullptr;

	// Default window hints
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create window object
	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) return nullptr;

	// Set context to be current
	glfwMakeContextCurrent(window);
	glfwShowWindow(window);

	// Return pointer
	return (void *)window;
}

// Shutdown a platform-specific graphics context
void Platform_Quit_Graphics(void *context)
{
	// Destroy window
	if (context) glfwDestroyWindow((GLFWwindow *)context);

	// Terminate GLFW
	glfwTerminate();
}

//
// Message Handling
//

// Handle printing to the user
bool Platform_PrintHandler(const char *message, message_type type)
{
	switch (type)
	{
		case MESSAGE:
			cout << message << endl;
			return true;

		case WARNING:
			cout << "\033[33m" << message << "\033[0m" << endl;
			return true;

		case FAILURE:
			cout << "\033[31m" << message << "\033[0m" << endl;
			return true;

		default:
			return false;
	}
}

// Handle messages to the user
bool Platform_MessageHandler(const char *title, const char *message, message_type type, time_t time)
{
	rex_int type_for_sdl;

	switch (type)
	{
		case MESSAGE: type_for_sdl = SDL_MESSAGEBOX_INFORMATION; break;
		case WARNING: type_for_sdl = SDL_MESSAGEBOX_WARNING; break;
		case FAILURE: type_for_sdl=  SDL_MESSAGEBOX_ERROR; break;
		default: return false;
	}

	// Show SDL message box
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, NULL);

	// If it's a critical failure, quit the engine and exit the program
	if (type == FAILURE)
	{
		Quit();
		exit(EXIT_FAILURE);
	}

	return true;
}

} // namespace Rex

//
//
// MAIN PLATFORM ENTRY POINT
//
//

int main(int argc, char **argv)
{
	return RexMain(argc, argv);
}
