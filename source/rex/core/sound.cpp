// ========================================================
//
// FILE:			/source/rex/core/sound.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Sound implementation
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Sound
//
//

namespace Rex
{

//
//
// Functions
//
//

bool InitializeSound()
{
	#if (REX_TARGET == PLATFORM_DOS)

		#if (MODULE_ADLIB)
			return ADLIB::Detect();
		#endif

	#else
		return false;
	#endif
}

void ShutdownSound()
{
	IMF::Close();

	#if (REX_TARGET == PLATFORM_DOS)

		#if (MODULE_ADLIB)
			return ADLIB::Reset();
		#endif

	#endif
}

bool SetMusicFile(string filename, sound_file_type type)
{
	switch (type)
	{
		case SOUND_IMF:
			return IMF::Open(filename);

		default:
			return false;
	}
}

// Play music in buffer
void PlayMusic()
{
	IMF::Service();
}

} // namespace Rex
