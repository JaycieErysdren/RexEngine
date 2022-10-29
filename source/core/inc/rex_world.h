// ========================================================
//
// FILE:			/source/core/inc/rex_world.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for world handling.
//
// LAST EDITED:		October 29th, 2022
//
// ========================================================

//
// World base types
//

typedef struct
{
	PHYS_body *phys_body;
	br_actor *br_actor;
} rex_actor;

typedef struct
{
	// metadata
	rex_byte *identifier;

	// root
	br_actor *root;

	// actors
	rex_actor *actors;
	rex_int num_actors;

	// physics
	PHYS_closest_point_function environment_function;
	PHYS_collision_callback collision_callback;
} rex_world;
