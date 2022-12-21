// ========================================================
//
// FILE:			/source/core/inc/lib_io/nuklear.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the Nuklear helpers.
//
// LAST EDITED:		November 7th, 2022
//
// ========================================================

// Rex Nuklear Context
typedef struct
{
	struct nk_context ctx;
	struct nk_rect scissors;
	struct br_pixelmap *pm;
	struct br_pixelmap *font_tex;
	struct nk_font_atlas atlas;
} rex_nuklear_context;

// Initialize Nuklear with source pixelmap, font and font size.
rex_nuklear_context *Rex_Nuklear_Init(br_pixelmap *pm, const rex_byte *font_filename, rex_float font_size);

// Render the given Nuklear context to the defined pixelmap.
void Rex_Nuklear_Render(const rex_nuklear_context *context, const struct nk_color clear, const rex_ubyte enable_clear);

// Shutdown the given Nuklear context.
void Rex_Nuklear_Shutdown(rex_nuklear_context *context);
