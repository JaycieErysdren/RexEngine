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
// LAST EDITED:		December 14th, 2022
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

// Voxel RLE element
typedef struct
{
	rex_uint8 skipped;
	rex_uint8 drawn;
	rex_uint8 side_color;
	rex_uint8 slab_color;
} voxel_rle_element_t;

// Voxel RLE column
typedef struct
{
	int16_t num_elements;
	voxel_rle_element_t *elements;
} voxel_rle_column_t;

//
// Globals
//

#define VOXMAP_RLE_X 1024
#define VOXMAP_RLE_Y 1024
#define VOXMAP_RLE_Z 256

// V-ReX
voxel_rle_column_t *voxmap;
rex_uint8 *ybuff;

// Camera
camera_t camera;

// Math tables
math_t math;

// Console buffer
char console_buffer[256];

//
// V-ReX
//

// Allocate world pointermap
void VReX_AllocateWorld()
{
	voxmap = (voxel_rle_column_t *)calloc(VOXMAP_RLE_X * VOXMAP_RLE_Y, sizeof(voxel_rle_column_t));
}

rex_uint8 *geobuffer;
rex_uint8 *colbuffer;

void setgeom(rex_int32 x, rex_int32 y, rex_int32 z, rex_int32 is_solid)
{
	geobuffer[(z * 64) + (y * 512) + x] = (rex_uint8)(is_solid);
}

void setcolor(rex_int32 x, rex_int32 y, rex_int32 z, rex_int32 color)
{
	colbuffer[(z * 64) + (y * 512) + x] = (rex_uint8)(color);
}

void buffers_to_vrex()
{
	// variables
	rex_int x = 0, y = 0, z = 0, i = 0;

	// father, i require loupes
	for (y = 0; y < 512; y++)
	{
		for (x = 0; x < 512; x++)
		{
			bool generating = true;

			// generate elements for this column
			while (generating == true)
			{
				rex_int skipped = 0;
				rex_int drawn = 0;
				voxel_rle_element_t *e;

				// determine air run
				while (geobuffer[(z * 64) + (y * 512) + x] == 0)
				{
					skipped++;
					z++;

					if (z > 63) break;
				}

				// determine drawn run
				while (geobuffer[(z * 64) + (y * 512) + x] == 1)
				{
					drawn++;
					z++;

					if (z > 63) break;
				}
			}
		}
	}
}

// VXL loader
void VReX_LoadVXL(string filename, bool aos)
{
	// General variables
	rex_int x, y, z, i;
	rex_int file_len;
	rex_uint8 *file_buffer;
	rex_uint8 *v, *v_base;
	FILE *file;

	// tell the user we're doing something
	printf("loading...\n");

	if (aos == true)
	{
		// open file
		file = fopen(filename.c_str(), "rb");
		if (file == NULL) return;

		// seek to end
		fseek(file, 0L, SEEK_END);

		// get file size
		file_len = ftell(file);

		// seek to start
		fseek(file, 0L, SEEK_SET);

		// allocate memory for file buffer
		file_buffer = (rex_uint8 *)calloc(1, file_len);

		// read in file data
		fread(file_buffer, file_len, 1, file);

		// close file
		fclose(file);

		// allocate temporary buffers
		geobuffer = (rex_uint8 *)calloc(1, 512 * 512 * 64);
		colbuffer = (rex_uint8 *)calloc(1, 512 * 512 * 64);

		v_base = file_buffer;
		v = file_buffer;

		for (y = 0; y < 512; y++)
		{
			for (x = 0; x < 512; x++)
			{
				for (z = 0; z < 64; z++)
				{
					setgeom(x, y, z, 1);
				}

				z = 0;
				for(;;)
				{
					rex_uint32 *color;
					int i;
					int number_4byte_chunks = v[0];
					int top_color_start = v[1];
					int top_color_end  = v[2]; // inclusive
					int bottom_color_start;
					int bottom_color_end; // exclusive
					int len_top;
					int len_bottom;

					for(i=z; i < top_color_start; i++)
					{
						setgeom(x, y, i, 0);
					}

					color = (rex_uint32 *)(v+4);

					for(z=top_color_start; z <= top_color_end; z++)
					{
						setcolor(x, y, z, *color++);
					}

					len_bottom = top_color_end - top_color_start + 1;

					// check for end of data marker
					if (number_4byte_chunks == 0)
					{
						// infer ACTUAL number of 4-byte chunks from the length of the color data
						v += 4 * (len_bottom + 1);
						break;
					}

					// infer the number of bottom colors in next span from chunk length
					len_top = (number_4byte_chunks-1) - len_bottom;

					// now skip the v pointer past the data to the beginning of the next span
					v += v[0]*4;

					bottom_color_end  = v[3]; // aka air start
					bottom_color_start = bottom_color_end - len_top;

					for (z = bottom_color_start; z < bottom_color_end; z++)
					{
						setcolor(x, y, z, *color++);
					}
				}
			}
		}

		buffers_to_vrex();

		// free memory
		if (file_buffer) free(file_buffer);
		if (geobuffer) free(geobuffer);
		if (colbuffer) free(colbuffer);
	}
}

