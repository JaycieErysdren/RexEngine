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

// render HTML
void RenderHTML(Tag root, rex_int row, rex_int column)
{
	rex_int font_size;

	if (root.type.compare("h1") == 0)
		font_size = 16;
	else
		font_size = 12;

	// add the text
	Rex::ConsoleTextF(&pic_bbuffer, &pic_font, font_size, column, row, root.content.c_str());

	// render children
	for (rex_int i = 0; i < root.children.size(); i++)
	{
		RenderHTML(root.children[i], row + 1, column);
	}
}

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// hello
	Initialize();

	// tags
	Tag root;
	root.type = "html";

	Tag child1;
	child1.content = "Header 1";
	child1.type = "h1";

	root.children.push_back(child1);

	Tag child2;
	child2.content = "Paragraph";
	child2.type = "p";

	root.children.push_back(child2);

	// Main loop
	while (!Rex::KeyTest(REX_SC_ESCAPE))
	{
		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 0);

		// Render HTML
		RenderHTML(root, 0, 0);

		// Flip the rendering buffers
		Rex::SurfaceToFrontBuffer(&pic_bbuffer);
	}

	// goodbye
	Shutdown();

	// Exit gracefully
	return EXIT_SUCCESS;
}
