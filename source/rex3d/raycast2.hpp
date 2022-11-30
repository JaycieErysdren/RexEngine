// ========================================================
//
// FILE:			/source/raycast2.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Raycaster namespace header
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

//
//
// Raycaster namespace
//
//

// Raycaster namespace definition (public)
namespace Raycaster
{
	//
	//
	// Functions
	//
	//

	//
	// Bootstrap
	//

	void Initialize();

	//
	// Rendering
	//

	// Cast rays into the world and render the result
	void Render();
}
