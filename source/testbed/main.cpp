// ========================================================
//
// FILE:			/source/testbed/main.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Testbed program entry point
//
// LAST EDITED:		December 11th, 2022
//
// ========================================================

// Testbed header
#include "testbed.hpp"

#define CYCLES 30

//
// Types
//

// Math tables
typedef struct
{
	rex_scalar cos[360];
	rex_scalar sin[360];
	rex_scalar tan[360];
} math_t;

// Camera
typedef struct
{
	rex_vec3s origin;				// X, Y, Z
	rex_vec3s velocity;				// X, Y, Z
	rex_vec3i angles;				// Pitch, yaw, roll
	rex_scalar draw_distance;		// Draw distance (scalar units)
	rex_int32 movespeedkey;
	rex_int32 anglespeedkey;
} camera_t;

// Voxel
typedef struct
{
	uint8_t density;
	uint8_t color;
} voxel_t;

//
// Globals
//

#define VOXMAP_X 64
#define VOXMAP_Y 64
#define VOXMAP_Z 64

// V-ReX
voxel_t voxmap[VOXMAP_Z][VOXMAP_Y][VOXMAP_X];
voxel_t *voxworld;
rex_vec3i voxworld_dim;
uint8_t *heightmap;
uint8_t *colormap;

// Voxels
int32_t *ybuffer;
uint8_t *f_heightmap;
uint8_t *f_colormap;
uint8_t *c_heightmap;
uint8_t *c_colormap;

// Camera
camera_t camera;

// Math tables
math_t math;

// Console buffer
char console_buffer[256];

// Bools
bool shading = false;

//
// Camera
//

void CameraController()
{
	// Mouse read
	static rex_int32 mx_prev, my_prev;
	rex_int32 delta_mx, delta_my;
	rex_int32 mb, mx, my;
	Rex::MouseRead(&mb, &mx, &my);

	delta_mx = mx_prev - mx;
	delta_my = my_prev - my;

	// Mouse look
	if (mb == 1 && delta_mx != 0) camera.angles.y += delta_mx;
	if (mb == 2 && delta_my != 0) camera.angles.x -= delta_my;

	// Reset pitch
	if (mb == 3)
		camera.angles.x = 0;
	
	camera.anglespeedkey = 2;

	// Keyboard look
	{
		// Rotate leftwards
		if (Rex::KeyTest(REX_KB_LTARROW)) camera.angles.y += camera.anglespeedkey;

		// Rotate rightwards
		if (Rex::KeyTest(REX_KB_RTARROW)) camera.angles.y -= camera.anglespeedkey;

		// Look upwards
		if (Rex::KeyTest(REX_KB_UPARROW)) camera.angles.x += camera.anglespeedkey;

		// Look downwards
		if (Rex::KeyTest(REX_KB_DNARROW)) camera.angles.x -= camera.anglespeedkey;
	}

	// Pitch angle sanity checks
	if (camera.angles.x < -90) camera.angles.x = -90;
	if (camera.angles.x > 90) camera.angles.x = 90;

	// Yaw angle sanity checks
	if (camera.angles.y < 0) camera.angles.y += 360;
	if (camera.angles.y > 359) camera.angles.y -= 360;

	// Check if sprinting
	if (Rex::KeyTest(REX_KB_LTSHIFT))
		camera.movespeedkey = 2;
	else
		camera.movespeedkey = 1;

	// Set velocity
	camera.velocity.x = math.sin[camera.angles.y] * camera.movespeedkey;
	camera.velocity.y = math.cos[camera.angles.y] * camera.movespeedkey;
	camera.velocity.z = REX_SCALAR(1.0f) * camera.movespeedkey;

	// Move forwards
	if (Rex::KeyTest(REX_KB_W))
	{
		camera.origin.x += camera.velocity.x;
		camera.origin.y += camera.velocity.y;
	}

	// Move backwards
	if (Rex::KeyTest(REX_KB_S))
	{
		camera.origin.x -= camera.velocity.x;
		camera.origin.y -= camera.velocity.y;
	}

	// Move leftwards
	if (Rex::KeyTest(REX_KB_A))
	{
		camera.origin.x += camera.velocity.y;
		camera.origin.y -= camera.velocity.x;
	}

	// Move rightwards
	if (Rex::KeyTest(REX_KB_D))
	{
		camera.origin.x -= camera.velocity.y;
		camera.origin.y += camera.velocity.x;
	}

	// Move upwards
	if (Rex::KeyTest(REX_KB_Q))
		camera.origin.z += camera.velocity.z;

	// Move downwards
	if (Rex::KeyTest(REX_KB_E))
		camera.origin.z -= camera.velocity.z;

	mx_prev = mx;
	my_prev = my;
}

