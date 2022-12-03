// ========================================================
//
// FILE:			/source/rex3d/thrdprty/tiled.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Tiled namespace header
//
// LAST EDITED:		December 2nd, 2022
//
// ========================================================

//
//
// Tiled namespace
//
//

// Tiled namespace definition (public)
namespace Tiled
{
	//
	//
	// Classes
	//
	//

	class TileMap
	{
		public:
			int width;
			int height;
			uint8_t *floor_height_data;
			uint8_t *floor_texture_data;
			uint8_t *wall_texture_data;
			uint8_t *ceiling_height_data;
			uint8_t *ceiling_texture_data;
	};

	//
	//
	// Functions
	//
	//

	TileMap *LoadTMJ(string filename);
	void FreeTileMap(TileMap *map);

}
