// ========================================================
//
// FILE:			/source/rex/core/model/poly.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Poly Model header
//
// LAST EDITED:		December 26th, 2022
//
// ========================================================

#ifndef __REX_MODEL_POLY_H__
#define __REX_MODEL_POLY_H__

//
//
// Rex namespace: Poly Model
//
//

namespace Rex
{

//
// Polygon
//

class Polygon
{
	public:
		vector<rex_int32> vertices;
};

//
// Array of vertices and polygons
//

class PolyData
{
	public:
		vector<Vertex> vertices;
		vector<Polygon> polygons;
};

} // namespace Rex

#endif // __REX_MODEL_POLY_H__
