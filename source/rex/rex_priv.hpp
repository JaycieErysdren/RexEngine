// ========================================================
//
// FILE:			/source/rex/rev_priv.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex Engine private header
//
// LAST EDITED:		December 30th, 2022
//
// ========================================================

#ifndef __REX_ENGINE_PRIVATE_H__
#define __REX_ENGINE_PRIVATE_H__

//
//
// Rex Engine public header
//
//

#include "rex.hpp"

//
//
// Rex Engine private headers
//
//

// Platform
#include "platform/platform.hpp"

// Commandline Args
#include "core/args.hpp"

// Logging
#include "core/log.hpp"

// Memory Pool
#include "core/mempool.hpp"

// Virtual File System
#include "core/vfs.hpp"

//
// Hardware I/O modules
//

// DOS hardware
#if (REX_TARGET == PLATFORM_DOS)

	// DOS
	#if MODULE_DOS
	#include "modules/hwio/dos/dos.hpp"
	#endif

	// VGA
	#if MODULE_VGA
	#include "modules/hwio/vga/vga.hpp"
	#endif

	// VESA
	#if MODULE_VESA
	#include "modules/hwio/vesa/vesa.hpp"
	#endif

	// ADLIB
	#if MODULE_ADLIB
	#include "modules/hwio/adlib/adlib.hpp"
	#endif

#endif

//
// File I/O modules
//

// PAK
#if MODULE_PAK
#include "modules/fileio/pak/pak.hpp"
#endif

// ZIP
#if MODULE_ZIP
#include "modules/fileio/zip/zip.hpp"
#endif

// GRP
#if MODULE_GRP
#include "modules/fileio/grp/grp.hpp"
#endif

// WAD
#if MODULE_WAD
#include "modules/fileio/wad/wad.hpp"
#endif

// ART
#if MODULE_ART
#include "modules/fileio/art/art.hpp"
#endif

// IMF
#if MODULE_IMF
#include "modules/fileio/imf/imf.hpp"
#endif

#endif // __REX_ENGINE_PRIVATE_H__
