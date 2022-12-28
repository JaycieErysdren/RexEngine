// ========================================================
//
// FILE:			/source/rex/core/scene.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Scene implementation
//
// LAST EDITED:		December 18th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Scene
//
//

namespace Rex
{

//
//
// Class functions
//
//

// Returns true if the actor is overtop or underneath another actor
bool Actor2D::OriginInside(Actor2D *actor)
{
	return (origin.x > actor->origin.x + actor->draw_area.x1 &&
			origin.x < actor->origin.x + actor->draw_area.x2 &&
			origin.y > actor->origin.y + actor->draw_area.y1 &&
			origin.y < actor->origin.y + actor->draw_area.y2) ? true : false;
}

// Clear the events bitmask in the actor
void Actor2D::ClearEvents()
{
	events = 0;
}

//
//
// Functions
//
//

//
// Actor3D
//

// Allocates the memory associated with an actor, and returns a pointer to it
Actor3D *AddActor3D(Actor3D *parent, rex_actor3d_type type)
{
	// Allocate memory and assign actor to it
	void *actor_memory = MemPool_Alloc(MEMORY_ACTORS, sizeof(Actor3D));
	Actor3D *actor = new(actor_memory) Actor3D;

	actor->memory = actor_memory;

	// If parent, add them
	if (parent)
	{
		actor->parent = parent;
		parent->children.push_back(actor);
	}

	// Set type
	actor->type = type;

	// Return pointer
	return actor;
}

// Frees the memory associated with an actor, as well as all children actors
void FreeActor3D(Actor3D *actor)
{
	if (actor == NULL) return;

	void *actor_memory = actor->memory;

	// Free all the children of this actor
	for (rex_int i = 0; i < actor->children.size(); i++)
	{
		FreeActor3D(actor->children[i]);
	}

	// Remove this actor from the parent
	if (actor->parent)
	{
		remove(actor->parent->children.begin(), actor->parent->children.end(), actor);
	}

	// Free model
	switch (actor->type)
	{
		case ACTOR3D_VOXELMODEL:
			//Voxel::FreeVoxelModel((Voxel::VoxelModel *)actor->model);
			break;

		case ACTOR3D_RAYCASTMODEL:
			//Raycast::FreeRaycastModel((Raycast::RaycastModel *)actor->model);
			break;

		default:
			break;
	}

	// Call destructor
	actor->~Actor3D();

	// Free the actor
	MemPool_Free(MEMORY_ACTORS, actor_memory);
}

//
// Actor2D
//

// Allocates the memory associated with an Actor2D, and returns a pointer to it
Actor2D *AddActor2D(Actor2D *parent, rex_actor2d_type type)
{
	// Allocate memory and assign actor to it
	void *actor_memory = MemPool_Alloc(MEMORY_ACTORS, sizeof(Actor2D));
	Actor2D *actor = new(actor_memory) Actor2D;

	actor->memory = actor_memory;

	// If parent, add them
	if (parent)
	{
		actor->parent = parent;
		parent->children.push_back(actor);
	}

	// Set type & blit mode
	actor->type = type;

	// Return pointer
	return actor;
}

// Frees the memory associated with an Actor2D, as well as all children actors
void FreeActor2D(Actor2D *actor)
{
	if (actor == NULL) return;

	void *actor_memory = actor->memory;

	// Free all the children of this actor
	for (rex_int i = 0; i < actor->children.size(); i++)
	{
		FreeActor2D(actor->children[i]);
	}

	// Remove this actor from the parent
	if (actor->parent)
	{
		remove(actor->parent->children.begin(), actor->parent->children.end(), actor);
	}

	// Remove the color buffer
	FreeSurface(actor->color);

	// Call destructor
	actor->~Actor2D();

	// Free the actor
	MemPool_Free(MEMORY_ACTORS, actor_memory);
}

// Renders an Actor2D to the specified surface, along with all its children
void RenderActor2D(Surface *dst, Actor2D *actor)
{
	if (actor == NULL) return;

	//actor->Draw(dst, actor->color_blit_mode);

	for (rex_int i = 0; i < actor->children.size(); i++)
	{
		RenderActor2D(dst, actor->children[i]);
	}
}

} // namespace Rex
