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
// Classes
//
//

// The main engine graphics context class
class EngineGraphicsContext
{
	public:
		rex_int width;
		rex_int height;
		rex_int bpp;
		rex_string title;
		void *platform;

		// Constructor
		EngineGraphicsContext();

		// Destructor
		~EngineGraphicsContext();
};

// The main engine context class
class EngineContext
{
	public:

		//
		// Variables
		//

		// Start time (epoch)
		time_t time_start;

		// End time (epoch)
		time_t time_end;

		// Graphics context
		EngineGraphicsContext *graphics_context;

		// Log file handle
		FILE *log_file_handle;

		// Multi-purpose string buffer
		rex_string string_buffer;

		// MessageHandler function pointer
		bool (*MessageHandler)(const char *, const char *, message_type, time_t);
		bool (*PrintHandler)(const char *, message_type);

		//
		// Functions
		//

		// Constructor & Destructor
		EngineContext();
		~EngineContext();

		// Dump information about current session
		bool DumpSession();

		// Log the formatted string input to this session's logfile
		bool Log(const char *str);

		// Enable logging
		bool EnableLogging();

		// Enable logging with custom filename
		bool EnableLogging(const char *filename);

		// Disable logging
		bool DisableLogging();
};

//
//
// Globals
//
//

// Engine context
extern EngineContext *engine_context;

//
//
// Functions
//
//

bool DumpEngineContext();

} // namespace Rex

#endif // __REX_CONTEXT_H__
