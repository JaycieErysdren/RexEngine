// ========================================================
//
// FILE:			/source/viddrv/vesa.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		VESA namespace implementation
//
// LAST EDITED:		December 7th, 2022
//
// ========================================================

// Rex3D header
#include "rex3d.hpp"

// Ref:
// http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html.en

//
//
// VESA namespace
//
//

// VESA namespace definition (private)
namespace VESA
{

//
//
// Types
//
//

typedef struct
{ 
	uint8_t VESASignature[4]		ATTR_PACKED;
	uint16_t VESAVersion			ATTR_PACKED;
	uint32_t OEMStringPtr			ATTR_PACKED;
	uint8_t Capabilities[4]			ATTR_PACKED;
	uint32_t VideoModePtr			ATTR_PACKED;
	uint16_t TotalMemory			ATTR_PACKED;
	uint16_t OemSoftwareRev			ATTR_PACKED;
	uint32_t OemVendorNamePtr		ATTR_PACKED;
	uint32_t OemProductNamePtr		ATTR_PACKED;
	uint32_t OemProductRevPtr		ATTR_PACKED;
	uint8_t Reserved[222]			ATTR_PACKED;
	uint8_t OemData[256]			ATTR_PACKED;
} vesa_info_t;

typedef struct
{
	uint16_t ModeAttributes			ATTR_PACKED;
	uint8_t WinAAttributes			ATTR_PACKED;
	uint8_t WinBAttributes			ATTR_PACKED;
	uint16_t WinGranularity			ATTR_PACKED;
	uint16_t WinSize				ATTR_PACKED;
	uint16_t WinASegment			ATTR_PACKED;
	uint16_t WinBSegment			ATTR_PACKED;
	uint32_t WinFuncPtr				ATTR_PACKED;
	uint16_t BytesPerScanLine		ATTR_PACKED;
	uint16_t XResolution			ATTR_PACKED;
	uint16_t YResolution			ATTR_PACKED;
	uint8_t XCharSize				ATTR_PACKED;
	uint8_t YCharSize				ATTR_PACKED;
	uint8_t NumberOfPlanes			ATTR_PACKED;
	uint8_t BitsPerPixel			ATTR_PACKED;
	uint8_t NumberOfBanks			ATTR_PACKED;
	uint8_t MemoryModel				ATTR_PACKED;
	uint8_t BankSize				ATTR_PACKED;
	uint8_t NumberOfImagePages		ATTR_PACKED;
	uint8_t Reserved_page			ATTR_PACKED;
	uint8_t RedMaskSize				ATTR_PACKED;
	uint8_t RedMaskPos				ATTR_PACKED;
	uint8_t GreenMaskSize			ATTR_PACKED;
	uint8_t GreenMaskPos			ATTR_PACKED;
	uint8_t BlueMaskSize			ATTR_PACKED;
	uint8_t BlueMaskPos				ATTR_PACKED;
	uint8_t ReservedMaskSize		ATTR_PACKED;
	uint8_t ReservedMaskPos			ATTR_PACKED;
	uint8_t DirectColorModeInfo		ATTR_PACKED;
	uint32_t PhysBasePtr			ATTR_PACKED;
	uint32_t OffScreenMemOffset		ATTR_PACKED;
	uint16_t OffScreenMemSize		ATTR_PACKED;
	uint8_t Reserved[206]			ATTR_PACKED;
} vesa_mode_info_t;

//
//
// Global variables
//
//

//
// Information blocks
//

vesa_info_t vesa_info;
vesa_mode_info_t vesa_mode_info;

//
//
// Functions
//
//

//
// Getting VESA info
//

// Get VESA driver info
bool GetInfo()
{
	__dpmi_regs r;
	long dosbuf;
	int c;

	/* use the conventional memory transfer buffer */
	dosbuf = __tb & 0xFFFFF;

	/* initialize the buffer to zero */
	for (c = 0; c < sizeof(vesa_info_t); c++)
	_farpokeb(_dos_ds, dosbuf+c, 0);

	dosmemput("VBE2", 4, dosbuf);

	/* call the VESA function */
	r.x.ax = 0x4F00;
	r.x.di = dosbuf & 0xF;
	r.x.es = (dosbuf>>4) & 0xFFFF;
	__dpmi_int(0x10, &r);

	/* quit if there was an error */
	if (r.h.ah)
		return false;

	/* copy the resulting data into our structure */
	dosmemget(dosbuf, sizeof(vesa_info_t), &vesa_info);

	/* check that we got the right magic marker value */
	if (strncmp((char *)vesa_info.VESASignature, "VESA", 4) != 0)
		return false;

	/* it worked! */
	return true;
}

// Get info about a specific VESA mode
bool GetModeInfo(int mode)
{
	__dpmi_regs r;
	long dosbuf;
	int c;

	/* use the conventional memory transfer buffer */
	dosbuf = __tb & 0xFFFFF;

	/* initialize the buffer to zero */
	for (c = 0; c < sizeof(vesa_mode_info_t); c++)
	{
		_farpokeb(_dos_ds, dosbuf + c, 0);
	}

	/* call the VESA function */
	r.x.ax = 0x4F01;
	r.x.di = dosbuf & 0xF;
	r.x.es = (dosbuf>>4) & 0xFFFF;
	r.x.cx = mode;
	__dpmi_int(0x10, &r);

	/* quit if there was an error */
	if (r.h.ah)
		return false;

	/* copy the resulting data into our structure */
	dosmemget(dosbuf, sizeof(vesa_mode_info_t), &vesa_mode_info);

	/* it worked! */
	return true;
}

// Find a specified VESA mode number
int FindMode(int w, int h, int bpp)
{
	int mode_list[256];
	int number_of_modes;
	long mode_ptr;
	int c;

	/* check that the VESA driver exists, and get information about it */
	if (GetInfo() == false)
		return 0;

	/* convert the mode list pointer from seg:offset to a linear address */
	mode_ptr = ((vesa_info.VideoModePtr & 0xFFFF0000) >> 12) + (vesa_info.VideoModePtr & 0xFFFF);

	number_of_modes = 0;

	/* read the list of available modes */
	while (_farpeekw(_dos_ds, mode_ptr) != 0xFFFF)
	{
		mode_list[number_of_modes] = _farpeekw(_dos_ds, mode_ptr);
		number_of_modes++;
		mode_ptr += 2;
	}

	/* scan through the list of modes looking for the one that we want */
	for (c = 0; c < number_of_modes; c++)
	{
		/* get information about this mode */
		if (GetModeInfo(mode_list[c]) == false)
			continue;

		/* check the flags field to make sure this is a color graphics mode,
		* and that it is supported by the current hardware */
		if ((vesa_mode_info.ModeAttributes & 0x19) != 0x19)
			continue;

		/* check that this mode is the right size */
		if ((vesa_mode_info.XResolution != w) || (vesa_mode_info.YResolution != h))
			continue;

		/* check that there is only one color plane */
		if (vesa_mode_info.NumberOfPlanes != 1)
			continue;

		/* check that it is a packed-pixel mode (other values are used for
		* different memory layouts, eg. 6 for a truecolor resolution) */
		if (vesa_mode_info.MemoryModel != 4)
			continue;

		/* check that this is an 8-bit (256 color) mode */
		if (vesa_mode_info.BitsPerPixel != bpp)
			continue;

		/* if it passed all those checks, this must be the mode we want! */
		return mode_list[c];
	}

	/* oh dear, there was no mode matching the one we wanted! */
	return 0; 
}

//
// Setting VESA attributes
//

// Set the vesa mode from a set of variables
bool SetMode(int w, int h, int bpp)
{
	__dpmi_regs r;
	int mode_number;

	/* find the number for this mode */
	mode_number = FindMode(w, h, bpp);

	if (!mode_number)
		return false;

	/* call the VESA mode set function */
	r.x.ax = 0x4F02;
	r.x.bx = mode_number;

	__dpmi_int(0x10, &r);

	if (r.h.ah)
		return false;

	/* it worked! */
	return true;
}

// Set the bank that the pixel functions are currently writing to
void SetBank(int bank_number)
{
	__dpmi_regs r;

	r.x.ax = 0x4F05;
	r.x.bx = 0;
	r.x.dx = bank_number;

	__dpmi_int(0x10, &r);
}

// Place a pixel on the screen (SLOW!!)
void PutPixel(int x, int y, int color)
{
	int address = y * 640 + x;
	int bank_size = vesa_mode_info.WinGranularity * 1024;
	int bank_number = address / bank_size;
	int bank_offset = address % bank_size;

	SetBank(bank_number);

	_farpokeb(_dos_ds, 0xA0000 + bank_offset, color);
}

// Place a pixel buffer into the video memory
void PlaceBuffer(int8_t *buffer, int buffer_size)
{
	int bank_size = vesa_mode_info.WinSize * 1024;
	int bank_granularity = vesa_mode_info.WinGranularity * 1024;
	int bank_number = 0;
	int todo = buffer_size;
	int copy_size = 0;
	int buffer_pos = 0;

	void *fuck = buffer;

	while (todo > 0)
	{
		/* select the appropriate bank */
		SetBank(bank_number);

		/* how much can we copy in one go? */
		if (todo > bank_size)
			copy_size = bank_size;
		else
			copy_size = todo;

		/* copy a bank of data to the screen */
		dosmemput(buffer, copy_size, 0xA0000);

		/* move on to the next bank of data */
		todo -= copy_size;
		buffer += copy_size;
		bank_number += bank_size / bank_granularity;
	}
}

//
// Bootstrap
//

// Initialize the VESA driver with the given width, height and bpp
bool Initialize(int w, int h, int bpp)
{
	return SetMode(w, h, bpp);
}

void Shutdown()
{
	// Set mode 0x03 (text mode)
	__dpmi_regs r;
	r.x.ax = 0x03;
	__dpmi_int(0x10, &r);
}

//
// Helpers
//

VidInfo GetVidInfo()
{
	VidInfo v;

	v.width = vesa_mode_info.XResolution;
	v.height = vesa_mode_info.YResolution;
	v.bpp = vesa_mode_info.BitsPerPixel;
	v.bytes_per_row = vesa_mode_info.BitsPerPixel * vesa_mode_info.XResolution;

	return v;
}

// Set the palette from a file
void SetPalette(string filename)
{
	int i;
	FILE *file;

	file = fopen(filename.c_str(), "rb");

	// Tell VGA that palette data is coming
	outp(0x3c8, 0);

	// Install the palette
	for (i = 0; i < 256; i++)
	{
		unsigned char r = getc(file);
		unsigned char g = getc(file);
		unsigned char b = getc(file);

		// stupid
		outp(0x3c9, (r * 63) / 255);
		outp(0x3c9, (g * 63) / 255);
		outp(0x3c9, (b * 63) / 255);
	}

	fclose(file);
}

} // namespace VESA
