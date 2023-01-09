// ========================================================
//
// FILE:			/source/testapp/testapp.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		TestApp entry point
//
// LAST EDITED:		January 5th, 2023
//
// ========================================================

// Header
#include "testapp.hpp"

rex_int width = 640;
rex_int height = 480;
rex_int bpp = 32;

#ifdef USB_TEST

// LibUSB
#include <libusb-1.0/libusb.h>

static void USB_PrintDevice(libusb_device *dev, libusb_device_handle *handle)
{
	struct libusb_device_descriptor desc;
	unsigned char string[256];
	const char *speed;
	int ret;
	uint8_t i;

	switch (libusb_get_device_speed(dev))
	{
		case LIBUSB_SPEED_LOW:
			speed = "1.5M";
			break;

		case LIBUSB_SPEED_FULL:
			speed = "12M";
			break;

		case LIBUSB_SPEED_HIGH:
			speed = "480M";
			break;

		case LIBUSB_SPEED_SUPER:
			speed = "5G";
			break;

		case LIBUSB_SPEED_SUPER_PLUS:
			speed = "10G";
			break;

		default:
			speed = "Unknown";
	}

	ret = libusb_get_device_descriptor(dev, &desc);

	if (ret < 0)
	{
		fprintf(stderr, "failed to get device descriptor");
		return;
	}

	printf("Dev (bus %u, device %u): %04X - %04X speed: %s\n",
	       libusb_get_bus_number(dev), libusb_get_device_address(dev),
	       desc.idVendor, desc.idProduct, speed);

	if (!handle)
		libusb_open(dev, &handle);

	if (handle)
	{
		if (desc.iManufacturer)
		{
			ret = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, string, sizeof(string));
			if (ret > 0)
				printf("  Manufacturer:              %s\n", (char *)string);
		}

		if (desc.iProduct)
		{
			ret = libusb_get_string_descriptor_ascii(handle, desc.iProduct, string, sizeof(string));
			if (ret > 0)
				printf("  Product:                   %s\n", (char *)string);
		}

		// test for gv-usb2
		if (memcmp(string, "I-O DATA GV-USB2", 16) == 0)
			printf("\nFound GV-USB2!\n\n");

		if (desc.iSerialNumber)
		{
			ret = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, string, sizeof(string));

			if (ret > 0)
				printf("  Serial Number:             %s\n", (char *)string);
		}
	}

	printf("\n");

	if (handle)
		libusb_close(handle);
}

static void USB_PrintDevices(libusb_device **devs)
{
	libusb_device *dev;
	rex_int i;

	while ((dev = devs[i++]) != NULL)
	{
		USB_PrintDevice(dev, NULL);
	}
}

rex_int USB_Test()
{
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);

	if (cnt < 0)
	{
		libusb_exit(NULL);
		return (rex_int)cnt;
	}

	USB_PrintDevices(devs);
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);
	return 0;
}

#endif

//
// Main entry point
//

int RexMain(int argc, char **argv)
{
	// Initialize Rex Engine
	Rex::Init();

	// Initialize Rex Engine Graphics
	Rex::Init_Graphics(width, height, bpp, "TestApp");

	// surface & color
	Rex::Color *color = new Rex::Color(255, 0, 255, 255);
	Rex::Surface *surf = new Rex::Surface(width, height, bpp);
	surf->Clear(color);

	// main loop
	while (Rex::DoMainLoop())
	{
		surf->Flip();

		if (Rex::KeyboardGet(REX_SC_ESCAPE) == true) break;
	}

	delete surf;
	delete color;

	// Quit Rex Engine Graphics
	Rex::Quit_Graphics();

	// Quit Rex Engine
	Rex::Quit();

	// Exit gracefully
	return EXIT_SUCCESS;
}
