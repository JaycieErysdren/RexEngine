// ========================================================
//
// FILE:			/source/rex/platform/dos.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine: DOS Platform implementation
//
// LAST EDITED:		December 31st, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex Engine: DOS Platform
//
//

namespace Rex
{

//
//
// VESA types
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

typedef struct
{
	vesa_info_t *info;
	vesa_mode_info_t *mode_info;
} vesa_t;

//
//
// Global variables
//
//

//
// DOS
//

// Regs
union REGS regs;
vesa_t *vesa;

//
// Mouse
//

rex_int16 mouse_x, mouse_y, mouse_b;

//
// Keyboard
//

#if (REX_COMPILER == COMPILER_DJGPP)
	_go32_dpmi_seginfo Keyboard_Handler_Old;
	_go32_dpmi_seginfo Keyboard_Handler_New;
#endif

char keys[256];
char key_last_pressed;

//
//
// Functions
//
//

//
// DOS
//

// Called by DOS whenever a key is pressed
void Keyboard_Handler()
{
	char key = (char)inp(0x60);
	keys[key_last_pressed = key & 0x7F] = !(key & 0x80);
	outp(0x20, 0x20);
}

//
// Bootstrap
//

// Initialize all interrupts
bool Platform_Init()
{
	//
	// Memory access
	//

	#if (REX_COMPILER == COMPILER_DJGPP)
		__djgpp_nearptr_enable();
	#endif

	//
	// Mouse
	//

	// Initialize mouse driver
	regs.w.ax = 0x00;
	int386(0x33, &regs, &regs);

	// Hide DOS mouse cursor
	regs.w.ax = 0x02;
	int386(0x33, &regs, &regs);

	//
	// Keyboard
	//

	memset(keys, 0, sizeof(keys));

	#if (REX_COMPILER == COMPILER_DJGPP)
		_go32_dpmi_get_protected_mode_interrupt_vector(9, &Keyboard_Handler_Old);
		Keyboard_Handler_New.pm_offset = (int)Keyboard_Handler;
		Keyboard_Handler_New.pm_selector = _go32_my_cs();
		_go32_dpmi_allocate_iret_wrapper(&Keyboard_Handler_New);
		_go32_dpmi_set_protected_mode_interrupt_vector(9, &Keyboard_Handler_New);
	#endif

	//
	// Timer
	//

	#if (REX_COMPILER == COMPILER_DJGPP)
		uclock();
	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)
		clock();
	#endif

	return true;
}

// Free all interrupts
bool Platform_Quit()
{
	//
	// Keyboard
	//

	#if (REX_COMPILER == COMPILER_DJGPP)
		_go32_dpmi_set_protected_mode_interrupt_vector(9, &Keyboard_Handler_Old);
		_go32_dpmi_free_iret_wrapper(&Keyboard_Handler_New);
	#endif

	//
	// Memory access
	//

	#if (REX_COMPILER == COMPILER_DJGPP)
		__djgpp_nearptr_disable();
	#endif

	return true;
}

//
// Devices
//

// Read data from all currently connected input devices (keyboards, mice, etc)
bool Platform_ReadInputDevices()
{
	//
	// Mouse
	//

	regs.w.ax = 0x03;
	int386(0x33, &regs, &regs);
	mouse_b = regs.w.bx;
	mouse_x = regs.w.cx;
	mouse_y = regs.w.dx;

	return true;
}

// Get mouse data
bool Platform_MouseGet(rex_int *mx, rex_int *my, rex_int *mb)
{
	if (mx) *mx = (rex_int)mouse_x;
	if (my) *my = (rex_int)mouse_y;
	if (mb) *mb = (rex_int)mouse_b;

	return true;
}

// Set mouse data
bool Platform_MouseSet(rex_int mx, rex_int my, rex_int mb)
{
	regs.w.ax = 0x04;
	regs.w.bx = (rex_int16)mb;
	regs.w.cx = (rex_int16)mx;
	regs.w.dx = (rex_int16)my;
	int386(0x33, &regs, &regs);

	return true;
}

