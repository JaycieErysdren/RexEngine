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
	ACTOR_MODEL,
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
		rex_int fov;				// Field of view from this actor
		rex_scalar draw_distance;	// How far this actor can "see"
		rex_int anglespeedkey;		// Multiplier for move speed
		rex_int movespeedkey;		// Multiplier for turn speed
		rex_string identifier;		// Identifier name

		//
		// Type variables
		//

		rex_actor_type type;		// Type enum

		//
		// Scene hierarchy
		//

		Actor *parent;				// Parent in the scene
		vector<Actor *> children;	// Children in the scene

		//
		// Model data
		//

		void *model;
	
		//
		// Danger zone
		//

		void *memory;				// Pointer to the allocated memory for this actor
};

//
//
// Functions
//
//

// Allocates the memory associated with an actor, and returns a pointer to it
Actor *AddActor(Actor *parent, rex_actor_type type);

// Frees the memory associated with an actor, as well as all children actors
void FreeActor(Actor *actor);

} // namespace Rex

#endif // __REX_SCENE_H__
