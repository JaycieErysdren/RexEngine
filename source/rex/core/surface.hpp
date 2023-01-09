// ========================================================
//
// FILE:			/source/rex/core/log.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Surface header
//
// LAST EDITED:		January 8th, 2023
//
// ========================================================

#ifndef __REX_SURFACE_H__
#define __REX_SURFACE_H__

//
//
// Rex namespace: Surface
//
//

namespace Rex
{

//
//
// Functions
//
//

class Surface
{
	public:
		// Constructors
		Surface();
		Surface(rex_uint width, rex_uint height, rex_uint bpp);
		Surface(rex_uint width, rex_uint height, rex_uint bpp, void *pixels);
		Surface(const char *filename);

		// Destructor
		~Surface();

		// Create blank surface
		bool Create(rex_uint width, rex_uint height, rex_uint bpp);

		// Create surface from pixel buffer
		bool Create(rex_uint width, rex_uint height, rex_uint bpp, void *pixels);

		// Load image into Surface
		bool Load(const char *filename);

		// Load image into Surface, with desired BPP
		bool Load(const char *filename, rex_int desired_bpp);

		// Save Surface to BMP file
		bool SaveBMP(const char *filename);

		// Dump raw pixel buffer
		bool SaveRaw(const char *filename);

		// Clear surface
		bool Clear(rex_color color);

		// Return read-only info
		rex_int Width();
		rex_int Height();
		rex_int BPP();
		rex_int BytesPerRow();

	private:
		// read-only variables
		rex_int width;
		rex_int height;
		rex_int bpp;
		rex_int bytes_per_row;
		void *pixels;

		// the address where this surface is stored
		void *memory_address;
};

} // namespace Rex

#endif // __REX_SURFACE_H__
