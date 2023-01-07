// ========================================================
//
// FILE:			/source/rex/core/error.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Error implementation
//
// LAST EDITED:		January 6th, 2023
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Error
//
//

namespace Rex
{

//
//
// Functions
//
//

// Set the engine's message handler
bool SetMessageHandler(bool (*MessageHandler)(const char *, const char *, message_type, time_t))
{
	// you can't do this if the engine hasn't been initialized
	if (!engine_context) return false;

	// set the handler and return
	engine_context->MessageHandler = MessageHandler;
	return true;
}

// Show a message to the user
bool Message(const char *title, const char *message, message_type type, time_t time)
{
	if (*engine_context->MessageHandler)
	{
		return (*engine_context->MessageHandler)(title, message, type, time);
	}
	else
	{
		cout << title << endl;
		cout << message << endl;
		return false;
	}
}

// Show a message to the user
bool Message(const char *title, const char *message, message_type type)
{
	if (*engine_context->MessageHandler)
	{
		return (*engine_context->MessageHandler)(title, message, type, time(NULL));
	}
	else
	{
		cout << title << endl;
		cout << message << endl;
		return false;
	}
}

// Show a message to the user
bool Message(const char *title, const char *message)
{
	if (*engine_context->MessageHandler)
	{
		return (*engine_context->MessageHandler)(title, message, MESSAGE, time(NULL));
	}
	else
	{
		cout << title << endl;
		cout << message << endl;
		return false;
	}
}

} // namespace Rex
