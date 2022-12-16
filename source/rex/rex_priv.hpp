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
// LAST EDITED:		December 11th, 2022
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
// Globals (ew)
//
//

extern Rex::VidInfo vid_info;

//
//
// Rex Engine private headers
//
//

//
// Core modules
//

// Math
#include "modules/core/math/math.hpp"

//
// Core modules (optional)
//

// Palette
#if MODULE_PALETTE
#include "modules/core/palette/palette.hpp"
#endif

// Utils
#if MODULE_UTILS
#include "modules/core/utils/utils.hpp"
#endif

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

#endif

#endif // __REX_ENGINE_PRIVATE_H__
