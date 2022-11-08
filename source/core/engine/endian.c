// ========================================================
//
// FILE:			/source/core/engine/endian.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Endian-swapping functions.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// System endian check (hack)
const rex_int _rex_endian_check = 1;

// Endian swap signed short
void Rex_EndianSwap_Short(rex_short *val)
{
	*val = (*val << 8) | ((*val >> 8) & 0xFF);
}

// Endian swap unsigned short
void Rex_EndianSwap_UShort(rex_ushort *val)
{
	*val = (*val << 8) | (*val >> 8 );
}

// Endian swap signed int
void Rex_EndianSwap_Int(rex_int *val)
{
	rex_int out = ((*val << 8) & 0xFF00FF00) | ((*val >> 8) & 0xFF00FF );
	*val = (out << 16) | ((out >> 16) & 0xFFFF);
}

// Endian swap unsigned int
void Rex_EndianSwap_UInt(rex_uint *val)
{
	rex_uint out = ((*val << 8) & 0xFF00FF00 ) | ((*val >> 8) & 0xFF00FF );
	*val = (out << 16) | (out >> 16);
}

// Endian swap signed long
void Rex_EndianSwap_Long(rex_long *val)
{
	rex_long out = ((*val << 8) & 0xFF00FF00FF00FF00 ) | ((*val >> 8) & 0x00FF00FF00FF00FF );
	out = ((out << 16) & 0xFFFF0000FFFF0000 ) | ((out >> 16) & 0x0000FFFF0000FFFF );
	*val = (out << 32) | ((out >> 32) & 0xFFFFFFFF);
}

// Endian swap unsigned long
void Rex_EndianSwap_ULong(rex_ulong *val)
{
	rex_ulong out = ((*val << 8) & 0xFF00FF00FF00FF00U ) | ((*val >> 8) & 0x00FF00FF00FF00FFU );
	out = ((out << 16) & 0xFFFF0000FFFF0000U ) | ((out >> 16) & 0x0000FFFF0000FFFFU );
	*val = (out << 32) | (out >> 32);
}
