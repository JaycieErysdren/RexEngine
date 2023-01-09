// ========================================================
//
// FILE:			/source/rex/core/context.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Context implementation
//
// LAST EDITED:		January 6th, 2023
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Context
//
//

namespace Rex
{

//
//
// Globals
//
//

// Engine context
EngineContext *engine_context;

//
//
// Functions
//
//

//
// Public functions
//

bool DumpEngineContext()
{
	if (engine_context)
		return engine_context->DumpSession();
	else
		return false;
}

//
// EngineGraphicsContext class
//

// Constructor
EngineGraphicsContext::EngineGraphicsContext()
{
	// just to make sure
	this->platform = nullptr;
}

// Destructor
EngineGraphicsContext::~EngineGraphicsContext()
{
	// clear platform-specific handle
	if (this->platform != nullptr)
		Platform_Quit_Graphics(this->platform);
}

//
// EngineContext class
//

// Constructor
EngineContext::EngineContext()
{
	// Set start time
	this->time_start = time(NULL);

	// Set default message handler
	this->MessageHandler = Platform_MessageHandler;
	this->PrintHandler = Platform_PrintHandler;

	// null these out for checking later
	this->graphics_context = nullptr;
	this->log_file_handle = NULL;
}

// Destructor
EngineContext::~EngineContext()
{
	// Quit graphics if it was started
	if (this->graphics_context != nullptr)
		delete this->graphics_context;

	// Set end time
	this->time_end = time(NULL);

	// close logfile handle
	if (this->log_file_handle != NULL)
		fclose(this->log_file_handle);
}

// Dump information about current session
bool EngineContext::DumpSession()
{
	// set filename strings
	rex_string filename = "rex_session_";
	time_t dumptime = time(NULL);
	char buf[16];

	// add time to filename
	filename += ConvertTime(dumptime);
	filename += ".log";
	filename.replace(' ', '_');

	// open file handle
	FILE *dumpfile = fopen(filename.buf, "w");
	if (dumpfile == NULL)
		return false;

	// set information strings
	rex_string dump01 = "Rex Engine Session Info:";
	rex_string dump02 = "============================";
	rex_string dump03 = "Start Time: ";
	rex_string dump04 = "Dump Time: ";

	// append external info
	dump03 += ConvertTime(this->time_start);
	dump04 += ConvertTime(dumptime);

	// append line endings
	dump01 += "\n";
	dump02 += "\n";
	dump03 += "\n";
	dump04 += "\n";

	// write information dump
	fwrite(dump01.buf, sizeof(char), dump01.len, dumpfile);
	fwrite(dump02.buf, sizeof(char), dump02.len, dumpfile);
	fwrite(dump03.buf, sizeof(char), dump03.len, dumpfile);
	fwrite(dump04.buf, sizeof(char), dump04.len, dumpfile);

	// if there's a graphic context, dump that info
	if (this->graphics_context != nullptr)
	{
		rex_string dump05 = "Graphics Info:";
		rex_string dump06 = "============================";
		rex_string dump07 = "Width: ";
		rex_string dump08 = "Height: ";
		rex_string dump09 = "Bits per pixel: ";
		rex_string dump10 = "Title: ";

		// append external info
		sprintf(buf, "%d", this->graphics_context->width);
		dump07 += buf;
		sprintf(buf, "%d", this->graphics_context->height);
		dump08 += buf;
		sprintf(buf, "%d", this->graphics_context->bpp);
		dump09 += buf;
		dump10 += this->graphics_context->title;

		// append line endings
		dump05 += "\n";
		dump06 += "\n";
		dump07 += "\n";
		dump08 += "\n";
		dump09 += "\n";
		dump10 += "\n";

		fwrite("\n", sizeof(char), 1, dumpfile);
		fwrite(dump05.buf, sizeof(char), dump05.len, dumpfile);
		fwrite(dump06.buf, sizeof(char), dump06.len, dumpfile);
		fwrite(dump07.buf, sizeof(char), dump07.len, dumpfile);
		fwrite(dump08.buf, sizeof(char), dump08.len, dumpfile);
		fwrite(dump09.buf, sizeof(char), dump09.len, dumpfile);
		fwrite(dump10.buf, sizeof(char), dump10.len, dumpfile);
	}

	// close file handle
	fclose(dumpfile);

	return true;
}

// Log a message to the console and a log file.
bool EngineContext::Log(const char *str)
{
	if (this->log_file_handle == NULL)
		return false;

	rex_string tm = ConvertTime(time(NULL));

	// Log to the file
	fwrite(tm.buf, sizeof(char), tm.len, this->log_file_handle);
	fwrite(": ", sizeof(char), 2, this->log_file_handle);
	fwrite(str, sizeof(char), strlen(str), this->log_file_handle);
	fwrite("\n", sizeof(char), 1, this->log_file_handle);

	return true;
}

// Enable logging
bool EngineContext::EnableLogging()
{
	return this->EnableLogging("rex.log");
}

// Enable logging with custom filename
bool EngineContext::EnableLogging(const char *filename)
{
	if (this->log_file_handle != NULL)
		return false;

	this->log_file_handle = fopen(filename, "a");

	if (this->log_file_handle == NULL)
		return false;

	return true;
}

// Disable logging
bool EngineContext::DisableLogging()
{
	if (this->log_file_handle)
	{
		fclose(this->log_file_handle);
		return true;
	}

	return false;
}

} // namespace Rex
