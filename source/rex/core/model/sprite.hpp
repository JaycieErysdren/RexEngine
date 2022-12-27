// ========================================================
//
// FILE:			/source/rex/core/model/sprite.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Sprite Model header
//
// LAST EDITED:		December 26th, 2022
//
// ========================================================

#ifndef __REX_MODEL_SPRITE_H__
#define __REX_MODEL_SPRITE_H__

//
//
// Rex namespace: Sprite Model
//
//

namespace Rex
{

//
// Array of Surface frames
//

class SpriteData
{
	public:
		vector<Surface> frames;
};

} // namespace Rex

#endif // __REX_MODEL_SPRITE_H__
