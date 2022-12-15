// ========================================================
//
// FILE:			/source/rex/core/camera.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Camera header
//
// LAST EDITED:		December 15th, 2022
//
// ========================================================

#ifndef __REX_CAMERA_H__
#define __REX_CAMERA_H__

//
//
// Rex namespace: Camera
//
//

namespace Rex
{

//
//
// Classes
//
//

class Camera
{
	public:
		rex_vec3s origin;
		rex_vec3s velocity;
		rex_vec3i angles;
		rex_scalar draw_distance;
		rex_int32 movespeedkey;
		rex_int32 anglespeedkey;
};

} // namespace Rex

#endif // __REX_CAMERA_H__
