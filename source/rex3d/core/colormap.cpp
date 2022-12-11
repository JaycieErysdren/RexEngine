// ========================================================
//
// FILE:			/source/core/colormap.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Colormap namespace implementation
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

// Colormap namespace (private)
namespace Colormap
{

//
//
// Globals
//
//

uint8_t colormap[64 * 256];

//
//
// Functions
//
//

// Load a colormap into memory
void Load(string filename)
{
	FILE *file = fopen(filename.c_str(), "rb");

	fread(&colormap, sizeof(uint8_t), 64 * 256, file);

	fclose(file);
}

// Save the current colormap to a file
void Save(string filename)
{
	FILE *file = fopen(filename.c_str(), "wb");

	fwrite(&colormap, sizeof(uint8_t), 64 * 256, file);

	fclose(file);
}

// Lookup a value with a given light level
uint8_t Lookup(uint8_t index, int light)
{
	return colormap[((CLAMP(light, -32, 31) + 32) * 256) + index];
}

// https://quakewiki.org/wiki/Quake_palette#Generating_the_colormap

/* sample a 24-bit RGB value to one of the colours on the existing 8-bit palette */
uint8_t RGB24_To_Index8(const uint8_t palette[768], const int rgb[3])
{
	int i, j;
	int best_index = -1;
	int best_dist = 0;

	for (i = 0; i < 256; i++)
	{
		int dist = 0;

		for (j = 0; j < 3; j++)
		{
			/* note that we could use RGB luminosity bias for greater accuracy, but quake's colormap apparently didn't do this */
			int d = ABS(rgb[j] - palette[i * 3 + j]);
			dist += d * d;
		}

		if (best_index == -1 || dist < best_dist)
		{
			best_index = i;
			best_dist = dist;
		}
	}

	return (uint8_t)best_index;
}

// Generate a colormap from a given palette and load it into memory
void Generate(string palette_filename, int num_fullbrights)
{
	//int num_fullbrights = 32; /* the last 32 colours will be full bright */
	int x, y, i;

	uint8_t palette[768];

	FILE *file = fopen(palette_filename.c_str(), "rb");

	fread(&palette, sizeof(uint8_t), 768, file);

	fclose(file);

	for (x = 0; x < 256; x++)
	{
		for (y = 0; y < 64; y++)
		{
			if (x < 256 - num_fullbrights)
			{
				int rgb[3];

				for (i = 0; i < 3; i++)
				{
					rgb[i] = (palette[x * 3 + i] * (63 - y) + 16) >> 5; /* divide by 32, rounding to nearest integer */
					if (rgb[i] > 255) rgb[i] = 255;
				}

				colormap[y * 256 + x] = RGB24_To_Index8(palette, rgb);
			}
			else
			{
				/* this colour is a fullbright, just keep the original colour */
				colormap[y * 256 + x] = x;
			}
		}
	}
}

} // namespace Colormap
