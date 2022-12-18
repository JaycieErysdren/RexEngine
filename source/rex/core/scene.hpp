// ========================================================
//
// FILE:			/source/rex/core/scene.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Scene header
//
// LAST EDITED:		December 18th, 2022
//
// ========================================================

#ifndef __REX_SCENE_H__
#define __REX_SCENE_H__

//
//
// Rex namespace: Scene
//
//

namespace Rex
{

//
//
// Enums
//
//

typedef enum
{
	ACTOR_NONE,
	ACTOR_CAMERA,
	ACTOR_VOXELMODEL,
	ACTOR_POLYMODEL,
} rex_actor_type;

//
//
// Classes
//
//

class Actor
{
	public:

		//
		// General variables
		//

		rex_vec3s origin;			// Origin (x, y, z)
		rex_vec3s velocity;			// Velocity (x, y, z)
		rex_vec3i angles;			// Angles (pitch, yaw, roll)

		//
		// Type variables
		//

		rex_actor_type type;		// Type enum

		//
		// Scene hierarchy
		//

		Actor *parent;				// Parent in the scene
		vector<Actor *> children;	// Children in the scene
};

} // namespace Rex

#endif // __REX_SCENE_H__
