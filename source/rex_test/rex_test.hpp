// ========================================================
//
// FILE:			/source/game/game.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Main game header
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

#ifndef __REX_TEST_H__
#define __REX_TEST_H__

// Rex Engine headers
#include "rex/rex.hpp"

// HTML tag attribute
class Attribute
{
	public:
		string type;
		string content;
};

// HTML tag
class Tag
{
	public:
		string type;
		string content;

		vector<Tag> children;
		vector<Attribute> attributes;
};

typedef enum
{
	TAG_NONE,	// unrecognized
	TAG_HTML,	// <html>
	TAG_H1,		// <h1>
	TAG_H2,		// <h2>
	TAG_H3,		// <h3>
	TAG_H4,		// <h4>
	TAG_H5,		// <h5>
	TAG_H6,		// <h6>
	TAG_P,		// <p>
	TAG_BR,		// <br>
	TAG_HR,		// <hr>
	TAG_SPAN,	// <span>
} html_tags;

// globals
extern Rex::Surface pic_font;
extern Rex::Surface pic_bbuffer;
extern Rex::VidInfo vidinfo;

// HTML functions
//rex_int HTML_ParseString(Tag *parent, char *str, rex_int start, rex_int end);
//Tag *HTML_ParseFile(string filename);
//rex_int HTML_DetermineTagType(Tag *tag);
rex_int HTML_Parse(Tag *parent, string str);
rex_int HTML_Render(Rex::Surface *dst, Rex::Surface *font, Tag tag, rex_int row, rex_int col);

#endif // __REX_TEST_H__