//
// V-ReX
//

void VReXInit()
{
	rex_int x, y, z;
	char line[256];
	char *token;
	char sep[2] = " ";
	rex_int num;
	rex_int vx, vy, vz;

	FILE *file = fopen("voxel/goxel2.txt", "rt");

	while (fgets(line, sizeof(line), file))
	{
		token = strtok(line, sep);

		for (rex_int i = 0; i < 3; i++)
		{
			num = atoi(token);

			switch (i)
			{
				case 0: vx = num; break;
				case 1: vy = num; break;
				case 2: vz = num; break;
				default: break;
			}

			token = strtok(NULL, sep);
		}

		voxmap[vz][vy][vx].density = 255;
		voxmap[vz][vy][vx].color = 31;
	}

	fclose(file);

	#ifdef GENERATOR

	// Make a square
	for (z = 0; z < VOXMAP_Z; z++)
	{
		for (y = 0; y < VOXMAP_Y; y++)
		{
			for (x = 0; x < VOXMAP_X; x++)
			{
				voxmap[z][y][x].color = x + y + z;

				if (x < 12 || x > 20 || y < 12 || y > 20 || z < 12 || z > 20) 
				{
					voxmap[z][y][x].density = 0;
				}
				else
				{
					voxmap[z][y][x].density = 255;
				}

				// make a floor
				if (z == 0) voxmap[z][y][x].density = 255;
			}
		}
	}

	#endif

	#ifdef CRINGE

	// Load a VOX
	FILE *vox = fopen("voxel/tank.vox", "rb");

	rex_int32 dx, dy, dz;

	fread(&dx, sizeof(rex_int32), 1, vox);
	fread(&dy, sizeof(rex_int32), 1, vox);
	fread(&dz, sizeof(rex_int32), 1, vox);

	uint8_t *voxbuff = (uint8_t *)calloc(dx * dy * dz, sizeof(rex_uint8));
	voxworld = (voxel_t *)calloc(dx * dy * dz, sizeof(voxel_t));

	voxworld_dim.x = dx;
	voxworld_dim.y = dy;
	voxworld_dim.z = dz;

	fread(voxbuff, sizeof(rex_uint8), dx * dy * dz, vox);

	for (rex_int i = 0; i < (dx * dy * dz); i++)
	{
		if (voxbuff[i] == 255)
		{
			voxworld[i].density = 0;
			voxworld[i].color = 0;
		}
		else
		{
			voxworld[i].density = 255;
			voxworld[i].color = voxbuff[i];
		}
	}

	free(voxbuff);

	fclose(vox);

	#endif

	// Position (scalar units)
	camera.origin.x = REX_SCALAR(16);
	camera.origin.y = REX_SCALAR(16);
	camera.origin.z = REX_SCALAR(16);

	// Angle (degrees)
	camera.angles.x = 0; // pitch
	camera.angles.y = 0; // yaw
	camera.angles.z = 0; // roll

	// Draw distance (scalar units)
	camera.draw_distance = REX_SCALAR(64);
}

