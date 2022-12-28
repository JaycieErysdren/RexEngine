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
		rex_int32 width;
		rex_int32 height;
		rex_int32 bpp;
		rex_int32 bytes_per_row;
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
bool InitializeGraphics(rex_int32 w, rex_int32 h, rex_int32 bpp);

// Shutdown graphics
bool ShutdownGraphics();

// Set the 256-color palette for 8-bit indexed color rendering
bool SetGraphicsPalette(rex_string filename);

// Return a VidInfo object with details about the current video state
VidInfo GetVidInfo();

//
// Colormap
//

rex_uint8 ColormapLookup(rex_color color, int light);
void ColormapGenerate(rex_string palette_filename, rex_int32 num_fullbrights);
void ColormapSave(rex_string filename);
void ColormapLoad(rex_string filename);

} // namespace Rex

#endif // __REX_GRAPHICS_H__
