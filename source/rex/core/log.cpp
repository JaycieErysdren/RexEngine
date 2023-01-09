// ========================================================
//
// FILE:			/source/rex/core/log.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Log implementation
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Log
//
//

namespace Rex
{

//
//
// Functions
//
//

//
// Bootstrap
//

// Enable logging
bool EnableLogging()
{
	return EnableLogging();
}

// Enable logging with specified filename
bool EnableLogging(const char *filename)
{
	if (engine_context)
		return engine_context->EnableLogging(filename);

	return false;
}

// Disable logging
bool DisableLogging()
{
	if (engine_context)
		return engine_context->DisableLogging();

	return false;
}

//
// Logging
//

// Print the formatted string input to the console
bool Print(const char *fmt, ...)
{
	// Variables
	va_list args;
	char buf[1024];

	// Print args into string
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	// there must be a better way to do this
	if (*engine_context->PrintHandler)
	{
		// Send to engine context
		return (*engine_context->PrintHandler)(buf, MESSAGE);
	}
	else
	{
		cout << buf << endl;
		return false;
	}
}

// Print the formatted string input to the console, with warning level
bool Print(message_type type, const char *fmt, ...)
{
	// Variables
	va_list args;
	char buf[1024];

	// Print args into string
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	// there must be a better way to do this
	if (*engine_context->PrintHandler)
	{
		// Send to engine context
		return (*engine_context->PrintHandler)(buf, type);
	}
	else
	{
		cout << buf << endl;
		return false;
	}
}

// Log a non-critical message to the console and a log file.
bool Log(const char *fmt, ...)
{
	// there must be a better way to do this
	if (engine_context)
	{
		// Variables
		va_list args;
		char buf[1024];

		// Print args into string
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		va_end(args);

		return engine_context->Log(buf);
	}

	return false;
}

} // namespace Rex
