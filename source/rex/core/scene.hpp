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
	ACTOR2D_NONE,
	ACTOR2D_WINDOW,
	ACTOR2D_BUTTON,
	ACTOR2D_TEXT,
	ACTOR2D_CURSOR,
} rex_actor2d_type;

typedef enum
{
	ACTOR2D_EVENT01 = 1 << 0,
	ACTOR2D_EVENT02 = 1 << 1,
	ACTOR2D_EVENT03 = 1 << 2,
	ACTOR2D_EVENT04 = 1 << 3,
	ACTOR2D_EVENT05 = 1 << 4,
	ACTOR2D_EVENT06 = 1 << 5,
	ACTOR2D_EVENT07 = 1 << 6,
	ACTOR2D_EVENT08 = 1 << 7,
} rex_actor2d_events;

typedef enum
{
	ACTOR3D_NONE,
	ACTOR3D_CAMERA,
	ACTOR3D_VOXELMODEL,
	ACTOR3D_POLYMODEL,
	ACTOR3D_SECTORMODEL,
	ACTOR3D_SPRITEMODEL,
} rex_actor3d_type;

//
//
// Classes
//
//

// Actor3D
class Actor3D
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
		rex_string identifier;		// Identifier

		//
		// Type variables
		//

		rex_actor3d_type type;		// Type enum

		//
		// Scene hierarchy
		//

		Actor3D *parent;				// Parent in the scene
		vector<Actor3D *> children;	// Children in the scene

		//
		// Model data
		//

		void *model;

		//
		// Danger zone
		//

		void *memory;				// Pointer to the allocated memory for this actor
};

// Actor2D
class Actor2D
{
	public:

		//
		// General variables
		//

		rex_vec2i origin;			// Origin (x, y)
		rex_vec2i dimensions;		// Dimensions (x, y)
		rex_rect draw_area;			// The area of the actor that should be drawn
		rex_vec2s velocity;			// Velocity (x, y)
		rex_int32 angle;			// Angle of rotation (0 - 359)
		rex_string identifier;		// Identifier
		rex_int32 events;			// Event mask

		//
		// Type variables
		//

		rex_actor2d_type type;		// Type enum

		//
		// Scene hierarchy
		//

		Actor2D *parent;				// Parent in the scene
		vector<Actor2D *> children;		// Children in the scene

		//
		// Color data
		//

		Surface color;
		blit_mode color_blit_mode;

		//
		// Danger zone
		//

		void *memory;				// Pointer to the allocated memory for this actor

		//
		// Functions
		//

		// Draw the actor on another surface
		void Draw(Surface *dst, blit_mode flags);

		// Returns true if the actor is overtop or underneath another actor
		bool OriginInside(Actor2D *actor);

		// Clear the events bitmask in the actor
		void ClearEvents();
};

//
//
// Functions
//
//

//
// Actor3D
//

// Allocates the memory associated with an actor, and returns a pointer to it
Actor3D *AddActor3D(Actor3D *parent, rex_actor3d_type type);

// Frees the memory associated with an actor, as well as all children actors
void FreeActor3D(Actor3D *actor);

//
// Actor2D
//

// Allocates the memory associated with an Actor2D, and returns a pointer to it
Actor2D *AddActor2D(Actor2D *parent, rex_actor2d_type type);

// Allocates the memory associated with an Actor2D, and returns a pointer to it
Actor2D *AddActor2D(Actor2D *parent, rex_actor2d_type type, string filename);

// Frees the memory associated with an Actor2D, as well as all children actors
void FreeActor2D(Actor2D *actor);

// Renders an Actor2D to the specified surface, along with all its children
void RenderActor2D(Surface *dst, Actor2D *actor);

} // namespace Rex

#endif // __REX_SCENE_H__
