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
// New surface type
//

class NewSurface
{
	public:
		rex_uint16 width;			// width of surface
		rex_uint16 height;			// height of surface
		rex_uint16 bpp;				// bits per pixel
		rex_uint16 bytes_per_row;	// bytes per row (includes padding)
		void *buffer;				// allocated pixel buffer
};

NewSurface *AddNewSurface(rex_uint16 width, rex_uint16 height, rex_uint16 bpp, void *buffer)
{
	NewSurface *surf = new NewSurface;

	surf->width = width;
	surf->height = height;
	surf->bpp = bpp;
	surf->bytes_per_row = (bpp / 8) * width;

	if (buffer == NULL)
	{
		surf->buffer = calloc(width * height, bpp / 8);
	}
	else
	{
		surf->buffer = buffer;
	}

	return surf;
}

void FreeNewSurface(NewSurface *surf)
{
	if (surf->buffer) free(surf->buffer);
	if (surf) delete surf;
}

//
// Globals
//

// Picture buffers
Rex::Surface pic_font;
Rex::Surface pic_bbuffer;

// Video info
Rex::VidInfo vidinfo;

// Intiialization function
void Initialize()
{
	// Initialize Rex Engine
	Rex::Initialize();

	// Initialize graphics
	if (Rex::InitializeGraphics(640, 480, 32) == false)
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

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// hello
	Initialize();

	Tag document;
	string html_str = "<html><h1>hello world</h1><br><hr><br><p>lorem ipsum</p></html>";

	HTML_Parse(&document, html_str);

	// load RGB8888 file
	FILE *file = fopen("test/forest.dat", "rb");
	rex_uint32 *buffer = (rex_uint32 *)calloc(640 * 480, sizeof(rex_uint32));

	// convert to RGB565
	for (rex_int i = 0; i < 640 * 480; i++)
	{
		rex_uint8 r = getc(file);
		rex_uint8 g = getc(file);
		rex_uint8 b = getc(file);
		rex_uint8 a = getc(file);

		buffer[i] = REX_COLOR_RGB_8888(r, g, b, a);
	}

	// close handle
	fclose(file);

	// create surf
	NewSurface *surf = AddNewSurface(640, 480, 32, buffer);

	// Main loop
	while (!Rex::KeyTest(REX_SC_ESCAPE))
	{
		Rex::PlaceBuffer(surf->buffer, surf->width * surf->height * (surf->bpp / 8));

		if (false)
		{
			// Clear back buffer
			Rex::SurfaceClear(&pic_bbuffer, 0);

			// Render HTML
			HTML_Render(&pic_bbuffer, &pic_font, document, 0, 0);

			// Flip the rendering buffers
			Rex::SurfaceToFrontBuffer(&pic_bbuffer);
		}
	}

	FreeNewSurface(surf);

	// goodbye
	Shutdown();

	// Exit gracefully
	return EXIT_SUCCESS;
}
