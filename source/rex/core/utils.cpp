// ========================================================
//
// FILE:			/source/rex/core/utils.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Utils implementation
//
// LAST EDITED:		January 6th, 2023
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Utils
//
//

namespace Rex
{

//
//
// Functions
//
//

// Ugh
rex_string time_string;

rex_string &ConvertTime(time_t time)
{
	// Variables
	va_list args;

	// Set length of time string
	time_string = 19;

	// Convert time and sprintf it
	struct tm *info = localtime(&time);
	sprintf(time_string.buf, "%04d-%02d-%02d %02d-%02d-%02d",
		info->tm_year + 1900,
		info->tm_mon,
		info->tm_mday,
		info->tm_hour,
		info->tm_min,
		info->tm_sec
	);

	return time_string;
}

} // namespace Rex
