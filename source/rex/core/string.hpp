// ========================================================
//
// FILE:			/source/rex/core/string.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: String header
//
// LAST EDITED:		January 2nd, 2023
//
// ========================================================

#ifndef __REX_STRING_H__
#define __REX_STRING_H__

//
//
// Rex namespace: String
//
//

namespace Rex
{

// String class
class rex_string
{
	public:
		// Variables
		char *buf;
		size_t len;

		// Constructors
		rex_string();
		rex_string(const char *str);

		// Destructor
		~rex_string();

		// Operators
		void operator=(const rex_string &in);
		void operator=(const char *in);
		void operator+=(const rex_string &in);
		void operator+=(const char *in);
		void operator+=(const rex_int &len);
		void operator-=(const rex_int &len);
};

} // namespace Rex

#endif // __REX_STRING_H__
