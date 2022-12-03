// ========================================================
//
// FILE:			/source/rex3d/thrdprty/rcl.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		RCL namespace implementation
//
// LAST EDITED:		December 2nd, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

//
//
// RCL namespace
//
//

// RCL namespace definition (private)
namespace RCL
{

//
//
// Ok
//
//

// raycastlib header
#define RCL_PIXEL_FUNCTION Pixel
#define RCL_COMPUTE_FLOOR_DEPTH 0
#define RCL_COMPUTE_CEILING_DEPTH 0
#include "thirdparty/rcl/raycastlib.h"

//
//
// Global variables
//
//

int level_height, level_width;
Picture::pic_t *pic_rbuffer;
int8_t *level_data;
RCL_Camera camera;

//
//
// Functions
//
//

//
// Forward declarations
//

RCL_Unit Height(int16_t x, int16_t y);
void Pixel(RCL_PixelInfo *p);

//
// Bootstrap
//

// Initialization
void Initialize(int sx, int sy, int sa, int lw, int lh, int8_t *ld)
{
	RCL_initCamera(&camera);
	level_height = lh;
	level_width = lw;
	level_data = ld;

	camera.position.x = sx * RCL_UNITS_PER_SQUARE;
	camera.position.y = sy * RCL_UNITS_PER_SQUARE;
	camera.direction = sa;
}

void InitializeTileMap(int start_x, int start_y, int start_angle, Tiled::TileMap *tilemap)
{
	
}

//
// Camera helpers
//

void CameraTransform(int step)
{
	RCL_Vector2D offset;
	RCL_Vector2D angle = RCL_angleToDirection(camera.direction);
	angle.x /= 10;
	angle.y /= 10;

	offset.x = step * angle.x;
	offset.y = step * angle.y;

	RCL_moveCameraWithCollision(&camera, offset, 0, Height, 0, 1, 1);
}

void CameraRotateX(int x)
{
	camera.direction += x;
}

void CameraTransformDir(int step)
{
	RCL_Vector2D angle = RCL_angleToDirection(camera.direction);
	angle.x /= 10;
	angle.y /= 10;

	camera.position.x += step * angle.x;
	camera.position.y += step * angle.y;
}

void CameraSetPosition(int x, int y)
{
	camera.position.x = x;
	camera.position.y = y;
}

void CameraSetSquare(int x, int y)
{
	camera.position.x = x * RCL_UNITS_PER_SQUARE;
	camera.position.y = y * RCL_UNITS_PER_SQUARE;
}

//
// Rendering
//

// Determining the height of a square in the level
RCL_Unit Height(int16_t x, int16_t y)
{
	int32_t index = y * level_width + x;

	if (index < 0 || (index >= (level_width * level_height)))
		return RCL_UNITS_PER_SQUARE * 2;

	return level_data[y * level_width + x] * RCL_UNITS_PER_SQUARE * 2;
}

// Function for plotting pixels
void Pixel(RCL_PixelInfo *p)
{
	uint8_t color = 0;

	if (p->isWall)
	{
		switch (p->hit.direction)
		{
			case 0: color = 47; break;
			case 1: color = 127; break;
			case 2: color = 149; break;
			case 3: color = 191; break;
			default: color = 7; break;
		}
	}
	else if (p->isFloor)
	{
		color = 224;
	}
	else if (p->isHorizon)
	{
		color = 64;
	}

	Picture::DrawPixel(pic_rbuffer, p->position.x, p->position.y, color);
}

// Render
void Render(Picture::pic_t *dst, int max_hits, int max_steps)
{
	pic_rbuffer = dst;
	camera.resolution.x = dst->width;
	camera.resolution.y = dst->height;

	RCL_RayConstraints c;
	RCL_initRayConstraints(&c);
	c.maxHits = max_hits;
	c.maxSteps = max_steps;

	RCL_renderSimple(camera, Height, 0, 0, c);
}

} // namespace RCL