void VReXRender(Rex::Surface *dst, rex_rect area)
{
	// Drawable area
	rex_int draw_w = area.x2 - area.x1;
	rex_int draw_h = area.y2 - area.y1;

	// Camera yaw sin and cos
	rex_scalar sn = math.sin[camera.angles.y];
	rex_scalar cs = math.cos[camera.angles.y];

	rex_scalar snx = math.sin[camera.angles.x];
	rex_scalar csx = math.cos[camera.angles.x];

	// The voxel coordinates
	rex_vec3s vox, pvox;

	// The screen space coordinates
	rex_vec2i s;

	#ifdef CRINGE

	// horrible inefficient renderer
	// but it works
	for (s.x = area.x1; s.x < area.x2; s.x++)
	{
		// variables
		rex_vec3s ray_dir;
		rex_vec3s side_dist;
		rex_vec3s delta_dist;
		rex_vec3i map_pos;
		rex_vec3i step;

		bool hit = false;

		rex_scalar rdz = REX_DIV(REX_SCALAR(2.0f), REX_SCALAR(draw_h));
		rex_scalar rz = REX_MUL(rdz, REX_SCALAR(2.0f));

		// map pos (int)
		map_pos.x = RexScalarToInteger(camera.origin.x);
		map_pos.y = RexScalarToInteger(camera.origin.y);
		map_pos.z = RexScalarToInteger(camera.origin.z);

		// calculate ray direction
		ray_dir.x = REX_MUL(REX_DIV(REX_SCALAR(2.0f), REX_SCALAR(draw_w)), REX_SCALAR(s.x)) - REX_SCALAR(1.0f);
		ray_dir.y = REX_SCALAR(1.0f);

		// rotate around (0, 0) by camera yaw
		rex_vec3s temp = ray_dir;

		ray_dir.x = REX_MUL(-temp.x, cs) - REX_MUL(-temp.y, sn);
		ray_dir.y = REX_MUL(temp.x, sn) + REX_MUL(temp.y, cs);

		// get delta of ray
		delta_dist.x = (ray_dir.x == 0) ? REX_SCALAR_MIN : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.x));
		delta_dist.y = (ray_dir.y == 0) ? REX_SCALAR_MIN : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.y));

		// calculate step and initial side_dist
		if (ray_dir.x < 0)
		{
			step.x = -1;
			side_dist.x = REX_MUL((camera.origin.x - REX_SCALAR(map_pos.x)), delta_dist.x);
		}
		else
		{
			step.x = 1;
			side_dist.x = REX_MUL((REX_SCALAR(map_pos.x) + REX_SCALAR(1) - camera.origin.x), delta_dist.x);
		}

		if (ray_dir.y < 0)
		{
			step.y = -1;
			side_dist.y = REX_MUL((camera.origin.y - REX_SCALAR(map_pos.y)), delta_dist.y);
		}
		else
		{
			step.y = 1;
			side_dist.y = REX_MUL((REX_SCALAR(map_pos.y) + REX_SCALAR(1) - camera.origin.y), delta_dist.y);
		}

		// perform flat DDA
		while (hit == false)
		{
			if (side_dist.x < side_dist.y)
			{
				side_dist.x += delta_dist.x;
				map_pos.x += step.x;
			}
			else
			{
				side_dist.y += delta_dist.y;
				map_pos.y += step.y;
			}

			// if out of bounds, stop the ray
			if (map_pos.x > (VOXMAP_X - 1) || map_pos.x < 0) break;
			if (map_pos.y > (VOXMAP_Y - 1) || map_pos.y < 0) break;

			// vertical march
			for (s.y = area.y1; s.y < area.y2; s.y++)
			{
				map_pos.z += 1;

				if (map_pos.z > (VOXMAP_Z - 1) || map_pos.z < 0)
				{
					hit = true;
					break;
				}

				// voxel at this coordinate
				voxel_t vox = voxmap[map_pos.z][map_pos.y][map_pos.x];

				if (vox.density > 0)
				{
					Rex::SurfaceDrawPixel(dst, s.x, s.y, vox.color);
					hit = true;
				}
			}
		}
	}

	#endif

	// horrible inefficient renderer
	// but it works
	for (s.x = area.x1 + 1; s.x < area.x2; s.x += 2)
	{
		for (s.y = area.y1 + 1; s.y < area.y2; s.y += 2)
		{
			// variables
			rex_vec3s ray_dir;
			rex_vec3s side_dist;
			rex_vec3s delta_dist;
			rex_vec3i map_pos;
			rex_vec3i step;

			bool hit = false;

			// map pos (int)
			map_pos.x = RexScalarToInteger(camera.origin.x);
			map_pos.y = RexScalarToInteger(camera.origin.y);
			map_pos.z = RexScalarToInteger(camera.origin.z);

			// calculate ray direction
			ray_dir.x = REX_MUL(REX_DIV(REX_SCALAR(2.0f), REX_SCALAR(draw_w)), REX_SCALAR(s.x)) - REX_SCALAR(1.0f);
			ray_dir.y = REX_SCALAR(1.0f);
			ray_dir.z = REX_MUL(REX_DIV(REX_SCALAR(2.0f), REX_SCALAR(draw_h)), REX_SCALAR(-s.y)) + REX_SCALAR(1.0f);

			// rotate around (0, 0) by camera yaw
			rex_vec3s temp = ray_dir;

			ray_dir.x = REX_MUL(-temp.x, cs) - REX_MUL(-temp.y, sn);
			ray_dir.y = REX_MUL(temp.x, sn) + REX_MUL(temp.y, cs);
			//ray_dir.z = REX_MUL(ray_dir.z, csx) + REX_MUL(ray_dir.z, snx);

			// get delta of ray
			delta_dist.x = (ray_dir.x == 0) ? REX_SCALAR_MAX : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.x));
			delta_dist.y = (ray_dir.y == 0) ? REX_SCALAR_MAX : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.y));
			delta_dist.z = (ray_dir.z == 0) ? REX_SCALAR_MAX : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.z));

			// calculate step and initial side_dist
			if (ray_dir.x < 0)
			{
				step.x = -1;
				side_dist.x = REX_MUL((camera.origin.x - REX_SCALAR(map_pos.x)), delta_dist.x);
			}
			else
			{
				step.x = 1;
				side_dist.x = REX_MUL((REX_SCALAR(map_pos.x) + REX_SCALAR(1) - camera.origin.x), delta_dist.x);
			}

			if (ray_dir.y < 0)
			{
				step.y = -1;
				side_dist.y = REX_MUL((camera.origin.y - REX_SCALAR(map_pos.y)), delta_dist.y);
			}
			else
			{
				step.y = 1;
				side_dist.y = REX_MUL((REX_SCALAR(map_pos.y) + REX_SCALAR(1) - camera.origin.y), delta_dist.y);
			}

			if (ray_dir.z < 0)
			{
				step.z = -1;
				side_dist.z = REX_MUL((camera.origin.z - REX_SCALAR(map_pos.z)), delta_dist.z);
			}
			else
			{
				step.z = 1;
				side_dist.z = REX_MUL((REX_SCALAR(map_pos.z) + REX_SCALAR(1) - camera.origin.z), delta_dist.z);
			}

			rex_int side;

			// perform DDA
			while (hit == false)
			{
				if (side_dist.x < side_dist.y)
				{
					if (side_dist.x < side_dist.z)
					{
						side_dist.x += delta_dist.x;
						map_pos.x += step.x;
						side = 1;
					}
					else
					{
						side_dist.z += delta_dist.z;
						map_pos.z += step.z;
						side = 2;
					}
				}
				else
				{
					if (side_dist.y < side_dist.z)
					{
						side_dist.y += delta_dist.y;
						map_pos.y += step.y;
						side = 3;
					}
					else
					{
						side_dist.z += delta_dist.z;
						map_pos.z += step.z;
						side = 4;
					}
				}

				rex_scalar dist;

				switch (side)
				{
					case 1: dist = (side_dist.x - delta_dist.x); break;

					case 2: dist = (side_dist.z - delta_dist.z); break;

					case 3: dist = (side_dist.y - delta_dist.y); break;

					case 4: dist = (side_dist.z - delta_dist.z); break;

					default: break;
				}

				if (dist > camera.draw_distance) break;

				// if out of bounds, keep going
				if (map_pos.x > (VOXMAP_X - 1) || map_pos.x < 0) continue;
				if (map_pos.y > (VOXMAP_Y - 1) || map_pos.y < 0) continue;
				if (map_pos.z > (VOXMAP_Z - 1) || map_pos.z < 0) continue;

				// voxel at this coordinate
				voxel_t vox = voxmap[map_pos.z][map_pos.y][map_pos.x];

				if (vox.density > 0)
				{
					rex_uint8 c = Rex::ColormapLookup(vox.color, RexScalarToInteger(dist));
					//rex_uint8 c = vox.color;
					//Rex::SurfaceDrawRectangle(dst, s.x - 1, s.y - 1, 2, 2, vox.color, false);
					Rex::SurfaceDrawPixel(dst, s.x - 1, s.y - 1, c);
					Rex::SurfaceDrawPixel(dst, s.x - 1, s.y, c);
					Rex::SurfaceDrawPixel(dst, s.x, s.y - 1, c);
					Rex::SurfaceDrawPixel(dst, s.x, s.y, c);
					hit = true;
				}
			}
		}
	}
}

