// ========================================================
//
// FILE:			/source/raycastr.hpp
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

// Placeholder world map
extern uint8_t world_map[24][24];

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
	// Classes
	//
	//

	// A view into the 2.5D world
	class Camera
	{
		public:
			scalar_t origin[2];
			scalar_t angle[2];
			scalar_t plane[2];
	};

	// An 8-bit indexed texture
	class Texture
	{
		public:
			int width;
			int height;
			uint8_t *pixels;
	};

	//
	//
	// Functions
	//
	//

	// Load some textures into memory
	void LoadTextures();

	// Cast rays into the world and render the result
	void Render(Camera &camera, int width, int height, bool texture_mapping);
}
