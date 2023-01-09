// ========================================================
//
// FILE:			/source/rex/core/surface.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Surface implementation
//
// LAST EDITED:		January 8th, 2023
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

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

//
// Surface class
//

// Constructor with nothing provided
Surface::Surface()
{
	this->width = 0;
	this->height = 0;
	this->bpp = 0;
	this->bytes_per_row = 0;
	this->pixels = nullptr;
}

// Constructor with some details
Surface::Surface(rex_uint width, rex_uint height, rex_uint bpp)
{
	this->Create(width, height, bpp);
}

// Constructor with some details and a pixel buffer
// Pixel buffer must be created with malloc(), calloc() or realloc()
Surface::Surface(rex_uint width, rex_uint height, rex_uint bpp, void *pixels)
{
	this->Create(width, height, bpp, pixels);
}

// Constructor with image filename
Surface::Surface(const char *filename)
{
	this->Load(filename);
}

// Destructor
Surface::~Surface()
{
	if (this->pixels != NULL && this->pixels != nullptr)
		free(this->pixels);
}

// Create blank surface
bool Surface::Create(rex_uint width, rex_uint height, rex_uint bpp)
{
	// error checking
	if (width < 1 || height < 1)
	{
		Message("Surface::Surface", "Surface was created with a dimension less than 1", FAILURE);
		return false;
	}

	if (bpp < 8)
	{
		Message("Surface::Surface", "Surface was created with a bpp less than 8", FAILURE);
		return false;
	}

	// set basic info
	this->width = width;
	this->height = height;
	this->bpp = bpp;
	this->bytes_per_row = width * (bpp / 8);

	// allocate pixel buffer
	this->pixels = calloc(this->width * this->height, this->bpp / 8);

	// error checking
	if (this->pixels == nullptr || this->pixels == NULL)
	{
		Message("Surface::Surface", "Failed to allocate pixel buffer in Surface!", FAILURE);
		return false;
	}

	return true;
}

// Create surface from pixel buffer
bool Surface::Create(rex_uint width, rex_uint height, rex_uint bpp, void *pixels)
{
	// free pixels if it exists
	if (this->pixels != nullptr && this->pixels != NULL)
		free(this->pixels);

	// error checking
	if (width < 1 || height < 1)
	{
		Message("Surface::Surface", "Surface was created with a dimension less than 1", FAILURE);
		return false;
	}

	if (bpp < 8)
	{
		Message("Surface::Surface", "Surface was created with a bpp less than 8", FAILURE);
		return false;
	}

	// set basic info
	this->width = width;
	this->height = height;
	this->bpp = bpp;
	this->bytes_per_row = width * (bpp / 8);

	// set pixel buffer
	if (pixels != NULL && pixels != nullptr)
	{
		this->pixels = pixels;
		return true;
	}
	else
	{
		Message("Surface::Surface", "Surface constructor was called with pixel buffer, but pixel buffer was null", FAILURE);
		return false;
	}
}

// Load image into Surface, with desired BPP
bool Surface::Load(const char *filename, rex_int desired_bpp)
{
	// free pixels if it exists
	if (this->pixels != nullptr && this->pixels != NULL)
		free(this->pixels);

	// load image with stb_image
	unsigned char *data = stbi_load(filename, &this->width, &this->height, &this->bpp, desired_bpp / 8);

	// error checking
	if (data == NULL || data == nullptr)
	{
		Message("Surface::Surface", "Failed to load or parse image from Surface", FAILURE);
		return false;
	}

	// set info
	this->bpp *= 8;
	this->bytes_per_row = this->width * (this->bpp / 8);

	// allocate pixel buffer
	this->pixels = calloc(this->width * this->height, this->bpp / 8);

	// copy in pixel buffer
	memcpy(this->pixels, data, this->height * this->width * (this->bpp / 8));

	// error checking
	if (this->pixels == nullptr || this->pixels == NULL)
	{
		Message("Surface::Surface", "Failed to allocate pixel buffer in Surface!", FAILURE);
		return false;
	}

	// free image data
	stbi_image_free(data);

	return true;
}

// Load image into Surface
bool Surface::Load(const char *filename)
{
	return this->Load(filename, 0);
}

// Save Surface to BMP file
bool Surface::SaveBMP(const char *filename)
{
	if (stbi_write_bmp(filename, this->width, this->height, this->bpp / 8, this->pixels) == 0)
		return true;
	else
		return false;
}

// Dump raw pixel buffer
bool Surface::SaveRaw(const char *filename)
{
	FILE *file = fopen(filename, "wb");

	if (file == NULL)
		return false;

	if (fwrite(this->pixels, this->bytes_per_row, this->height, file) != this->bytes_per_row * this->height)
		return false;

	fclose(file);
	
	return true;
}

// Return width
rex_int Surface::Width()
{
	return this->width;
}

// Return height
rex_int Surface::Height()
{
	return this->height;
}

// Return BPP
rex_int Surface::BPP()
{
	return this->bpp;
}

// Return bytes per row
rex_int Surface::BytesPerRow()
{
	return this->bytes_per_row;
}

} // namespace Rex