// KV6 loader
void VReX_LoadKV6(string filename)
{
	// General variables
	rex_int x, y, i;
	FILE *kv6;

	// KV6 variables
	char kv6_magic[5] = "Kvxl";
	char magic[4];
	rex_int32 xsize, ysize, zsize;
	rex_int32 num_surface_voxels;

	// open the file
	kv6 = fopen(filename.c_str(), "rb");
	if (kv6 == NULL) return;

	// read in magic
	fread(&magic, sizeof(rex_uint8), 4, kv6);

	// compare magic
	if (memcmp(kv6_magic, magic, 4) != 0) return;

	// read in size vlaues
	fread(&xsize, sizeof(rex_int32), 1, kv6);
	fread(&ysize, sizeof(rex_int32), 1, kv6);
	fread(&zsize, sizeof(rex_int32), 1, kv6);

	// skip the pivot, we don't need it
	fseek(kv6, 12, SEEK_CUR);

	// get number of surface voxels
	fread(&num_surface_voxels, sizeof(rex_int32), 1, kv6);

	// skip the surface voxels
	fseek(kv6, num_surface_voxels * 8, SEEK_CUR);

	// read in the x plane info
	rex_uint32 xlen[xsize];

	for(x = 0; x < xsize; x++)
	{
		fread(&xlen[x], sizeof(rex_uint32), 1, kv6);
	}

	// read in x,y column information
	rex_uint16 ylen[xsize][ysize];

	for(x = 0; x < xsize; x++)
	{
		for(y = 0; y < ysize; y++)
		{
			fread(&ylen[x][y], sizeof(rex_uint16), 1, kv6);
		}
	}

	// create columns
	for(x = 0; x < xsize; x++)
	{
		for(y = 0; y < ysize; y++)
		{
			voxel_rle_element_t *e = (voxel_rle_element_t *)calloc(1, sizeof(voxel_rle_element_t));

			e->skipped = 0;
			e->drawn = ylen[x][y];
			e->side_color = 31;
			e->slab_color = 15;

			voxmap[(y * VOXMAP_RLE_Y) + x].elements = e;
			voxmap[(y * VOXMAP_RLE_Y) + x].num_elements = 1;
		}
	}

	// close the file
	fclose(kv6);
}

// KVX loader
typedef struct
{
	rex_uint8 ztop;
	rex_uint8 zleng;
	rex_uint8 cullinfo;
	rex_uint8 color;
} slab_t;

void VReX_LoadKVX(string filename)
{
	// general variables
	rex_int x, y, i;
	FILE *file;

	// kvx variables
	rex_int num_mips = 1;
	rex_int32 len_mip;
	rex_int32 xsize, ysize, zsize;

	// open the file
	file = fopen(filename.c_str(), "rb");

	for (i = 0; i < num_mips; i++)
	{
		// size of mip after this point (in bytes)
		fread(&len_mip, sizeof(rex_int32), 1, file);

		// size boundaries of voxel model
		fread(&xsize, sizeof(rex_int32), 1, file);
		fread(&ysize, sizeof(rex_int32), 1, file);
		fread(&zsize, sizeof(rex_int32), 1, file);

		// skip the pivot
		fseek(file, sizeof(rex_int32) * 3, SEEK_CUR);

		// x offsets, xy offsets and raw voxel data
		rex_int len_xoffsets = xsize + 1;
		rex_int len_xyoffsets = xsize * (ysize + 1);
		rex_int len_voxdata = len_mip - 24 - (len_xoffsets * sizeof(rex_int32)) - (len_xyoffsets * sizeof(rex_int16));

		rex_int32 xoffsets[len_xoffsets];
		rex_int16 xyoffsets[xsize][ysize + 1];
		rex_int8 voxdata[len_voxdata];

		fread(&xoffsets, sizeof(rex_int32), len_xoffsets, file);
		fread(&xyoffsets, sizeof(rex_int16), len_xyoffsets, file);
		fread(&voxdata, sizeof(rex_int8), len_voxdata, file);

		// spool in the data
		for (y = 0; y < ysize; y++)
		{
			for (x = 0; x < xsize; x++)
			{
				slab_t *start = (slab_t *)&voxdata[xoffsets[x] + xyoffsets[x][y]];
				slab_t *end = (slab_t *)&voxdata[xoffsets[x] + xyoffsets[x][y + 1]];

				voxel_rle_element_t *e = (voxel_rle_element_t *)calloc(1, sizeof(voxel_rle_element_t));

				e->skipped = start->ztop;
				e->drawn = start->zleng;
				e->side_color = start->color;
				e->slab_color = start->color;

				voxmap[(y * VOXMAP_RLE_Y) + x].num_elements = 1;
				voxmap[(y * VOXMAP_RLE_Y) + x].elements = e;
			}
		}
	}

	// close the file
	fclose(file);
}

