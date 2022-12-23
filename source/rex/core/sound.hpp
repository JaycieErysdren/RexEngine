// ========================================================
//
// FILE:			/source/rex/core/sound.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Sound header
//
// LAST EDITED:		December 23rd, 2022
//
// ========================================================

#ifndef __REX_SOUND_H__
#define __REX_SOUND_H__

//
//
// Rex namespace: Sound
//
//

namespace Rex
{

typedef enum
{
	SOUND_IMF = 1,
} sound_file_type;

//
//
// Function prototypes
//
//

// Initialize sound driver
bool InitializeSound();

// Shutdown sound driver
void ShutdownSound();

// Open a music file for playback
bool SetMusicFile(string filename, sound_file_type type);

void PlayMusic();

} // namespace Rex

#endif // __REX_SOUND_H__