// Returns true if the specified key is pressed
bool Platform_KeyboardGet(rex_int scancode)
{
	return keys[scancode] > 0 ? true : false;
}

//
// Timer
//

// Returns the number of ticks elapsed since program start
rex_int64 Platform_GetTicks64()
{
	#if (REX_COMPILER == COMPILER_DJGPP)
		return (rex_int64)uclock();
	#endif

	#if (REX_COMPILER == COMPILER_WATCOM)
		return (rex_int64)clock();
	#endif
}

//
// Graphics
//

// Get VESA driver info
bool VESA_GetInfo()
{
	__dpmi_regs r;
	long dosbuf;
	int c;

	// use the conventional memory transfer buffer
	dosbuf = __tb & 0xFFFFF;

	// initialize the buffer to zero
	for (c = 0; c < sizeof(vesa_info_t); c++)
		_farpokeb(_dos_ds, dosbuf+c, 0);

	dosmemput("VBE2", 4, dosbuf);

	// call the VESA function
	r.x.ax = 0x4F00;
	r.x.di = dosbuf & 0xF;
	r.x.es = (dosbuf>>4) & 0xFFFF;
	__dpmi_int(0x10, &r);

	// quit if there was an error
	if (r.h.ah)
		return false;

	// copy the resulting data into our structure
	dosmemget(dosbuf, sizeof(vesa_info_t), vesa->info);

	// check that we got the right magic marker value
	if (memcmp(vesa->info->VESASignature, "VESA", 4) != 0)
		return false;

	// it worked!
	return true;
}

// Get info about a specific VESA mode
bool VESA_GetModeInfo(rex_int32 mode)
{
	__dpmi_regs r;
	long dosbuf;
	int c;

	// use the conventional memory transfer buffer
	dosbuf = __tb & 0xFFFFF;

	// initialize the buffer to zero
	for (c = 0; c < sizeof(vesa_mode_info_t); c++)
		_farpokeb(_dos_ds, dosbuf + c, 0);

	// call the VESA function
	r.x.ax = 0x4F01;
	r.x.di = dosbuf & 0xF;
	r.x.es = (dosbuf>>4) & 0xFFFF;
	r.x.cx = mode;
	__dpmi_int(0x10, &r);

	// quit if there was an error
	if (r.h.ah)
		return false;

	// copy the resulting data into our structure
	dosmemget(dosbuf, sizeof(vesa_mode_info_t), vesa->mode_info);

	// it worked!
	return true;
}

// Find a specified VESA mode number
int VESA_FindMode(rex_int32 w, rex_int32 h, rex_int32 bpp)
{
	int mode_list[256];
	int number_of_modes;
	long mode_ptr;
	int c;

	// check that the VESA driver exists, and get information about it
	if (VESA_GetInfo() == false)
		return 0;

	// convert the mode list pointer from seg:offset to a linear address
	mode_ptr = ((vesa->info->VideoModePtr & 0xFFFF0000) >> 12) + (vesa->info->VideoModePtr & 0xFFFF);

	number_of_modes = 0;

	// read the list of available modes
	while (_farpeekw(_dos_ds, mode_ptr) != 0xFFFF)
	{
		mode_list[number_of_modes] = _farpeekw(_dos_ds, mode_ptr);
		number_of_modes++;
		mode_ptr += 2;
	}

	// scan through the list of modes looking for the one that we want
	for (c = 0; c < number_of_modes; c++)
	{
		// get information about this mode
		if (VESA_GetModeInfo(mode_list[c]) == false)
			continue;

		// check the flags field to make sure this is a color graphics mode,
		// * and that it is supported by the current hardware
		if ((vesa->mode_info->ModeAttributes & 0x19) != 0x19)
			continue;

		// check that this mode is the right size
		if ((vesa->mode_info->XResolution != w) || (vesa->mode_info->YResolution != h))
			continue;

		// check that there is only one color plane
		if (vesa->mode_info->NumberOfPlanes != 1)
			continue;

		// check that this is an 8-bit (256 color) mode
		if (vesa->mode_info->BitsPerPixel != bpp)
			continue;

		// if it passed all those checks, this must be the mode we want!
		return mode_list[c];
	}

	// oh dear, there was no mode matching the one we wanted!
	return 0; 
}

