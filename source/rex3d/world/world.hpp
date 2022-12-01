// ========================================================
//
// FILE:			/source/world/world.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		World namespace header
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

//
//
// World namespace
//
//

// World namespace definition (public)
namespace World
{
	//
	//
	// Types
	//
	//

	// Vertex type
	typedef struct
	{
		int32_t x;
		int32_t y;
		int32_t z;
		int32_t u;
		int32_t v;
		int32_t w;
	} vertex_t;

	// Surface type
	typedef struct
	{
		uint8_t texture;
		uint8_t overlay;
		int16_t flags;
		uint8_t light, red, green, blue;			// RGB is _not_ currently used.
		int32_t slopex;
		int32_t slopey;
		int32_t slopez;
		uint8_t repeatx, _pad4, _pad5, _pad6;
		uint8_t repeaty, _pad7, _pad8, _pad9;
		int32_t panningx; 							// fixed point texture u offset
		int32_t panningy; 							// fixed point texture v offset
		int32_t reserved;
	} surface_t;

	// Sector type
	typedef struct
	{
		// Persistent Variables. Moving these variables will break the file format!
		int16_t lid;
		int16_t padding;
		int16_t first_wall;
		int16_t flags;
		surface_t top;
		surface_t bot;
		surface_t mid;

		int reserved;

		// Calculated variables. These variables are not saved out to a file.
		int32_t vis_id;
		rect_t bounds;
		point_t center;
		int32_t locked;
	} sector_t;

	// Wall type
	typedef struct
	{
		// Persistent Variables. Moving these variables will break the file format!
		int16_t sid , _sid;
		int16_t next, _next;
		int16_t port, _port;
		int32_t x, y;
		surface_t surface;

		int reserved;

		// Calculated variables. These variables are not saved out to a file.
		vertex_t top;
		vertex_t bot;
		vertex_t mid;
		vertex_t poly[4];
		int32_t dx, dy;
		int32_t visible;
		int32_t frame;
		int32_t length;
		int32_t distance;
	} wall_t;

	// Polygon type
	typedef vertex_t polygon_t[MAX_POLYGON];
}