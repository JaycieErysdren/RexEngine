// ========================================================
//
// FILE:			/source/rex/modules/fileio/adlib/adlib.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		ADLIB namespace: Implementation
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// ADLIB header
#include "adlib.hpp"

//
//
// ADLIB namespace: Implementation
//
//

namespace ADLIB
{

//
//
// Functions
//
//

// Send an output to the AdLib ports
void Output(rex_uint8 reg, rex_uint8 data)
{
	rex_int i;

	outportb(ADLIB_PORT, reg);

	for (i = 6; i > 0; i--)
		inportb(ADLIB_PORT);

	outportb(ADLIB_PORT + 1, data);

	for (i = 35; i > 0; i--)
		inportb(ADLIB_PORT);
}

// Reset the AdLib card
void Reset()
{
	rex_uint8 i, slot1, slot2;

	static rex_uint8 slotVoice[9][2] = {{0,3},{1,4},{2,5},{6,9},{7,10},{8,11},{12,15},{13,16},{14,17}};
	static rex_uint8 offsetSlot[18] = {0,1,2,3,4,5,8,9,10,11,12,13,16,17,18,19,20,21};

	// Set WSE=1
	Output(1, 0x20);

	// Set CSM=0 & SEL=0
	Output(8, 0);

	// Set AM Depth, VIB depth & Rhythm = 0
	Output(0xBD, 0);

	for(i = 0; i < 9; i++)
	{
		slot1 = offsetSlot[slotVoice[i][0]];
		slot2 = offsetSlot[slotVoice[i][1]];

		// Turn note off
		Output(0xB0 + i, 0);

		// Clear frequency
		Output(0xA0 + i, 0);

		Output(0xE0 + slot1, 0);
		Output(0xE0 + slot2, 0);

		Output(0x60 + slot1, 0xff);
		Output(0x60 + slot2, 0xff);
		Output(0x80 + slot1, 0xff);
		Output(0x80 + slot2, 0xff);

		Output(0x40 + slot1, 0xff);
		Output(0x40 + slot2, 0xff);
	}
}

// Clear all AdLib registers
void ClearRegisters()
{
	rex_uint8 i;

	//clear all registers
	for (i = 1; i <= 0xF5; i++)
		Output(i, 0);

	// Set WSE=1
	Output(1, 0x20);
}

// Detect if an AdLib card is present
bool Detect()
{
	rex_int status1, status2, i;

	Output(4, 0x60);
	Output(4, 0x80);

	status1 = inportb(ADLIB_PORT);
	
	Output(2, 0xFF);
	Output(4, 0x21);

	for (i = 100; i > 0; i--)
		inportb(ADLIB_PORT);

	status2 = inportb(ADLIB_PORT);
	
	Output(4, 0x60);
	Output(4, 0x80);

	if (((status1 & 0xe0) == 0x00) && ((status2 & 0xe0) == 0xc0))
	{
		ClearRegisters();
		return true;
	}
	else
	{
		return false;
	}
}

} // namespace ADLIB
