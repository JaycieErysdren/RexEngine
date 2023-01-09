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

//
//
// rex_string class
//
//

//
// Constructors & destructors
//

// Generic constructor
rex_string::rex_string()
{
	this->len = 0;
	this->buf = nullptr;
}

// Constructor with char sequence
rex_string::rex_string(const char *str)
{
	this->buf = (char *)calloc(strlen(str), sizeof(char));
	this->len = strlen(str);
	memcpy(this->buf, str, this->len);
}

// Constructor with length
rex_string::rex_string(const rex_int len)
{
	this->len = len;
	this->buf = (char *)calloc(this->len + 1, sizeof(char));
}

// Destructor
rex_string::~rex_string()
{
	if (this->buf != nullptr) free(this->buf);
}

//
// Functions
//

// Clear string
void rex_string::clear()
{
	if (this->buf != nullptr) free(this->buf);
	this->len = 0;
}

// Replace character in string
void rex_string::replace(char find, char replace)
{
	if (this->buf != nullptr)
	{
		for (rex_int i = 0; i < this->len; i++)
		{
			if (this->buf[i] == find)
			{
				this->buf[i] = replace;
			}
		}
	}
}

//
// Operators
//

// Set string equal to string
void rex_string::operator=(const rex_string &in)
{
	if (this->buf != nullptr) free(this->buf);

	this->buf = in.buf;
	this->len = in.len;
}

// Set string equal to char sequence
void rex_string::operator=(const char *in)
{
	if (this->buf != nullptr) free(this->buf);

	this->buf = (char *)calloc(strlen(in) + 1, sizeof(char));
	this->len = strlen(in);
	memcpy(this->buf, in, this->len);
}

// Set string equal to length
void rex_string::operator=(const rex_int &len)
{
	char *outbuf;
	rex_int outlen;

	outlen = len;
	outbuf = (char *)calloc(outlen + 1, sizeof(char));

	if (this->buf != nullptr) free(this->buf);

	this->buf = outbuf;
	this->len = outlen;
}

// Concat string with string
void rex_string::operator+=(const rex_string &in)
{
	char *outbuf;
	rex_int outlen;

	outlen = this->len + in.len;
	outbuf = (char *)calloc(outlen + 1, sizeof(char));

	if (this->buf != nullptr)
	{
		memcpy(outbuf, this->buf, this->len);
		memcpy(outbuf + this->len, in.buf, in.len);
		free(this->buf);
	}
	else
	{
		memcpy(outbuf, in.buf, in.len);
	}

	this->buf = outbuf;
	this->len = outlen;
}

// Concat string with char sequence
void rex_string::operator+=(const char *in)
{
	char *outbuf;
	rex_int outlen;

	outlen = this->len + strlen(in);
	outbuf = (char *)calloc(outlen + 1, sizeof(char));

	if (this->buf != nullptr)
	{
		memcpy(outbuf, this->buf, this->len);
		memcpy(outbuf + this->len, in, strlen(in));
		free(this->buf);
	}
	else
	{
		memcpy(outbuf, in, strlen(in));
	}

	this->buf = outbuf;
	this->len = outlen;
}

// Pad the string with N blank chars
void rex_string::operator+=(const rex_int &len)
{
	char *outbuf;
	rex_int outlen;

	outlen = this->len + len;
	outbuf = (char *)calloc(outlen + 1, sizeof(char));

	if (this->buf != nullptr)
	{
		memcpy(outbuf, this->buf, this->len);
		free(this->buf);
	}

	this->buf = outbuf;
	this->len = outlen;
}

// Remove N chars from the end of the string
void rex_string::operator-=(const rex_int &len)
{
	char *outbuf;
	rex_int outlen;

	if (this->buf && this->len != 0)
	{
		outlen = this->len - len;

		if (outlen < 1)
		{
			this->len = 0;
			free(this->buf);
		}
		else
		{
			outbuf = (char *)calloc(outlen + 1, sizeof(char));
			memcpy(outbuf, this->buf, outlen);
			free(this->buf);

			this->buf = outbuf;
			this->len = outlen;
		}
	}
}

// Compare string to string
bool rex_string::operator==(const rex_string &in)
{
	if (strcmp(this->buf, in.buf) == 0)
		return true;

	return false;
}

// Compare string to char sequence
bool rex_string::operator==(const char *in)
{
	if (strcmp(this->buf, in) == 0)
		return true;

	return false;
}

} // namespace Rex
