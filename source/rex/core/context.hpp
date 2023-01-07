// ========================================================
//
// FILE:			/source/rex/core/context.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Context header
//
// LAST EDITED:		January 6th, 2023
//
// ========================================================

#ifndef __REX_CONTEXT_H__
#define __REX_CONTEXT_H__

//
//
// Rex namespace: Context
//
//

namespace Rex
{

//
//
// Functions
//
//

// The main engine context class
class EngineContext
{
	public:

		//
		// Basic information
		//

		// Start time
		time_t time_start;

		// End time
		time_t time_end;

		//
		// Function pointers
		//

		bool (*MessageHandler)(const char *, const char *, message_type, time_t);

		//
		// Functions
		//

		// Constructor & Destructor
		EngineContext();
		~EngineContext();
};

// Engine context
extern EngineContext *engine_context;

} // namespace Rex

#endif // __REX_CONTEXT_H__
