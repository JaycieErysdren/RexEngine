// ========================================================
//
// FILE:			/source/rex/core/string.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: String implementation
//
// LAST EDITED:		January 2nd, 2023
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: String
//
//

namespace Rex
{

// Generic constructor
rex_string::rex_string()
{
	
}

// Constructor with const char pointer
rex_string::rex_string(const char *str)
{
	this->buf = (char *)calloc(strlen(str), sizeof(char));
	this->len = strlen(str);
	memcpy(this->buf, str, this->len);
}

// Destructor
rex_string::~rex_string()
{
	if (this->buf) free(this->buf);
}

rex_string rex_string::operator=(const rex_string &in)
{
	rex_string string;

	string.len = in.len;
	string.buf = in.buf;

	return string;
}

void rex_string::operator+=(const rex_string &in)
{
	char *outbuf;
	rex_int outlen;

	outlen = this->len + in.len;
	outbuf = (char *)calloc(outlen + 1, sizeof(char));

	memcpy(outbuf, this->buf, this->len);
	memcpy(outbuf + this->len, in.buf, in.len);

	if (this->buf) free(this->buf);

	this->buf = outbuf;
	this->len = outlen;
}


} // namespace Rex