// Heightmap loader
void VReX_LoadHeightmap(string filename_color, string filename_height, rex_int size_x, rex_int size_y)
{
	// Variables
	rex_int x, y, i;

	printf("loading...\n");

	FILE *hei = fopen(filename_height.c_str(), "rb");
	FILE *col = fopen(filename_color.c_str(), "rb");

	for (y = 0; y < size_y; y++)
	{
		for (x = 0; x < size_x; x++)
		{
			voxel_rle_element_t *e = (voxel_rle_element_t *)calloc(1, sizeof(voxel_rle_element_t));

			rex_uint8 height = fgetc(hei);
			rex_uint8 color = fgetc(col);

			e->drawn = height;
			e->skipped = 255 - height;
			e->slab_color = color;
			e->side_color = color - 2;

			voxmap[(y * VOXMAP_RLE_Y) + x].num_elements = 1;
			voxmap[(y * VOXMAP_RLE_Y) + x].elements = e;
		}
	}

	fclose(hei);
	fclose(col);
}

void VReX_Init()
{
	Rex::SetGraphicsPalette("gfx/mindgrdn.pal");
	Rex::ColormapLoad("gfx/mindgrdn.tab");

	// allocate pointermap
	VReX_AllocateWorld();

	//VReX_LoadKV6("voxel/block.kv6");

	VReX_LoadHeightmap("voxel/m11.col", "voxel/m11.hei", 1024, 1024);
	//VReX_LoadKVX("voxel/desklamp.kvx");
	//if (loadvxl("voxel/untitled.vxl") == -1) exit(1);

	//VReX_LoadVXL("voxel/babel.vxl", true);

	// camera
	camera.draw_distance = REX_SCALAR(128);

	camera.origin.x = REX_SCALAR(0);
	camera.origin.y = REX_SCALAR(0);
	camera.origin.z = REX_SCALAR(255);

	camera.angles.x = 0;
	camera.angles.y = 0;
	camera.angles.z = 0;

	// y buffer
	ybuff = (rex_uint8 *)calloc(200, sizeof(rex_uint8));
}

void VReX_Shutdown()
{
	if (voxmap->elements) free(voxmap->elements);
	if (voxmap) free(voxmap);
	if (ybuff) free(ybuff);
}