//
// Voxels
//

void VoxelLoadMap1()
{
	Rex::SetGraphicsPalette("voxel/m1.pal");
	f_heightmap = (uint8_t *)calloc(1024 * 1024, sizeof(uint8_t));
	f_colormap = (uint8_t *)calloc(1024 * 1024, sizeof(uint8_t));

	// Load heightmap
	FILE *hei = fopen("voxel/m1h.dat", "rb");

	fread(f_heightmap, sizeof(uint8_t), 1024 * 1024, hei);

	fclose(hei);

	// Load colormap
	FILE *col = fopen("voxel/m1c.dat", "rb");

	fread(f_colormap, sizeof(uint8_t), 1024 * 1024, col);

	fclose(col);
}

void VoxelLoadMap2()
{
	//
	// Floor
	//

	// Load heightmap
	f_heightmap = (uint8_t *)calloc(4096, sizeof(uint8_t));

	FILE *f_hei = fopen("voxel/m2fh.dat", "rb");

	fread(f_heightmap, sizeof(uint8_t), 4096, f_hei);

	fclose(f_hei);

	// Load colormap
	f_colormap = (uint8_t *)calloc(4096, sizeof(uint8_t));

	FILE *f_col = fopen("voxel/m2fc.dat", "rb");

	fread(f_colormap, sizeof(uint8_t), 4096, f_col);

	fclose(f_col);

	//
	// Ceiling
	//

	// Load heightmap
	c_heightmap = (uint8_t *)calloc(4096, sizeof(uint8_t));

	FILE *c_hei = fopen("voxel/m2ch.dat", "rb");

	fread(c_heightmap, sizeof(uint8_t), 4096, c_hei);

	fclose(c_hei);

	// Load colormap
	c_colormap = (uint8_t *)calloc(4096, sizeof(uint8_t));

	FILE *c_col = fopen("voxel/m2cc.dat", "rb");

	fread(c_colormap, sizeof(uint8_t), 4096, c_col);

	fclose(c_col);
}

