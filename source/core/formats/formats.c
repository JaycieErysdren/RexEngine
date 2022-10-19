// ========================================================
//
// FILE:			/source/core/formats/formats.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Generic format functions.
//
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

rex_coord3f *gl_vertices_f;
rex_int num_gl_vertices_f;

// Return a string detailing what format operation the user has chosen
rex_byte *Rex_Formats_OptHelper(rex_int optcode)
{
	switch (optcode)
	{
		case REX_FORMATOP_GETINFO:
			return "List file contents.";

		case REX_FORMATOP_EXTRACT:
			return "Extract file contents.";

		case REX_FORMATOP_CONVERT:
			return "Convert file contents.";

		case REX_FORMATOP_VIEW:
			return "View file contents.";

		default:
			return "Invalid operation code";
	}
}
