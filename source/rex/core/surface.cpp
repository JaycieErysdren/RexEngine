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
// Color class
//

//
// constructors and destructors
//

// blank constructor
Color::Color()
{
	this->tag = NONE;
	this->val.u8 = 0;
}

// 8-bit constructor
Color::Color(rex_uint8 c)
{
	this->tag = U8;
	this->val.u8 = c;
}

// 16-bit constructor
//Color::Color(rex_int rgb16[3])
//{
//	this->tag = U16;
//	this->val.u8 = 0;
//	this->val.u16 = ((((rex_uint8)rgb16[0] >> 3) & 0x1f) << 11) | ((((rex_uint8)rgb16[1] >> 2) & 0x3f) << 5) | ((rex_uint8)rgb16[2] >> 3) & 0x1f;
//	this->val.u32 = 0;
//}

// 16-bit constructor
Color::Color(rex_uint8 r, rex_uint8 g, rex_uint8 b)
{
	this->Color16(r, g, b);
}

// 32-bit constructor
Color::Color(rex_uint8 r, rex_uint8 g, rex_uint8 b, rex_uint8 a)
{
	this->Color32(r, g, b, a);
}

//
// color setting and retrival
//

// retrieve 8-bit color value
rex_uint8 Color::Color8()
{
	this->tag = U8;
	return this->val.u8;
}

// retrieve and set 8-bit color value
rex_uint8 Color::Color8(rex_uint8 c)
{
	this->val.u8 = c;
	this->tag = U8;
	return this->val.u8;
}

// retrieve 16-bit color value
rex_uint16 Color::Color16()
{
	this->tag = U16;
	return this->val.u16;
}

// retrieve and set 16-bit color value
rex_uint16 Color::Color16(rex_uint8 r, rex_uint8 g, rex_uint8 b)
{
	this->val.u16 = (rex_uint16)((((r >> 3) & 0x1f) << 11) | (((g >> 2) & 0x3f) << 5) | (b >> 3) & 0x1f);
	this->tag = U16;
	return this->val.u16;
}

// retrieve 32-bit color value
rex_uint32 Color::Color32()
{
	this->tag = U32;
	return this->val.u32;
}

// retrieve and set 32-bit color value
rex_uint32 Color::Color32(rex_uint8 r, rex_uint8 g, rex_uint8 b, rex_uint8 a)
{
	this->val.u32 = (rex_uint32)((a << 24) | (r << 16) | (g << 8) | b);
	this->tag = U32;
	return this->val.u32;
}

// retrieve red component
rex_uint8 Color::R()
{
	switch (this->tag)
	{
		case U16:
			return (rex_uint8)(((this->val.u16 & 0xF800) >> 11) << 3);

		case U32:
			return (rex_uint8)((this->val.u32 >> 16) & 0xFF);

		default:
			return 0;
	}
}

// retrieve and set red component
rex_uint8 Color::R(rex_uint8 r)
{
	switch (this->tag)
	{
		case U16:
			this->val.u16 |= (((r >> 3) & 0x1f) << 11);
			break;

		case U32:
			this->val.u32 |= (r << 16);
			break;

		default:
			break;
	}

	return this->R();
}

// retrieve green component
rex_uint8 Color::G()
{
	switch (this->tag)
	{
		case U16:
			return (rex_uint8)(((this->val.u16 & 0x7E0) >> 5) << 2);

		case U32:
			return (rex_uint8)((this->val.u32 >> 8) & 0xFF);

		default:
			return 0;
	}
}

// retrieve and set green component
rex_uint8 Color::G(rex_uint8 g)
{
	rex_uint8 ret = this->B();

	switch (this->tag)
	{
		case U16:
			this->val.u16 |= (((g >> 2) & 0x3f) << 5);
			break;

		case U32:
			this->val.u32 |= (g << 8);
			break;

		default:
			break;
	}

	return ret;
}

// retrieve blue component
rex_uint8 Color::B()
{
	switch (this->tag)
	{
		case U16:
			return (this->val.u16 & 0x1F) << 3;

		case U32:
			return (this->val.u32 & 0xFF);

		default:
			return 0;
	}
}

// retrieve and set blue component
rex_uint8 Color::B(rex_uint8 b)
{
	rex_uint8 ret = this->B();

	switch (this->tag)
	{
		case U16:
			this->val.u16 |= (b >> 3) & 0x1f;
			break;

		case U32:
			this->val.u32 |= b;
			break;

		default:
			break;
	}

	return ret;
}

