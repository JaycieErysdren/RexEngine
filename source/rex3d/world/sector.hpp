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
// LAST EDITED:		December 1st, 2022
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

	//
	//
	// Functions
	//
	//

	int PointInside(int sid, int x, int y);
	int FromPoint(int x, int y);
	void ComputeZ(int sid, int x, int y, int *bot, int *top, int *mid);
	void Transform(int sid, matrix_t matrix);
	int ExtrudeCeiling(int sid, World::polygon_t poly);
	int ExtrudeFloor(int sid, World::polygon_t poly);
	int ExtrudeMiddle(int sid, World::polygon_t poly);
	point_t Center(int sid);
	rect_t Bounds(int sid);
	int Linked(int s1, int s2);
	void CalcVis(Object::object_t *camera);
}