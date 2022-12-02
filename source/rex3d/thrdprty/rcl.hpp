// ========================================================
//
// FILE:			/source/rex3d/thrdprty/rcl.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		RCL namespace header
//
// LAST EDITED:		December 2nd, 2022
//
// ========================================================

//
//
// RCL namespace
//
//

// RCL namespace definition (public)
namespace RCL
{
	//
	//
	// Functions
	//
	//

	// Initialization
	void Initialize(int sx, int sy, int sa, int lw, int lh, int8_t *ld);

	// Render
	void Render(Picture::pic_t *dst, int max_hits, int max_steps);

	// Rotate camera on X axis
	void CameraRotateX(int x);

	// Transform camera in the current look direction
	void CameraTransformDir(int step);

	// Add x and y to the camera's position
	void CameraTransform(int x, int y);

	// Set the position of the camera
	void CameraSetPosition(int x, int y);

	// Set the grid square of the camera
	void CameraSetSquare(int x, int y);
}
