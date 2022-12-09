// ========================================================
//
// FILE:			/source/hwdrv/dos.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		DOS namespace header
//
// LAST EDITED:		December 9th, 2022
//
// ========================================================

//
//
// Helpful definitions
//
//

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
#define KB_Q				16
#define KB_W				17
#define KB_E				18
#define KB_A				30
#define KB_S				31
#define KB_D				32
#define KB_F				33
#define KB_G				34
#define KB_SPACE			57
#define KB_C				46

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
