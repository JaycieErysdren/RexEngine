// ========================================================
//
// FILE:			/source/rex/modules/hwio/dos/dos.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		DOS namespace header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

//
//
// Helpful definitions
//
//

// DOS clock speed
#define DOS_CLOCK_SPEED 1192755L

//
//
// DOS namespace
//
//

// DOS namespace definition (public)
namespace DOS
{
	//
	//
	// Functions
	//
	//

	//
	// Bootstrap
	//

	// Set all interrupts
	void Initialize();

	// Free all interrupts
	void Shutdown();

	//
	// Mouse
	//

	// Initialize mouse
	void MouseInitialize();

	// Shutdown mouse
	void MouseShutdown();

	// Read mouse position (returns mouse buttons mask)
	int16_t MouseRead(int16_t *x, int16_t *y);

	// Set mouse position
	void MouseSet(int16_t x, int16_t y);

	//
	// Keyboard
	//

	// Initialize keyboard
	void KeyboardInitialize();

	// Shutdown keyboard
	void KeyboardShutdown();

	// Get key press
	char KeyTest(int scancode);

	//
	// Timer
	//

	// Initialize uclock timer
	void TimerInitialize();

	// Fetch the current time since initialization
	int64_t TimerGet64();
}
