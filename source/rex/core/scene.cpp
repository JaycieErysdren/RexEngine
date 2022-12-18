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
// Classes
//
//

// Allocates the memory associated with an actor, and returns a pointer to it
Actor *AddActor(Actor *parent, rex_actor_type type)
{
	// Allocate memory and assign actor to it
	void *actor_memory = MemPool_Alloc(MEMORY_ACTORS, sizeof(Actor));
	Actor *actor = new(actor_memory) Actor;

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
void FreeActor(Actor *actor)
{
	if (actor == NULL) return;

	void *actor_memory = actor->memory;

	// Free all the children of this actor
	for (rex_int i = 0; i < actor->children.size(); i++)
	{
		FreeActor(actor->children[i]);
	}

	// Remove this actor from the parent
	if (actor->parent)
	{
		remove(actor->parent->children.begin(), actor->parent->children.end(), actor);
	}

	// Call destructor
	actor->~Actor();

	// Free the actor
	MemPool_Free(MEMORY_ACTORS, actor_memory);
}

} // namespace Rex
