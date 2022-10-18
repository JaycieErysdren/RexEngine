// ========================================================
//
// FILE:			/source/core/inc/formats.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for the format systems.
//
// LAST EDITED:		October 18th, 2022
//
// ========================================================

//
// Generic format functions (formats.c)
//

// File format enumerations
enum rex_formats
{
	REX_FORMAT_BUILD_ART,			// BUILD Engine ART
	REX_FORMAT_BUILD_GRP,			// BUILD Engine GRP
	REX_FORMAT_BUILD_MAP,			// BUILD Engine MAP
	REX_FORMAT_BRENDER_DAT,			// BRender datafile
	REX_FORMAT_PORTAL2D_MAP,		// Portal2D map
	REX_FORMAT_IDTECH_BSP,			// idTech BSP
	REX_FORMAT_IDTECH_MDL,			// idTech MDL
	REX_FORMAT_IDTECH_PAK,			// idTech PAK
	REX_FORMAT_IDTECH_WAL,			// idTech WAL
	REX_FORMAT_LOBOTOMY_LEV,		// Lobotomy Software LEV
	REX_FORMAT_LOBOTOMY_PCS,		// Lobotomy Software PCS
	REX_FORMAT_LOBOTOMY_PIC,		// Lobotomy Software PIC
	REX_FORMAT_LGS_RES,				// Looking Glass Studios RES
	REX_FORMAT_MICROSOFT_X,			// Microsoft DirectX "X" model
	REX_FORMAT_MICROSOFT_3DMM,		// Microsoft 3D Movie Maker datafile
	REX_FORMAT_TANKENGINE_TMF,		// Tank Engine TMF
	REX_FORMAT_VALVE_RMF,			// Valve Software RMF
	REX_FORMAT_VALVE_VPK,			// Valve Software VPK
	REX_FORMAT_DESCENT_HOG,			// Descent HOG archive
	REX_FORMAT_DESCENT_PAL,			// Descent palette
	REX_FORMAT_DESCENT_PIG,			// Descent PIG archive
	REX_FORMAT_DESCENT_POF,			// Descent POF model
	REX_FORMAT_VOLITION_VPP,		// Volition VPP archive
};

// File format operations
enum rex_format_operations
{
	REX_FORMATOP_GETINFO,			// List file contents
	REX_FORMATOP_EXTRACT,			// Extract file contents
	REX_FORMATOP_CONVERT,			// Convert file contents
	REX_FORMATOP_VIEW,				// View file contents
};

// Return a string detailing what format operation the user has chosen
rex_byte *Rex_Formats_OptHelper(rex_int optcode);

//
// Lobotomy Software LEV files (lev.c)
//

// Load and process a Lobotomy LEV file. Returns an error code. (Formats/Lobotomy Software/lev_quake.ksy)
rex_int Rex_Formats_Lobotomy_Lev(rex_int operation, rex_byte *filename);

//
// idTech pak files (pak.c)
//

// Load and process an idTech PAK file. Returns an error code. (Formats/id Software/pak.ksy)
rex_int Rex_Formats_idTech_Pak(rex_int operation, rex_byte *filename);
