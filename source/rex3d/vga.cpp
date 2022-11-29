// ========================================================
//
// FILE:			/source/vga.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		VGA namespace implementation
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

// Rex3D header
#include "rex3d.hpp"

//
//
// VGA namespace
//
//

// VGA namespace definition (private)
namespace VGA
{
	//
	//
	// Variables
	//
	//

	//
	// Buffers
	//

	// Back buffer
	uint8_t *buffer_back;

	// Front buffer (pointer to video memory)
	uint8_t *buffer_front;
}

//
// Bootstrap
//

// Initialize
bool VGA::Initialize()
{
	// Set VGA mode 0x13
	union REGS regs;
	regs.x.ax = 0x13;
	int86(0x10, &regs, &regs);

	// Allocate & clear back buffer
	buffer_back = new uint8_t [64000];
	memset(buffer_back, 0, 64000);

	// Enable access to the memory we use
	__djgpp_nearptr_enable();

	// Set & clear video memory
	buffer_front = (uint8_t *)(0xa0000 + __djgpp_conventional_base);
	memset(buffer_front, 0, 64000);

	return true;
}

// Shutdown
bool VGA::Shutdown()
{
	// Set mode 0x03
	union REGS regs;
	regs.x.ax = 0x03;
	int86(0x10, &regs, &regs);

	// Ee-enable memory checking
	__djgpp_nearptr_disable();

	// Free memory
	delete [] (buffer_back);

	return true;
}

//
// Palette
//

// set a colour in the palette
void VGA::SetPaletteColor(uint8_t i, uint8_t r, uint8_t g, uint8_t b)
{
	// Output the index of the colour
	outp(0x3c8, i);

	// And its RGB components
	outp(0x3c9, r);
	outp(0x3c9, g);
	outp(0x3c9, b);
}

//
// Pixel
//

// Place a pixel in the back buffer
void VGA::PlacePixel(int x, int y, uint8_t color)
{
	// set the memory
	// same as y*320+x, but slightly quicker
	buffer_back[(y << 8) + (y << 6) + x] = color;
}

//
// Rendering
//

// Clear the back buffer
void VGA::Clear()
{
	memset(buffer_back, 0, 64000);
}

// Copy the back buffer to the front buffer
void VGA::Flip()
{
	// Wait for vertical retrace
	while (inportb(0x3da) & 0x08);
	while (!(inportb(0x3da) & 0x08));

	// Copy the back buffer to the front buffer
	memcpy(buffer_front, buffer_back, 64000);
}
