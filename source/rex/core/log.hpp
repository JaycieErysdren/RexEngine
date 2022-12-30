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

// Open log file handle
bool Log_Init();

// Close log file handle
bool Log_Shutdown();

//
// Logging
//

// Log a non-critical message to the console and a log file. Pass an empty string as the filename to skip file logging.
void Log(const char *filename, const char *fmt, ...);

} // namespace Rex

#endif // __REX_LOG_H__