// Set the vesa mode from a set of variables
bool VESA_SetMode(rex_int32 w, rex_int32 h, rex_int32 bpp)
{
	__dpmi_regs r;
	int mode_number;

	// find the number for this mode
	mode_number = VESA_FindMode(w, h, bpp);

	if (mode_number < 1)
		return false;

	// call the VESA mode set function
	regs.w.ax = 0x4F02;
	regs.w.bx = mode_number;

	int386(0x10, &regs, &regs);

	if (regs.h.ah)
		return false;

	// it worked!
	return true;
}

// Initialize a platform-specific graphics context
void *Platform_Init_Graphics(rex_int width, rex_int height, rex_int bpp, const char *title)
{
	// Allocate VESA structures
	vesa = (vesa_t *)calloc(1, sizeof(vesa_t));
	vesa->mode_info = (vesa_mode_info_t *)calloc(1, sizeof(vesa_mode_info_t));
	vesa->info = (vesa_info_t *)calloc(1, sizeof(vesa_info_t));

	// Init VESA
	if (VESA_SetMode(width, height, bpp) == false) return nullptr;

	return (void *)vesa;
}

// Shutdown a platform-specific graphics context
void Platform_Quit_Graphics(void *context)
{
	// Free memory
	if (context)
	{
		if (((vesa_t *)context)->mode_info)
			free(((vesa_t *)context)->mode_info);

		if (((vesa_t *)context)->info)
			free(((vesa_t *)context)->info);

		free(context);
	}

	// Set mode 0x03 (text mode)
	regs.w.ax = 0x03;
	int386(0x10, &regs, &regs);
}

//
// Message Handling
//

// Handle printing to the user
bool Platform_PrintHandler(const char *message, message_type type)
{
	switch (type)
	{
		case MESSAGE:
			cout << message << endl;
			return true;

		case WARNING:
			cout << "\033[33m" << message << "\033[0m" << endl;
			return true;

		case FAILURE:
			cout << "\033[31m" << message << "\033[0m" << endl;
			return true;

		default:
			return false;
	}
}

// Handle messages to the user
bool Platform_MessageHandler(const char *title, const char *message, message_type type, time_t time)
{
	rex_string type_str;

	switch (type)
	{
		case MESSAGE: type_str = "Message: "; break;
		case WARNING: type_str = "Warning: "; break;
		case FAILURE: type_str = "Failure: "; break;
		default: return false;
	}

	cout << "\n";
	cout << "+--------------------------------------+" << "\n";
	cout << "| " << type_str.buf << "\n";
	cout << "| " << "\n";
	cout << "| " << title << "\n";
	cout << "| " << "\n";
	cout << "| " << message << "\n";
	cout << "| " << "\n";
	cout << "| " << "Press enter to continue." << "\n";
	cout << "+--------------------------------------+" << "\n";
	cout << endl;

	while (Platform_KeyboardGet(REX_SC_ENTER) == false)
	{
		// this is the shittiest hack ever
		// djgpp INSISTS that something needs to be happening here
		// it also doesn't work with other actions, only cout seems
		// to work
		cout << "\0";
	}

	// If it's a critical failure, quit the engine and exit the program
	if (type == FAILURE)
	{
		Quit_Graphics();
		Quit();
		exit(EXIT_FAILURE);
	}

	return true;
}

} // namespace Rex

//
//
// MAIN PLATFORM ENTRY POINT
//
//

int main(int argc, char **argv)
{
	return RexMain(argc, argv);
}
