// ========================================================
//
// FILE:			/source/rex/core/bootstrp.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Bootstrap header
//
// LAST EDITED:		January 6th, 2023
//
// ========================================================

#ifndef __REX_BOOTSTRAP_H__
#define __REX_BOOTSTRAP_H__

//
//
// Rex namespace: Bootstrap
//
//

namespace Rex
{

// Initialize basic functions of Rex Engine
bool Init();

// Shutdown basic functions of Rex Engine
void Quit();

// Returns true if the main loop should be looping
bool DoMainLoop();

} // namespace Rex

#endif // __REX_BOOTSTRAP_H__
