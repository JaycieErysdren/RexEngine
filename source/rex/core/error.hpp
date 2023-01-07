// ========================================================
//
// FILE:			/source/rex/core/error.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Error header
//
// LAST EDITED:		January 6th, 2023
//
// ========================================================

#ifndef __REX_ERROR_H__
#define __REX_ERROR_H__

//
//
// Rex namespace: Error
//
//

namespace Rex
{

typedef enum
{
	MESSAGE,
	WARNING,
	FAILURE,
} message_type;

// Set the engine's message handler
bool SetMessageHandler(bool (*MessageHandler)(const char *, const char *, message_type, time_t));

// Show a message to the user
bool Message(const char *title, const char *message, message_type type, time_t time);

// Show a message to the user
bool Message(const char *title, const char *message, message_type type);

// Show a message to the user
bool Message(const char *title, const char *message);

} // namespace Rex

#endif // __REX_ERROR_H__
