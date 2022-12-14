// ========================================================
//
// FILE:			/source/rex_test/rex_test.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Main game entry point
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

// Header
#include "rex_test.hpp"

rex_int HTML_Parse(Tag *parent, string str)
{
	// variables
	Tag child;
	rex_int i, c;
	bool closing_tag = false;

	string tag_open = "";
	string tag_close = "";
	ostringstream tag_content;

	ostringstream tempstr;

	// parse the string
	for (i = 0, c = 0; i < str.length(); i++)
	{
		// clear tempstr
		tempstr.str("");
		tempstr.clear();

		// parse opening and closing tags
		if (str[i] == '<')
		{
			// if its not a closing tag and the open tag is already filled, it must be a child
			// otherwise just parse the tag
			if (str[i + 1] != '/' && tag_open.empty() == false)
			{
				c = HTML_Parse(&child, str.substr(i));
				if (c == -1) break;
				i += c;
			}
			else
			{
				// advance i
				i++;

				// parse the tag
				while (str[i] != '>')
				{
					// if there's a slash, its a closing tag
					if (str[i] == '/')
					{
						closing_tag = true;
						i++;
					}

					// put the character into the tempstr
					tempstr << str[i];

					// advance i
					i++;
				}

				// put in results
				if (closing_tag == false)
				{
					tag_open = tempstr.str();

					if (tag_open.compare("hr") == 0 || tag_open.compare("br") == 0)
					{
						child.type = tag_open;
						child.content = "";
						parent->children.push_back(child);
						return i;
					}
				}
				else
				{
					tag_close = tempstr.str();

					if (tag_open.empty() == false && tag_open.compare(tag_open) == 0)
					{
						child.type = tag_open;
						child.content = tag_content.str();
						parent->children.push_back(child);
						return i;
					}
				}
			}
		}
		else if (tag_open.empty() == false)
		{
			tag_content << str[i];
		}
	}

	// error
	return -1;
}

#ifdef UGH

// Parse a string
rex_int HTML_ParseString(Tag *parent, char *str, rex_int start, rex_int end)
{
	// variables
	rex_int buffer_size = 80 * 40;
	rex_int i = 0;
	rex_int c = 0;
	ostringstream oss;
	bool child = false;
	bool closed = false;

	Tag *tag = new Tag;

	// parse the characters in the string
	while (start + i < end)
	{
		// clear our stream string
		oss.str("");
		oss.clear();
		c = 0;

		//cout << tag.content << endl;
		//cout << tag.type << endl;
		//cout << i << " - " << str[start + i] << endl;

		// parse tags
		if (str[start + i] == '<' && str[start + i + 1] == '/')
		{
			// parse closing tag
			i += 2;
			while (str[start + i] != '>' && str[start + i] != ' ')
			{
				oss << str[start + i];
				i++;
			}

			// print
			//cout << "OPENING TAG: " << tag->type << endl;
			//cout << "CONTENT: " << tag->content << endl;
			cout << "CLOSING TAG: " << oss.str() << endl << endl;

			// check if malformed tag
			if (tag->type.compare(oss.str()) == 0)
			{
				parent->children.push_back(tag);
				return i;
			}
		}
		else if (str[start + i] == '<')
		{
			// parse opening tag
			i++;
			while (str[start + i] != '>' && str[start + i] != ' ')
			{
				oss << str[start + i];
				i++;
			}

			i++;

			// set the tag type
			tag->type = oss.str();

			// print
			cout << "OPENING TAG: " <<  tag->type << endl;
		}
		else if (tag->type.empty() == false)
		{
			// parse content inside tag
			while (str[start + i] != '<')
			{
				if ((str[start + i] != '\t') && // tab
					(str[start + i] != '\r') && // carriage return
					(str[start + i] != '\n')) // newline
				{
					oss << str[start + i];
				}

				i++;
			}

			// set the tag content
			tag->content += oss.str();

			// print
			cout << "TAG CONTENT: " <<  tag->content << endl;

			// parse the children
			if (str[start + i] != '/')
			{
				c = HTML_ParseString(tag, str, start + i, end);
			}

			// iterate
			i += c;
		}
		else
		{
			i++;
		}
	}

	// return end position of this str
	return i;
}

// Parse a HTML file
Tag *HTML_ParseFile(string filename)
{
	// variables
	Tag *document = new Tag;
	char *filebuffer;
	rex_int filesize;
	FILE *file;

	// open file
	file = fopen(filename.c_str(), "rb");
	if (file == NULL) return NULL;

	// get filesize
	fseek(file, 0L, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0L, SEEK_SET);

	// allocate buffer
	filebuffer = (char *)malloc(filesize);
	if (filebuffer == NULL) return NULL;

	// read text into buffer
	fread(filebuffer, sizeof(char), filesize, file);

	// parse the buffer recursively
	HTML_ParseString(document, filebuffer, 0, filesize);

	// free buffer
	free(filebuffer);

	// return document
	return document;
}

#endif

// Determine file type (lazy)
rex_int HTML_DetermineTagType(Tag tag)
{
	if (tag.type.compare("html") == 0) return TAG_HTML;
	if (tag.type.compare("h1") == 0) return TAG_H1;
	if (tag.type.compare("h2") == 0) return TAG_H2;
	if (tag.type.compare("h3") == 0) return TAG_H3;
	if (tag.type.compare("h4") == 0) return TAG_H4;
	if (tag.type.compare("h5") == 0) return TAG_H5;
	if (tag.type.compare("h6") == 0) return TAG_H6;
	if (tag.type.compare("h7") == 0) return TAG_P;
	if (tag.type.compare("br") == 0) return TAG_BR;
	if (tag.type.compare("hr") == 0) return TAG_HR;
	if (tag.type.compare("span") == 0) return TAG_SPAN;

	return TAG_NONE;
}

// render HTML
rex_int HTML_Render(Rex::Surface *dst, Rex::Surface *font, Tag tag, rex_int row, rex_int col)
{
	rex_int i;
	rex_int font_size = 1;
	rex_int next_row;
	rex_int next_col;

	rex_int tag_type = HTML_DetermineTagType(tag);

	switch (tag_type)
	{
		// line break
		case TAG_BR:
		{
			next_row = row + font_size;
			next_col = col;
			break;
		}

		// horizontal ruler
		case TAG_HR:
		{
			string ruler (80 - col - 4, 196);
			Rex::ConsoleTextF(dst, font, 8 * font_size, col + 2, row, ruler.c_str());

			next_row = row + font_size;
			next_col = col;

			break;
		}

		// H1
		case TAG_H1: font_size = 2;

		// render the text content of the element
		default:
		{
			// if the element has text content, render it
			if (tag.content.empty() == false)
			{
				Rex::ConsoleTextF(dst, font, 8 * font_size, col, row, tag.content.c_str());

				rex_int num_lines = ceil(float(tag.content.length()) / 80);

				next_row = row + (font_size * num_lines);
				next_col = col /* + (font_size * tag.content.length()) */;
			}
			else
			{
				next_row = row;
				next_col = col;
			}
			break;
		}
	}

	// render children
	for (i = 0; i < tag.children.size(); i++)
	{
		next_row = HTML_Render(dst, font, tag.children[i], next_row, next_col);
	}

	return next_row;
}
