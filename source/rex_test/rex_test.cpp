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

#define CYCLES 30

//
// Globals
//

// Picture buffers
Rex::Surface pic_font;
Rex::Surface pic_bbuffer;

// Video info
Rex::VidInfo vidinfo;

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
} html_tags;

// Intiialization function
void Initialize()
{
	// Initialize Rex Engine
	Rex::Initialize();

	// Initialize graphics
	if (Rex::InitializeGraphics(640, 480, 8) == false)
	{
		cout << "failed to initialize graphics driver" << endl;
		exit(EXIT_FAILURE);
	}

	vidinfo = Rex::GetVidInfo();

	// Create picture buffers
	Rex::SurfaceLoadBMP(&pic_font, "gfx/font8x8.bmp");
	Rex::SurfaceCreate(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);
}

void Shutdown()
{
	// Cleanup picture buffers
	Rex::SurfaceDestroy(&pic_font);
	Rex::SurfaceDestroy(&pic_bbuffer);

	// Shutdown Graphics
	Rex::ShutdownGraphics();

	// Shutdown Rex Engine
	Rex::Shutdown();
}

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

	return TAG_NONE;
}

// render HTML
rex_int RenderHTML(Tag tag, rex_int row, rex_int col)
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
			Rex::ConsoleTextF(&pic_bbuffer, &pic_font, 8 * font_size, col + 2, row, ruler.c_str());

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
				Rex::ConsoleTextF(&pic_bbuffer, &pic_font, 8 * font_size, col, row, tag.content.c_str());

				rex_int num_lines = ceil(float(tag.content.length()) / 80);

				next_row = row + (font_size * num_lines);
				next_col = col + (font_size * tag.content.length());
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
		next_row = RenderHTML(tag.children[i], next_row, next_col);
	}

	return next_row;
}

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// hello
	Initialize();

	// document root
	Tag document;
	document.type = "html";

	// header
	Tag h1;
	h1.content = " Lorem Ipsum";
	h1.type = "h1";

	// line break
	Tag br;
	br.type = "br";	

	// horizontal ruler
	Tag hr;
	hr.type = "hr";	

	// lorem ipsum
	Tag p_lorem;
	p_lorem.content = "  Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	p_lorem.type = "p";

	// assemble document
	document.children.push_back(h1);
	document.children.push_back(br);
	document.children.push_back(hr);
	document.children.push_back(br);
	document.children.push_back(p_lorem);
	document.children.push_back(br);
	document.children.push_back(hr);
	document.children.push_back(br);
	document.children.push_back(p_lorem);
	document.children.push_back(br);

	// Main loop
	while (!Rex::KeyTest(REX_SC_ESCAPE))
	{
		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 0);

		// Render HTML
		RenderHTML(document, 0, 0);

		// Flip the rendering buffers
		Rex::SurfaceToFrontBuffer(&pic_bbuffer);
	}

	// goodbye
	Shutdown();

	// Exit gracefully
	return EXIT_SUCCESS;
}
