// ========================================================
//
// FILE:			/source/core/lib_io/tinyfd.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		tinyfiledialogs helpers.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Include Rex Engine header
#include "rex.h"

// Open a native file dialog window.
rex_byte *Rex_IO_OpenFileDialog(rex_byte *title, rex_byte *default_path, rex_int num_filter_patterns, rex_byte **filter_patterns, rex_byte *filter_descriptions, rex_int allow_multiple_selects)
{
	return (rex_byte *)tinyfd_openFileDialog(title, default_path, num_filter_patterns, (const char *const *)filter_patterns, filter_descriptions, allow_multiple_selects);
}
