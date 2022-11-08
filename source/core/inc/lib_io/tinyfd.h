// ========================================================
//
// FILE:			/source/core/inc/lib_io/tinyfd.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the tinyfiledialogs helpers.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Open a native file dialog window.
rex_byte *Rex_IO_OpenFileDialog(rex_byte *title, rex_byte *default_path, rex_int num_filter_patterns, rex_byte **filter_patterns, rex_byte *filter_descriptions, rex_int allow_multiple_selects);
