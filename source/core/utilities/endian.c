// ========================================================
//
// FILE:			/source/core/utilities/endian.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Endian-swapping functions.
//
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// System endian check (hack)
const rex_int _rex_endian_check = 1;

// Endian swap signed short
rex_short Rex_EndianSwap_Short(rex_short val)
{
	return (val << 8) | ((val >> 8) & 0xFF);
}

// Endian swap unsigned short
rex_ushort Rex_EndianSwap_UShort(rex_ushort val)
{
	return (val << 8) | (val >> 8 );
}

// Endian swap signed int
rex_int Rex_EndianSwap_Int(rex_int val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
	return (val << 16) | ((val >> 16) & 0xFFFF);
}

// Endian swap unsigned int
rex_uint Rex_EndianSwap_UInt(rex_uint val)
{
	val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
	return (val << 16) | (val >> 16);
}

// Endian swap signed long
rex_long Rex_EndianSwap_Long(rex_long val)
{
	val = ((val << 8) & 0xFF00FF00FF00FF00 ) | ((val >> 8) & 0x00FF00FF00FF00FF );
	val = ((val << 16) & 0xFFFF0000FFFF0000 ) | ((val >> 16) & 0x0000FFFF0000FFFF );
	return (val << 32) | ((val >> 32) & 0xFFFFFFFF);
}

// Endian swap unsigned long
rex_ulong Rex_EndianSwap_ULong(rex_ulong val)
{
	val = ((val << 8) & 0xFF00FF00FF00FF00U ) | ((val >> 8) & 0x00FF00FF00FF00FFU );
	val = ((val << 16) & 0xFFFF0000FFFF0000U ) | ((val >> 16) & 0x0000FFFF0000FFFFU );
	return (val << 32) | (val >> 32);
}