// retrieve alpha component
rex_uint8 Color::A()
{
	switch (this->tag)
	{
		case U32:
			return (this->val.u32 >> 24) & 0xFF;

		default:
			return 255;
	}
}

// retrieve and set alpha component
rex_uint8 Color::A(rex_uint8 a)
{
	rex_uint8 ret = this->A();

	switch (this->tag)
	{
		case U32:
			this->val.u32 |= (a << 24);
			break;

		default:
			break;
	}

	return ret;
}

// retrieve color depth
rex_uint8 Color::Depth()
{
	switch (this->tag)
	{
		case U8:
			return 8;

		case U16:
			return 16;

		case U32:
			return 32;

		default:
			return 0;
	}
}

//
// operators
//

// 8-bit color
void Color::operator=(const rex_int index)
{
	this->Color8((rex_uint8)index);
}

// 16-bit color
//void Color::operator=(const rex_int rgba16[3])
//{
//	this->Color16((rex_uint8)rgba16[0], (rex_uint8)rgba16[1], (rex_uint8)rgba16[2]);
//}

// 32-bit color
void Color::operator=(const rex_int rgba32[4])
{
	this->Color32((rex_uint8)rgba32[0], (rex_uint8)rgba32[1], (rex_uint8)rgba32[2], (rex_uint8)rgba32[3]);
}

//
// Surface class
//

//
// Constructors/Destructors
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
	this->Destroy();
}

//
// Create/Destroy
//

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

bool Surface::Destroy()
{
	// free pixels if it exists
	if (this->pixels != nullptr && this->pixels != NULL)
		free(this->pixels);

	// reset all values
	this->width = 0;
	this->height = 0;
	this->bpp = 0;
	this->bytes_per_row = 0;
	this->pixels = nullptr;

	return true;
}

//
// Load/Save
//

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

// Save Surface to file
bool Surface::Save(const char *filename)
{
	return this->Save(filename, BMP);
}

// Save Surface to file
bool Surface::Save(const char *filename, surface_save_format format)
{
	switch (format)
	{
		// raw image data
		case RAW:
		{
			FILE *file = fopen(filename, "wb");

			if (file == NULL)
				return false;

			if (fwrite(this->pixels, this->bytes_per_row, this->height, file) != this->bytes_per_row * this->height)
				return false;

			fclose(file);

			return true;
		}

		// png
		case PNG:
		{
			if (stbi_write_png(filename, this->width, this->height, this->bpp / 8, this->pixels, this->bytes_per_row) == 0)
				return true;
			else
				return false;
		}

		// bitmap
		case BMP:
		{
			if (stbi_write_bmp(filename, this->width, this->height, this->bpp / 8, this->pixels) == 0)
				return true;
			else
				return false;
		}

		// targa
		case TGA:
		{
			if (stbi_write_tga(filename, this->width, this->height, this->bpp / 8, this->pixels) == 0)
				return true;
			else
				return false;
		}

		// jpeg
		case JPG:
		{
			if (stbi_write_jpg(filename, this->width, this->height, this->bpp / 8, this->pixels, 100) == 0)
				return true;
			else
				return false;
		}

		// fail
		default:
			return false;
	}

	return false;
}

//
// Clearing functions
//

// Clear surface
bool Surface::Clear()
{
	memset(this->pixels, this->bytes_per_row * this->height, 0);
	return true;
}

// Clear surface with specified color
bool Surface::Clear(Color *color)
{
	if (color == NULL || color == nullptr)
		return false;

	if (color->Depth() != this->bpp)
		return false;

	switch(this->bpp)
	{
		case 8:
			MemSet8(this->pixels, color->Color8(), this->width * this->height);
			return true;

		case 16:
			MemSet16(this->pixels, color->Color16(), this->width * this->height);
			return true;

		case 32:
			MemSet32(this->pixels, color->Color32(), this->width * this->height);
			return true;

		default:
			return false;
	}
}

//
// Draw functions
//

// Plot a single pixel
bool Surface::DrawPixel(rex_int x, rex_int y, Color &color)
{
	if (this->pixels == nullptr || this->pixels == NULL)
		return false;

	switch(this->bpp)
	{
		case 8:
			((rex_uint8 *)this->pixels)[(this->width * y) + x] = color.Color8();
			return true;

		case 16:
			((rex_uint16 *)this->pixels)[(this->width * y) + x] = color.Color16();
			return true;

		case 32:
			((rex_uint32 *)this->pixels)[(this->width * y) + x] = color.Color32();
			return true;

		default:
			return false;
	}
}

//
// Return read-only info
//

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