void VoxelLoadMap3()
{
	f_heightmap = (uint8_t *)calloc(150 * 150, sizeof(uint8_t));
	f_colormap = (uint8_t *)calloc(150 * 150, sizeof(uint8_t));

	// Load heightmap
	FILE *hei = fopen("voxel/m3fh.dat", "rb");

	fread(f_heightmap, sizeof(uint8_t), 150 * 150, hei);

	fclose(hei);

	// Load colormap
	FILE *col = fopen("voxel/m3fc.dat", "rb");

	fread(f_colormap, sizeof(uint8_t), 150 * 150, col);

	fclose(col);
}

void VoxelLoadCasino()
{
	f_heightmap = (uint8_t *)calloc(512 * 512, sizeof(uint8_t));
	f_colormap = (uint8_t *)calloc(512 * 512, sizeof(uint8_t));

	// Load heightmap
	FILE *hei = fopen("voxel/casino1h.dat", "rb");

	fread(f_heightmap, sizeof(uint8_t), 512 * 512, hei);

	fclose(hei);

	// Load colormap
	FILE *col = fopen("voxel/casino1c.dat", "rb");

	fread(f_colormap, sizeof(uint8_t), 512 * 512, col);

	fclose(col);
}

void VoxelInit(rex_int32 screen_width)
{
	// Load the map from heightmap and colors
	//VoxelLoadMap1();
	//VoxelLoadMap2();
	//VoxelLoadMap3();

	VoxelLoadCasino();

	// Build y-buffer
	ybuffer = (int32_t *)calloc(screen_width, sizeof(int32_t));

	// Position (scalar units)
	camera.origin.x = REX_SCALAR(32);
	camera.origin.y = REX_SCALAR(32);
	camera.origin.z = REX_SCALAR(32);

	// Angle (degrees)
	camera.angles.x = 0; // pitch
	camera.angles.y = 0; // yaw
	camera.angles.z = 0; // roll

	// Draw distance (scalar units)
	camera.draw_distance = REX_SCALAR(256);
}

void VoxelShutdown()
{
	free(c_heightmap);
	free(c_colormap);
	free(f_heightmap);
	free(f_colormap);
	free(ybuffer);
}

