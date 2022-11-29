// ========================================================
//
// FILE:			/source/portrend.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		PortalRenderer namespace header
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

//
//
// PortalRenderer namespace
//
//

// PortalRenderer namespace definition (public)
namespace PortalRenderer
{
	//
	//
	// Classes
	//
	//

	// A view into the 2.5D world
	class Player
	{
		public:
			scalar_t origin[3];
			scalar_t velocity[3];
			scalar_t angle, angle_sin, angle_cos;
			scalar_t yaw;
			unsigned char sector_id;
	};

	// Each vertex has 2 components 
	class Vertex2f
	{
		public:
			scalar_t v[2];
	};

	// A segment of the world space
	class Sector
	{
		public:
			scalar_t floor;
			scalar_t ceiling;
			int num_vertices;
			Vertex2f *vertices;
			signed char *neighbors;
	};

	//
	//
	// Functions
	//
	//

	//
	// Rendering
	//

	// Render the world through portals
	void Render(int screen_width, int screen_height, bool enable_textures);

	// Load map data
	void LoadData();

	// Free map data
	void UnloadData();
}
