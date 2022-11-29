// ========================================================
//
// FILE:			/source/raycastr.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		RaycastRenderer namespace implementation
//
// LAST EDITED:		November 29th, 2022
//
// ========================================================

// Rex3D header
#include "rex3d.hpp"

#define texHeight 64
#define texWidth 64

// Placeholder world map
uint8_t world_map[24][24] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,0,0,0,4,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

//
//
// RaycastRenderer namespace
//
//

// RaycastRenderer namespace definition (private)
namespace RaycastRenderer
{
	// Texture array
	vector<Texture> textures;
}

//
// Functions
//

// Load some textures into memory
void RaycastRenderer::LoadTextures()
{
	// Variables
	Texture texture0;
	Texture texture1;
	Texture texture2;
	Texture texture3;
	FILE *file;

	// Texture 0
	texture0.width = texWidth;
	texture0.height = texHeight;
	texture0.pixels = new uint8_t [texture0.width * texture0.height];

	file = fopen("wall001l.tex", "rb");
	fread(texture0.pixels, sizeof(uint8_t), texture0.width * texture0.height, file);
	fclose(file);

	textures.push_back(texture0);

	// Texture 1
	texture1.width = texWidth;
	texture1.height = texHeight;
	texture1.pixels = new uint8_t [texture1.width * texture1.height];

	file = fopen("wall001d.tex", "rb");
	fread(texture1.pixels, sizeof(uint8_t), texture1.width * texture1.height, file);
	fclose(file);

	textures.push_back(texture1);

	// Texture 2
	texture2.width = texWidth;
	texture2.height = texHeight;
	texture2.pixels = new uint8_t [texture2.width * texture2.height];

	file = fopen("floor001.tex", "rb");
	fread(texture2.pixels, sizeof(uint8_t), texture2.width * texture2.height, file);
	fclose(file);

	textures.push_back(texture2);

	// Texture 3
	texture3.width = texWidth;
	texture3.height = texHeight;
	texture3.pixels = new uint8_t [texture3.width * texture3.height];

	file = fopen("ceil001.tex", "rb");
	fread(texture3.pixels, sizeof(uint8_t), texture3.width * texture3.height, file);
	fclose(file);

	textures.push_back(texture3);
}

