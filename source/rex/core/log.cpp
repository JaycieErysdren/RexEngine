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
// Global Variables
//
//

FILE *log_file;

//
//
// Functions
//
//

//
// Bootstrap
//

// Open log file handle
bool Log_Init()
{
	log_file = fopen("rex.log", "a");

	if (log_file == NULL) return false;

	return true;
}

// Close log file handle
bool Log_Shutdown()
{
	if (log_file)
	{
		if (fclose(log_file) != 0) return false;
	}

	return true;
}

//
// Logging
//

// Log a non-critical message to the console and a log file.
void Log(const char *fmt, ...)
{
	if (log_file == NULL) return;

	// Variables
	va_list args;
	char log_buffer[1024];
	char time_buffer[22];

	// Get current time
	time_t time_raw = time(NULL);
	struct tm *time_info = localtime(&time_raw);
	sprintf(time_buffer, "%04d-%02d-%02d %02d-%02d-%02d: ",
		time_info->tm_year + 1900,
		time_info->tm_mon,
		time_info->tm_mday,
		time_info->tm_hour,
		time_info->tm_min,
		time_info->tm_sec
	);

	// Copy time into buffer
	strncpy(log_buffer, time_buffer, sizeof(time_buffer));

	// Print args into string
	va_start(args, fmt);
	vsprintf(log_buffer + (sizeof(time_buffer) - 1), fmt, args);
	va_end(args);

	// Add newline
	strcpy(log_buffer + strlen(log_buffer), "\n");

	// Log to the file
	fwrite(log_buffer, sizeof(char), strlen(log_buffer), log_file);
}

} // namespace Rex
