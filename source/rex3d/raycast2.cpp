// ========================================================
//
// FILE:			/source/raycast2.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Raycaster namespace implementation
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

// Rex3D header
#include "rex3d.hpp"

// raycastlib header
#define RCL_PIXEL_FUNCTION PlacePixel
#include "thirdparty/raycastlib.h"

//
//
// Raycaster namespace
//
//

// Raycaster namespace definition (private)
namespace Raycaster
{
	//
	//
	// Variables
	//
	//

	RCL_Camera camera;
	RCL_RayConstraints constraints;
}

//
//
// Functions
//
//

//
// Helper functions
//

void PlacePixel(RCL_PixelInfo *pixel)
{
	if (pixel->isWall)
		VGA::PlacePixel(pixel->position.x, pixel->position.y, 64);
	else if (pixel->isFloor)
		VGA::PlacePixel(pixel->position.x, pixel->position.y, 0);
	else if (pixel->isHorizon)
		VGA::PlacePixel(pixel->position.x, pixel->position.y, 96);
}

RCL_Unit HeightAt(int16_t x, int16_t y)
{
	return (x < 0 || x > 10 || y < 0 || y > 10) ? 2 * RCL_UNITS_PER_SQUARE : 0;
}

//
// Bootstrap
//

void Raycaster::Initialize()
{
	RCL_initCamera(&camera);
	camera.position.x = 5 * RCL_UNITS_PER_SQUARE;
	camera.position.y = 6 * RCL_UNITS_PER_SQUARE;
	camera.direction = 5 * RCL_UNITS_PER_SQUARE / 6;
	camera.resolution.x = 320;
	camera.resolution.y = 200;

	RCL_initRayConstraints(&constraints);
	constraints.maxHits = 1;
	constraints.maxSteps = 40;
}

//
// Rendering
//

void Raycaster::Render()
{
	RCL_renderSimple(camera, HeightAt, 0, 0, constraints);
}
