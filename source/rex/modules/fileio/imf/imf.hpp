// ========================================================
//
// FILE:			/source/rex/modules/fileio/imf/imf.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		IMF namespace: Header
//
// LAST EDITED:		December 22nd, 2022
//
// ========================================================

#ifndef __IMF_H__
#define __IMF_H__

//
//
// IMF namespace: Header
//
//

namespace IMF
{

//
//
// Functions
//
//

bool Open(string filename);
void Service();
void Close();

} // namespace IMF

#endif // __IMF_H__
