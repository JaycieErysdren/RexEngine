// ========================================================
//
// FILE:			/source/rex/modules/fileio/imf/imf.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		IMF namespace: Implementation
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// IMF header
#include "imf.hpp"

//
//
// IMF namespace: Implementation
//
//

namespace IMF
{

//
//
// Globals
//
//

rex_uint16 *imf_buffer;
rex_int imf_size;
rex_int imf_position;

//
//
// Functions
//
//

bool Open(string filename)
{
	Rex::File imf_file;

	cout << "0" << endl;

	// try to open the file
	if (imf_file.Open(filename) == false)
		return false;

	imf_size = imf_file.filesize / 2;

	if (imf_buffer != NULL) free (imf_buffer);

	// allocate buffer
	if ((imf_buffer = (rex_uint16 *)calloc(imf_size, sizeof(rex_uint16))) == NULL)
		return false;

	// read in buffer
	if (imf_file.Read(sizeof(rex_uint16), imf_size, imf_buffer) == false)
		return false;

	// close file handle
	imf_file.Close();

	imf_position = 0;

	return true;
}

void Service()
{
	if (imf_buffer != NULL)
	{
		rex_uint16 reg;
		rex_uint16 val;

		reg = imf_buffer[imf_position];
		val = imf_buffer[imf_position + 1];

		ADLIB::Output(reg & 0xFF, val & 0xFF);

		imf_position += 2;
	}
}

void Close()
{
	if (imf_buffer)
		free(imf_buffer);
}

} // namespace IMF