void VoxelRender(Rex::Surface *dst, rex_rect area, rex_vec3s p, rex_int32 yaw, rex_int32 horizon, rex_scalar height_scale, rex_scalar draw_distance, bool ceiling, rex_vec2i map_size, uint8_t *colormap, uint8_t *heightmap)
{
	// Drawable area
	rex_int32 draw_w = area.x2 - area.x1;
	rex_int32 draw_h = area.y2 - area.y1;

	// sin and cos of yaw
	rex_scalar sn = math.sin[yaw];
	rex_scalar cs = math.cos[yaw];

	// Screen coords
	rex_vec2i s;

	// Initialize y-buffer
	for (rex_int i = 0; i < draw_w; i++)
		ybuffer[i] = draw_h;

	// More efficient renderer?
	for (s.x = area.x1; s.x < area.x2; s.x++)
	{
		rex_vec2s ray_dir, delta_dist, side_dist;
		rex_vec2i step, map_pos;

		// map pos (int)
		map_pos.x = RexScalarToInteger(p.x);
		map_pos.y = RexScalarToInteger(p.y);

		// calculate ray direction
		ray_dir.x = REX_MUL(REX_DIV(REX_SCALAR(2.0f), REX_SCALAR(draw_w)), REX_SCALAR(s.x)) - REX_SCALAR(1.0f);
		ray_dir.y = REX_SCALAR(1.0f);

		// rotate around (0, 0) by camera yaw
		rex_vec2s temp = ray_dir;

		ray_dir.x = REX_MUL(-temp.x, cs) - REX_MUL(-temp.y, sn);
		ray_dir.y = REX_MUL(temp.x, sn) + REX_MUL(temp.y, cs);

		// get delta of ray (prevent div by 0)
		delta_dist.x = (ray_dir.x == 0) ? REX_SCALAR_MAX : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.x));
		delta_dist.y = (ray_dir.y == 0) ? REX_SCALAR_MAX : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.y));

		// calculate step and initial side_dist
		if (ray_dir.x < 0)
		{
			step.x = -1;
			side_dist.x = REX_MUL((p.x - REX_SCALAR(map_pos.x)), delta_dist.x);
		}
		else
		{
			step.x = 1;
			side_dist.x = REX_MUL((REX_SCALAR(map_pos.x) + REX_SCALAR(1) - p.x), delta_dist.x);
		}

		if (ray_dir.y < 0)
		{
			step.y = -1;
			side_dist.y = REX_MUL((p.y - REX_SCALAR(map_pos.y)), delta_dist.y);
		}
		else
		{
			step.y = 1;
			side_dist.y = REX_MUL((REX_SCALAR(map_pos.y) + REX_SCALAR(1) - p.y), delta_dist.y);
		}

		bool hit = false;
		rex_int side;
		rex_scalar dist;

		rex_scalar highest_point;

		// perform DDA
		while (hit == false)
		{
			if (side_dist.x < side_dist.y)
			{
				side_dist.x += delta_dist.x;
				map_pos.x += step.x;
				side = 1;
			}
			else
			{
				side_dist.y += delta_dist.y;
				map_pos.y += step.y;
				side = 2;
			}

			switch (side)
			{
				case 1: dist = (side_dist.x - delta_dist.x); break;

				case 2: dist = (side_dist.y - delta_dist.y); break;

				default: break;
			}

			if (dist > camera.draw_distance) break;

			// if out of bounds, keep going
			if (map_pos.x > (map_size.x - 1) || map_pos.x < 0) continue;
			if (map_pos.y > (map_size.y - 1) || map_pos.y < 0) continue;

			if (dist > REX_SCALAR(1))
			{
				rex_scalar h = REX_SCALAR(heightmap[(map_pos.y * map_size.y) + map_pos.x]);

				rex_scalar dh = p.z - h;

				uint8_t c = colormap[(map_pos.y * map_size.y) + map_pos.x];

				rex_int32 line_height = RexScalarToInteger(REX_MUL(REX_DIV(dh, dist), height_scale)) + horizon;

				// clamp to visible region
				line_height = CLAMP(line_height, area.y1, area.y2);

				if (line_height < ybuffer[s.x])
				{
					//c = Rex::ColormapLookup(c, RexScalarToInteger(dist));
					Rex::SurfaceDrawVerticalLine(dst, s.x, line_height, ybuffer[s.x], c);
					 ybuffer[s.x] = line_height;
				}
			}
		}
	}

	#ifdef RAY_RENDERER

	// Z, and current change in Z
	rex_scalar z = REX_SCALAR(0.05f);
	rex_scalar dz = REX_SCALAR(0.5f);

	// Initialize y-buffer
	for (rex_int32 i = 0; i < draw_w; i++)
		ybuffer[i] = ceiling == true ? 0 : draw_h;

	rex_vec2i s;

	rex_vec2s pleft, pright;

	pleft.x = (REX_MUL(-cs, z) - REX_MUL(sn, z)) + p.x;
	pleft.y = (REX_MUL(sn, z) - REX_MUL(cs, z)) + p.y;

	pright.x = (REX_MUL(cs, z) - REX_MUL(sn, z)) + p.x;
	pright.y = (REX_MUL(-sn, z) - REX_MUL(cs, z)) + p.y;

	rex_scalar dx = REX_DIV(pright.x - pleft.x, REX_SCALAR(draw_w));
	rex_scalar dy = REX_DIV(pright.y - pleft.y, REX_SCALAR(draw_w));

	// Render front to back
	while (z < draw_distance)
	{
		rex_vec2s pleft, pright;

		pleft.x = (REX_MUL(-cs, z) - REX_MUL(sn, z)) + p.x;
		pleft.y = (REX_MUL(sn, z) - REX_MUL(cs, z)) + p.y;

		pright.x = (REX_MUL(cs, z) - REX_MUL(sn, z)) + p.x;
		pright.y = (REX_MUL(-sn, z) - REX_MUL(cs, z)) + p.y;

		rex_scalar dx = REX_DIV(pright.x - pleft.x, REX_SCALAR(draw_w));
		rex_scalar dy = REX_DIV(pright.y - pleft.y, REX_SCALAR(draw_w));

		for (rex_int32 sx = 0; sx < draw_w; sx++)
		{
			rex_vec2i pi;
			pi.x = RexScalarToInteger(pleft.x);
			pi.y = RexScalarToInteger(pleft.y);

			if (pi.x > (map_size.x - 1) || pi.x < 0 || pi.y > (map_size.y - 1) || pi.y < 0)
			{
				pleft.x += dx;
				pleft.y += dy;
				continue;
			}

			//if (pi.x > (map_size.x - 1)) pi.x -= map_size.x;
			//if (pi.x < 0) pi.x += map_size.x;
			//if (pi.y > (map_size.y - 1)) pi.y -= map_size.y;
			//if (pi.y < 0) pi.y += map_size.y;

			rex_scalar h = REX_SCALAR(heightmap[(pi.y * map_size.y) + pi.x]);
			rex_scalar dh = ceiling == true ? h - p.z : p.z - h;

			uint8_t c = colormap[(pi.y * map_size.y) + pi.x];

			rex_int32 line_height = RexScalarToInteger(REX_MUL(REX_DIV(dh, z), height_scale)) + horizon;

			if (line_height > draw_h) break;
			line_height = CLAMP(line_height, area.y1, area.y2);

			if ((ceiling == true && line_height > ybuffer[sx]) || (ceiling == false && line_height < ybuffer[sx]))
			{
				if (shading == true) c = Rex::ColormapLookup(c, RexScalarToInteger(z) / 8);
				Rex::SurfaceDrawVerticalLine(dst, sx, line_height, ybuffer[sx], c);
				ybuffer[sx] = line_height;
			}

			pleft.x += dx;
			pleft.y += dy;
		}

		z += dz;
		//dz += REX_SCALAR(0.02f);
	}

	#endif
}

