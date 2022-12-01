// ========================================================
//
// FILE:			/source/world/object.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Object namespace header
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

//
//
// Object namespace
//
//

// Object namespace definition (public)
namespace Object
{
	//
	//
	// Types
	//
	//

	// Object type
	typedef struct
	{
		int32_t x, xx, xxx;
		int32_t y, yy, yyy;
		int32_t z, zz, zzz;
		vec3_t rot;
		int32_t sid;
		int32_t flags;
		World::surface_t front;
		World::surface_t back;
	} object_t;

	//
	//
	// Functions
	//
	//

	void LocalSpace(object_t *object, matrix_t matrix);
	void Matrix(object_t *object, matrix_t matrix, int width, int height);
	void Update(object_t *object);
	void WallCollision(object_t *object, int wid);
	void SectorCollision(object_t *object, int sid);
	void Collision(object_t *object);
}