// Cast rays into the world
void RaycastRenderer::Render(Camera &camera, int width, int height, bool enable_textures, bool enable_floors)
{
	int x, y;

    //FLOOR CASTING
	if (enable_floors == true)
	{
		for (y = 0; y < height; y++)
		{
			// rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
			scalar_t rayDirX0 = camera.angle[0] - camera.plane[0];
			scalar_t rayDirY0 = camera.angle[1] - camera.plane[1];
			scalar_t rayDirX1 = camera.angle[0] + camera.plane[0];
			scalar_t rayDirY1 = camera.angle[1] + camera.plane[1];

			// Current y position compared to the center of the screen (the horizon)
			int p = y - height / 2;

			// Horizontal distance from the camera to the floor for the current row.
			// 0.5 is the z position exactly in the middle between floor and ceiling.
			scalar_t rowDistance = camera.origin[2] / p;

			// calculate the real world step vector we have to add for each x (parallel to camera plane)
			// adding step by step avoids multiplications with a weight in the inner loop
			scalar_t floorStepX = rowDistance * (rayDirX1 - rayDirX0) / width;
			scalar_t floorStepY = rowDistance * (rayDirY1 - rayDirY0) / width;

			// real world coordinates of the leftmost column. This will be updated as we step to the right.
			scalar_t floorX = camera.origin[0] + rowDistance * rayDirX0;
			scalar_t floorY = camera.origin[1] + rowDistance * rayDirY0;

			for(int x = 0; x < width; x++)
			{
				// the cell coord is simply got from the integer parts of floorX and floorY
				int cellX = (int)(floorX);
				int cellY = (int)(floorY);

				// get the texture coordinate from the fractional part
				int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
				int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

				floorX += floorStepX;
				floorY += floorStepY;

				uint8_t color;

				if (enable_textures == true)
				{
					// choose texture and draw the pixel

					// floor
					color = textures[2].pixels[texWidth * ty + tx];
					VGA::PlacePixel(x, y, color);

					//ceiling (symmetrical, at height - y - 1 instead of y)
					color = textures[3].pixels[texWidth * ty + tx];
					VGA::PlacePixel(x, height - y - 1, color);
				}
				else
				{
					// floor
					color = 2;
					VGA::PlacePixel(x, y, color);

					// ceiling
					color = 4;
					VGA::PlacePixel(x, height - y - 1, color);
				}
			}
		}
	}

	// WALL CASTING
	for (x = 0; x < width; x++)
	{
		//calculate ray position and direction
		scalar_t cameraX = 2 * x / SCALAR(width) - 1; //x-coordinate in camera space
		scalar_t rayDirX = camera.angle[0] + camera.plane[0] * cameraX;
		scalar_t rayDirY = camera.angle[1] + camera.plane[1] * cameraX;

		//which box of the map we're in
		int mapX = int(camera.origin[0]);
		int mapY = int(camera.origin[1]);

		//length of ray from current position to next x or y-side
		scalar_t sideDistX;
		scalar_t sideDistY;

		//length of ray from one x or y-side to next x or y-side
		//these are derived as:
		//deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
		//deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
		//which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
		//where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
		//unlike (dirX, dirY) is not 1, however this does not matter, only the
		//ratio between deltaDistX and deltaDistY matters, due to the way the DDA
		//stepping further below works. So the values can be computed as below.
		// Division through zero is prevented, even though technically that's not
		// needed in C++ with IEEE 754 floating point values.
		scalar_t deltaDistX = (rayDirX == 0) ? 1e30 : abs(1 / rayDirX);
		scalar_t deltaDistY = (rayDirY == 0) ? 1e30 : abs(1 / rayDirY);

		scalar_t perpWallDist;

		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;

		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?
		//calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (camera.origin[0] - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - camera.origin[0]) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (camera.origin[1] - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - camera.origin[1]) * deltaDistY;
		}
		//perform DDA
		while (hit == 0)
		{
			//jump to next map square, either in x-direction, or in y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Check if ray has hit a wall
			if(world_map[mapX][mapY] > 0) hit = 1;
		}

		//Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
		//hit to the camera plane. Euclidean to center camera point would give fisheye effect!
		//This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
		//for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
		//because they were left scaled to |rcameraXst);
		if (side == 0)	perpWallDist = (sideDistX - deltaDistX);
		else			perpWallDist = (sideDistY - deltaDistY);

		//Calculate height of line to draw on screen
		int lineHeight = (int)(height / perpWallDist);

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + height / 2;
		if(drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + height / 2;
		if(drawEnd >= height) drawEnd = height - 1;

		if (enable_textures == true)
		{
			// texturing calculations
			int texNum = world_map[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

			// calculate value of wallX
			scalar_t wallX; //where exactly the wall was hit
			if (side == 0) wallX = camera.origin[1] + perpWallDist * rayDirY;
			else           wallX = camera.origin[0] + perpWallDist * rayDirX;
			wallX -= floor((wallX));

			// x coordinate on the texture
			int texX = int(wallX * SCALAR(texWidth));
			if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
			if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

            // How much to increase the texture coordinate per screen pixel
			scalar_t step = 1.0 * texHeight / lineHeight;
			// Starting texture coordinate
			scalar_t texPos = (drawStart - height / 2 + lineHeight / 2) * step;

			for (y = drawStart; y < drawEnd; y++)
			{
				// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
				int texY = (int)texPos & (texHeight - 1);
				texPos += step;
				uint8_t color = side ? textures[1].pixels[texHeight * texY + texX] : textures[0].pixels[texHeight * texY + texX];
				VGA::PlacePixel(x, y, color);
			}
		}
		else
		{
			//choose wall color
			uint8_t color;
			switch (world_map[mapX][mapY])
			{
				case 1:  color = 40; break; //red
				case 2:  color = 47; break; //green
				case 3:  color = 32; break; //blue
				case 4:  color = 15; break; //white
				default: color = 14; break; //yellow
			}

			//give x and y sides different brightness
			if(side == 1) {color -= 1;}

			//draw the pixels of the stripe as a vertical line
			for (y = drawStart; y < drawEnd; y++)
			{
				VGA::PlacePixel(x, y, color);
			}
		}
	}
}