void VoxelRenderWrapper(Rex::Surface *dst, rex_rect area)
{
	// Throw in some collision detection while we're at it
	rex_scalar minh = REX_SCALAR(32);
	if (camera.origin.z < minh) camera.origin.z = minh;

	// map 1 floor
	//rex_vec2i map_size = {1024, 1024};
	//VoxelRender(dst, area, camera.origin, camera.angles.y, camera.angles.x, REX_SCALAR(64), camera.draw_distance, false, map_size, f_colormap, f_heightmap);

	// map 2 floor
	//VoxelRender(dst, area, camera.origin, camera.angles.y, 100, REX_SCALAR(32), camera.draw_distance, false, VEC2I(64, 64), f_colormap, f_heightmap);

	// map 2 ceiling
	//VoxelRender(dst, area, camera.origin, camera.angles.y, 100, REX_SCALAR(32), camera.draw_distance, true, VEC2I(64, 64), c_colormap, c_heightmap);

	// map 3 floor
	//VoxelRender(dst, area, camera.origin, camera.angles.y, 100, REX_SCALAR(16), camera.draw_distance, false, VEC2I(150, 150), f_colormap, f_heightmap);

	// casino floor
	rex_vec2i map_size = {512, 512};
	VoxelRender(dst, area, camera.origin, camera.angles.y, 100, REX_SCALAR(128), camera.draw_distance, false, map_size, f_colormap, f_heightmap);
}

//
// Misc
//

