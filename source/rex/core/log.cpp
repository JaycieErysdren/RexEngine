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
// LAST EDITED:		December 18th, 2022
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

// Log a non-critical message to the console and a log file. Pass NULL as the filename to skip file logging.
void Log(const char *filename, const char *fmt, ...)
{
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

	// Log to a file if filename given
	if (filename != NULL)
	{
		FILE *log_file = fopen(filename, "a");

		if (log_file != NULL)
		{
			fwrite(log_buffer, sizeof(char), strlen(log_buffer), log_file);
			fclose(log_file);
		}
	}

	// Print the message to stdout
	//printf("%s", log_buffer);
}

} // namespace Rex