void VReX_Render(Rex::Surface *dst, rex_rect area, camera_t cam, rex_scalar height_scale)
{
	// General variables
	rex_int i;

	// Drawable area
	rex_int32 draw_w = area.x2 - area.x1;
	rex_int32 draw_h = area.y2 - area.y1;

	// Sin and cos of the camera's yaw
	rex_scalar sn = math.sin[cam.angles.y];
	rex_scalar cs = math.cos[cam.angles.y];

	// Screen coords
	rex_vec2i s;

	// meh
	rex_vec3s p = cam.origin;

	// meh
	rex_int horizon = -cam.angles.x + (draw_h / 2);

	// Draw left to right
	for (s.x = area.x1; s.x < area.x2; s.x++)
	{
		// variables
		rex_vec2s ray_dir, delta_dist, side_dist;
		rex_vec2i step, map_pos;

		// clear y-buffer
		memset(ybuff, 0, 200);

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
		delta_dist.x = (ray_dir.x == 0) ? REX_SCALAR_MIN : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.x));
		delta_dist.y = (ray_dir.y == 0) ? REX_SCALAR_MIN : ABS(REX_DIV(REX_SCALAR(1.0f), ray_dir.y));

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

		bool casting = true;
		rex_int side;
		rex_scalar dist, dist2;
		rex_int r;

		// perform DDA
		while (casting == true)
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
				case 1: dist = side_dist.x - delta_dist.x; dist2 = side_dist.y; break;

				case 2: dist = side_dist.y - delta_dist.y; dist2 = side_dist.x; break;

				default: break;
			}

			// if it goes beyond the draw distance, cut off the ray
			if (dist > cam.draw_distance) break;

			// if out of bounds, keep going in hopes of finding something in-bounds again
			// this allows rendering the map from an out of bounds location
			if (map_pos.x > (VOXMAP_RLE_X - 1) || map_pos.x < 0) continue;
			if (map_pos.y > (VOXMAP_RLE_Y - 1) || map_pos.y < 0) continue;

			if (dist > REX_SCALAR(1) && dist2 > REX_SCALAR(1))
			{
				voxel_rle_column_t column = voxmap[(map_pos.y * VOXMAP_RLE_Y) + map_pos.x];

				rex_int column_height = 256;

				rex_vec3s element_pos = {0, 0, 0};
				rex_scalar element_height = 0;

				rex_int line_height = 0;
				rex_int line_start = 0, line_end = 0;
				rex_int line_start2 = 0, line_end2 = 0;
				rex_scalar height_delta1 = 0, height_delta2 = 0;

				// draw the elements from top to bottom
				for (i = 0; i < column.num_elements; i++)
				{
					voxel_rle_element_t element = column.elements[i];

					// position of element
					element_pos.x = REX_SCALAR(map_pos.x);
					element_pos.y = REX_SCALAR(map_pos.y);
					element_pos.z = REX_SCALAR(column_height - element.skipped);

					// height of element
					element_height = REX_SCALAR(element.drawn);

					// height delta 1
					height_delta1 = p.z - element_pos.z;
					height_delta2 = p.z - (element_pos.z - element_height);

					// height of the line on screen
					line_start = RexScalarToInteger(REX_MUL(REX_DIV(height_delta1, dist), height_scale)) + horizon;
					line_end = RexScalarToInteger(REX_MUL(REX_DIV(height_delta2, dist), height_scale)) + horizon;

					// clamp the line to the visible region
					line_start = CLAMP(line_start, area.y1, area.y2);
					line_end = CLAMP(line_end, area.y1, area.y2);

					// draw the side of the voxel
					for (s.y = line_start; s.y < line_end; s.y++)
					{
						if (ybuff[s.y] == 0)
						{
							rex_uint8 c = element.side_color;
							//c = Rex::ColormapLookup(c, RexScalarToInteger(dist));
							Rex::SurfaceDrawPixel(dst, s.x, s.y, c);
							ybuff[s.y] = 1;
						}
					}

					// draw the top or bottom slab of the voxel
					{
						// this is where the magic happens
						if (dist2 > (dist + REX_SCALAR(1)))
							dist2 = (dist + REX_SCALAR(1));

						line_start2 = RexScalarToInteger(REX_MUL(REX_DIV(height_delta1, dist2), height_scale)) + horizon;
						line_end2 = RexScalarToInteger(REX_MUL(REX_DIV(height_delta2, dist2), height_scale)) + horizon;

						line_start2 = CLAMP(line_start2, area.y1, area.y2);
						line_end2 = CLAMP(line_end2, area.y1, area.y2);

						for (s.y = line_start2; s.y < line_end2; s.y++)
						{
							if (ybuff[s.y] == 0)
							{
								rex_uint8 c = element.slab_color;
								//c = Rex::ColormapLookup(c, RexScalarToInteger(dist));
								Rex::SurfaceDrawPixel(dst, s.x, s.y, c);
								ybuff[s.y] = 1;
							}
						}
					}

					// overall height of this column
					column_height -= (element.skipped + element.drawn);

					// gotta set a max height somewhere i guess
					if (column_height < 0) break;
				}
			}
		}
	}
}

//
// Misc
//

// Camera controller
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
	//Rex::SetGraphicsPalette("gfx/portal2d.pal");
	//Rex::ColormapLoad("gfx/portal2d.tab");

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
	VReX_Init();

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
		}

		//
		// Rendering
		//

		// Clear back buffer
		Rex::SurfaceClear(&pic_bbuffer, 241);

		// Voxels
		{
			// watcom...
			rex_rect screen_area = {0, 0, pic_bbuffer.width, pic_bbuffer.height};
			//rex_vec3i voxmap_dim = {32, 32, 32};

			// V-ReX renderer
			VReX_Render(&pic_bbuffer, screen_area, camera, REX_SCALAR(160));

			#ifdef SPIVIS2_KC

			rex_vec3f pp = {0, 0, 0};
			rex_vec3f pr = {1, 0, 0};
			rex_vec3f pd = {0, 0, 1};
			rex_vec3f pf = {0, -1, 0};
			rex_vec3f ph = {(float)(pic_bbuffer.width / 2), (float)(pic_bbuffer.height / 2), (float)(pic_bbuffer.width / 2)};

			VoxelRender2(&pic_bbuffer, screen_area, pp, pr, pd, pf, ph);
		
			#endif
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

	// Shutdown Voxel RLE
	VReX_Shutdown();

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