// Mouse helper function
void ReadMouse(rex_int32 *buttons, rex_vec2i *pos, rex_int32 speedlimit, rex_rect area)
{
	rex_int32 mb, mx, my, dmx, dmy;
	rex_int32 halfx = 160, halfy = 100;

	Rex::MouseRead(&mb, &mx, &my);
	
	dmx = mx - halfx;
	dmy = my - halfy;

	pos->x += (dmx < 0) ? (dmx < -speedlimit ? -speedlimit : dmx) : (dmx > speedlimit ? speedlimit : dmx);
	pos->y += (dmy < 0) ? (dmy < -speedlimit ? -speedlimit : dmy) : (dmy > speedlimit ? speedlimit : dmy);

	// Clip mouse to area
	if (pos->x < area.x1) pos->x = area.x1;
	if (pos->x > area.x2) pos->x = area.x2;
	if (pos->y < area.y1) pos->y = area.y1;
	if (pos->y > area.y2) pos->y = area.y2;

	*buttons = mb;

	// 160x100 is the the middle of the 320x200 screen
	// (use this regardless of actual screen resolution)
	Rex::MouseSet(halfx, halfy);
}

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// General variables
	rex_int i;

	// Cycle variables
	rex_int64 frame_start, frame_end;
	rex_int32 cycles, c;

	// Picture buffers
	Rex::Surface pic_font;
	Rex::Surface pic_bbuffer;
	//Rex::Surface pic_background;
	Rex::Surface pic_cursor;

	// Initialize Rex Engine
	Rex::Initialize();

	// Initialize Graphics
	if (Rex::InitializeGraphics(320, 200, 8) == false) return EXIT_FAILURE;
	Rex::VidInfo vidinfo = Rex::GetVidInfo();

	// Load colormap
	Rex::SetGraphicsPalette("gfx/portal2d.pal");
	Rex::ColormapLoad("gfx/portal2d.tab");

	// Create picture buffers
	Rex::SurfaceLoadBMP(&pic_font, "gfx/font8x8.bmp");
	Rex::SurfaceLoadBMP(&pic_cursor, "local/cursor.bmp");
	Rex::SurfaceCreate(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);

	// Generate math table
	for (i = 0; i < 360; i++)
	{
		math.sin[i] = REX_SCALAR(sin(i / 180.0f * PI));
		math.cos[i] = REX_SCALAR(cos(i / 180.0f * PI));
		math.tan[i] = REX_SCALAR(tan(i / 180.0f * PI));
	}

	// V-ReX init
	VReXInit();

	// Initialize voxel stuff
	//VoxelInit(vidinfo.width);

	// Start counting time
	frame_end = Rex::GetTicks64();

	// Main loop
	while (!Rex::KeyTest(REX_KB_ESC))
	{
		// Get start of frame time
		frame_start = Rex::GetTicks64();

		#if (REX_COMPILER == COMPILER_DJGPP)
			cycles = CYCLES * (frame_start - frame_end) / UCLOCKS_PER_SEC;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			cycles = CYCLES * (frame_start - frame_end) / CLOCKS_PER_SEC;
		#endif

		// Cycles
		for (c = 0; c < cycles; c++)
		{
			//
			// User inputs
			//

			CameraController();

			if (Rex::KeyTest(REX_KB_F)) shading = !shading;
		}

		//
		// Rendering
		//

		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 0);

		// Voxels
		{
			// watcom...
			rex_rect screen_area = {0, 0, pic_bbuffer.width, pic_bbuffer.height};
			rex_vec3i voxmap_dim = {32, 32, 32};

			// V-ReX renderer
			VReXRender(&pic_bbuffer, screen_area);

			// Voxel renderer
			//VoxelRenderWrapper(&pic_bbuffer, screen_area);
		}

		sprintf(console_buffer, "x: %d y: %d z %d", RexScalarToInteger(camera.origin.x), RexScalarToInteger(camera.origin.y), RexScalarToInteger(camera.origin.z));
		Rex::ConsoleAddText(0, 0, console_buffer);
		sprintf(console_buffer, "pitch: %d yaw: %d roll %d", camera.angles.x, camera.angles.y, camera.angles.z);
		Rex::ConsoleAddText(0, 1, console_buffer);

		// Render the console text
		Rex::ConsoleRender(&pic_bbuffer, &pic_font, 8);

		// Flip the rendering buffers
		Rex::SurfaceToFrontBuffer(&pic_bbuffer);

		// Get end of frame time
		#if (REX_COMPILER == COMPILER_DJGPP)
			frame_end = frame_end + cycles * UCLOCKS_PER_SEC / CYCLES;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			frame_end = frame_end + cycles * CLOCKS_PER_SEC / CYCLES;
		#endif
	}

	// Shutdown Graphics
	Rex::ShutdownGraphics();

	// Shutdown Rex Engine
	Rex::Shutdown();

	// Cleanup memory
	Rex::SurfaceDestroy(&pic_font);
	Rex::SurfaceDestroy(&pic_bbuffer);
	//Rex::SurfaceDestroy(&pic_background);
	Rex::SurfaceDestroy(&pic_cursor);

	// Exit gracefully
	return EXIT_SUCCESS;
}
