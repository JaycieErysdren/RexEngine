// ========================================================
//
// FILE:			/source/dos.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		DOS namespace header
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

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

	//
	// Keyboard
	//

	// Initialize keyboard
	void KeyboardInitialize();

	// Shutdown keyboard
	void KeyboardShutdown();

	// Get key press
	char KeyTest(int scancode);
}
