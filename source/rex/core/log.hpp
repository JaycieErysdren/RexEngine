// ========================================================
//
// FILE:			/source/rex/core/log.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Log header
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

#ifndef __REX_LOG_H__
#define __REX_LOG_H__

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
bool EnableLogging();

// Enable logging with specified filename
bool EnableLogging(const char *filename);

// Disable logging
bool DisableLogging();

//
// Logging
//

// Print the formatted string input to the console
bool Print(const char *fmt, ...);

// Print the formatted string input to the console, with warning level
bool Print(message_type type, const char *fmt, ...);

// Log a non-critical message to the console and a log file.
bool Log(const char *fmt, ...);

} // namespace Rex

#endif // __REX_LOG_H__
