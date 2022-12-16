// ========================================================
//
// FILE:			/source/rex/modules/core/io/graphics.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Graphics implementation
//
// LAST EDITED:		December 16th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Globals (ew)
//
//

Rex::VidInfo vid_info;

//
//
// Rex namespace: Graphics
//
//

namespace Rex
{

//
//
// Globals
//
//

rex_uint8 colormap[64 * 256];

//
//
// Functions
//
//

//
// Bootstrap
//

// Initialize graphics
bool InitializeGraphics(rex_int32 w, rex_int32 h, rex_int32 bpp)
{
	#if (REX_TARGET == PLATFORM_DOS)

		#if (MODULE_VESA) && (MODULE_VGA)

			// If VESA fails, try VGA
			if (VESA::Initialize(w, h, bpp) == true)
			{
				vid_info = GetVidInfo();
				return true;
			}
			else
			{
				if (VGA::Initialize(w, h, bpp) == true)
				{
					vid_info = GetVidInfo();
					return true;
				}
				else
				{
					return false;
				}
			}

		#elif (MODULE_VGA)

			if (VGA::Initialize(w, h, bpp) == true)
			{
				vid_info = GetVidInfo();
				return true;
			}
			else
			{
				return false;
			}

		#endif

	#else

	return false;

	#endif
}

// Shutdown graphics
bool ShutdownGraphics()
{
	#if (REX_TARGET == PLATFORM_DOS)

		#if (MODULE_VESA)

			VESA::Shutdown();

		#elif (MODULE_VGA)

			VGA::Shutdown();

		#endif

	#endif

	return true;
}

// Set the 256-color palette for 8-bit indexed color rendering
bool SetGraphicsPalette(string filename)
{
	#if (REX_TARGET == PLATFORM_DOS)

		#if (MODULE_VESA)

			VESA::SetPalette(filename);
			return true;

		#elif (MODULE_VGA)

			VGA::SetPalette(filename);
			return true;

		#endif

	#else

	return false;

	#endif
}

// Return a VidInfo object with details about the current video state
VidInfo GetVidInfo()
{
	VidInfo v;

	#if (REX_TARGET == PLATFORM_DOS) && (MODULE_VESA)

	VESA::GetVidInfo(&v.width, &v.height, &v.bpp, &v.bytes_per_row);

	#endif

	return v;
}

//
// Colormap
//

// Load a colormap into memory
void ColormapLoad(rex_string filename)
{
	FILE *file = fopen(filename.c_str(), "rb");

	fread(&colormap, sizeof(rex_uint8), 64 * 256, file);

	fclose(file);
}

// Save the current colormap to a file
void ColormapSave(string filename)
{
	FILE *file = fopen(filename.c_str(), "wb");

	fwrite(&colormap, sizeof(rex_uint8), 64 * 256, file);

	fclose(file);
}

// Lookup a value with a given light level
rex_color ColormapLookup(rex_color color, int light)
{
	return (rex_color)colormap[((CLAMP(light, -32, 31) + 32) * 256) + CLAMP(color, 0, 255)];
}

// https://quakewiki.org/wiki/Quake_palette#Generating_the_colormap
// sample a 24-bit RGB value to one of the colours on the existing 8-bit palette
rex_uint8 RGB24_To_Index8(rex_uint8 palette[768], rex_color col)
{
	rex_int32 i, j;
	rex_int32 best_index = -1;
	rex_int32 best_dist = 0;

	rex_int32 rgb[3];

	rgb[0] = REX_COLOR_RED(col);
	rgb[1] = REX_COLOR_GRN(col);
	rgb[2] = REX_COLOR_BLU(col);

	for (i = 0; i < 256; i++)
	{
		rex_int32 dist = 0;

		for (j = 0; j < 3; j++)
		{
			/* note that we could use RGB luminosity bias for greater accuracy, but quake's colormap apparently didn't do this */
			rex_int32 d = ABS(rgb[j] - palette[i * 3 + j]);

			dist += d * d;
		}

		if (best_index == -1 || dist < best_dist)
		{
			best_index = i;
			best_dist = dist;
		}
	}

	return (rex_uint8)best_index;
}

// Generate a colormap from a given palette and load it into memory
void ColormapGenerate(rex_string palette_filename, rex_int32 num_fullbrights)
{
	//int num_fullbrights = 32; /* the last 32 colours will be full bright */
	rex_int32 x, y, i;

	rex_uint8 palette[768];

	FILE *file = fopen(palette_filename.c_str(), "rb");

	fread(&palette, sizeof(rex_uint8), 768, file);

	fclose(file);

	for (x = 0; x < 256; x++)
	{
		for (y = 0; y < 64; y++)
		{
			if (x < 256 - num_fullbrights)
			{
				rex_int32 rgb[3];
				rex_color col;

				for (i = 0; i < 3; i++)
				{
					rgb[i] = (palette[x * 3 + i] * (63 - y) + 16) >> 5; /* divide by 32, rounding to nearest integer */
					if (rgb[i] > 255) rgb[i] = 255;
				}

				col = REX_COLOR_RGB(rgb[0], rgb[1], rgb[2]);

				colormap[y * 256 + x] = RGB24_To_Index8(palette, col);
			}
			else
			{
				/* this colour is a fullbright, just keep the original colour */
				colormap[y * 256 + x] = x;
			}
		}
	}
}

} // namespace Rex
