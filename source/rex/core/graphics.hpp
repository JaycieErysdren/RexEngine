// ========================================================
//
// FILE:			/source/rex/modules/core/io/graphics.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Graphics header
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

#ifndef __REX_GRAPHICS_H__
#define __REX_GRAPHICS_H__

//
//
// Rex namespace: Graphics
//
//

namespace Rex
{

//
//
// Classes
//
//

class VidInfo
{
	public:
		int width;
		int height;
		int bpp;
		int bytes_per_row;
};

//
//
// Function prototypes
//
//

//
// Bootstrap
//

// Initialize graphics
bool InitializeGraphics(int w, int h, int bpp);

// Shutdown graphics
bool ShutdownGraphics();

// Set the 256-color palette for 8-bit indexed color rendering
bool SetGraphicsPalette(string filename);

// Return a VidInfo object with details about the current video state
VidInfo GetVidInfo();

//
// Colormap
//

uint8_t ColormapLookup(uint8_t index, int light);
void ColormapGenerate(string palette_filename, int num_fullbrights);
void ColormapSave(string filename);
void ColormapLoad(string filename);

} // namespace Rex

#endif // __REX_GRAPHICS_H__
