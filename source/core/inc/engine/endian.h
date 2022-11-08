// ========================================================
//
// FILE:			/source/core/inc/engine/endian.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the endian-swapping functions.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// System endian check (hack)
extern const rex_int _rex_endian_check;

// Returns true is system is little endian
#define REX_LITTLE_ENDIAN (*(char *)&_rex_endian_check)

// Returns true if system is big endian
#define REX_BIG_ENDIAN !REX_LITTLE_ENDIAN

// Endian swap signed short
void Rex_EndianSwap_Short(rex_short *val);

// Endian swap unsigned short
void Rex_EndianSwap_UShort(rex_ushort *val);

// Endian swap signed int
void Rex_EndianSwap_Int(rex_int *val);

// Endian swap unsigned int
void Rex_EndianSwap_UInt(rex_uint *val);

// Endian swap signed long
void Rex_EndianSwap_Long(rex_long *val);

// Endian swap unsigned long
void Rex_EndianSwap_ULong(rex_ulong *val);
