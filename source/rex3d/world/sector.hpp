// ========================================================
//
// FILE:			/source/world/sector.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Sector namespace header
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

//
//
// Sector namespace
//
//

// Sector namespace definition (public)
namespace Sector
{
	//
	//
	// Types
	//
	//

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

	//
	//
	// Functions
	//
	//

	int PointInside(int sid, int x, int y);
	int FromPoint(int x, int y);
	void ComputeZ(int sid, int x, int y, int* bot, int* top, int* mid);
	void Transform(int sid, matrix_t matrix);
	int ExtrudeCeiling(int sid, polygon_t poly);
	int ExtrudeFloor(int sid, polygon_t poly);
	int ExtrudeMiddle(int sid, polygon_t poly);
	point_t Center(int sid);
	rect_t Bounds(int sid);
	int Linked(int s1, int s2);
	void CalcVis(object_t *camera);
}