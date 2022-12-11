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

// Math table
typedef struct
{
	scalar_t cos[360];
	scalar_t sin[360];
	scalar_t tan[360];
} math_t;

// Camera
typedef struct
{
	vec3s_t origin;				// X, Y, Z
	vec3s_t velocity;			// X, Y, Z
	vec3i_t angles;				// Pitch, yaw, roll
	scalar_t draw_distance;		// Draw distance (scalar units)
	int movespeedkey;
	int anglespeedkey;
} camera_t;

// Voxel
typedef struct
{
	uint8_t height;
	uint8_t color;
} voxel_t;

// VoxCave game state
typedef struct
{
	int32_t horizon;
} voxcave_t;

//
// Globals
//

// VoxCave
voxel_t map1[256][256]; // cave map 1
voxel_t map2[256][256]; // cave map 2
int16_t *startumost; // ??
int16_t *startdmost; // ??
voxcave_t voxcave; // voxcave game state

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

//
// Camera
//

void CameraController()
{
	// Mouse read
	static int16_t mx_prev, my_prev;
	int16_t delta_mx, delta_my;
	int16_t mb, mx, my;
	mb = DOS::MouseRead(&mx, &my);

	delta_mx = mx_prev - mx;
	delta_my = my_prev - my;

	// Mouse look
	if (mb == 1 && delta_mx != 0) camera.angles.y += delta_mx;
	if (mb == 2 && delta_my != 0) camera.angles.x += delta_my;

	// Reset pitch
	if (mb == 3)
		camera.angles.x = 100;

	// Keyboard look
	{
		// Rotate leftwards
		if (DOS::KeyTest(KB_LTARROW)) camera.angles.y += camera.anglespeedkey;

		// Rotate rightwards
		if (DOS::KeyTest(KB_RTARROW)) camera.angles.y -= camera.anglespeedkey;

		// Look upwards
		if (DOS::KeyTest(KB_UPARROW)) camera.angles.x += camera.anglespeedkey;

		// Look downwards
		if (DOS::KeyTest(KB_DNARROW)) camera.angles.x -= camera.anglespeedkey;
	}

	// Pitch angle sanity checks
	if (camera.angles.x < 0) camera.angles.x = 0;
	if (camera.angles.x > 200) camera.angles.x = 200;

	// Yaw angle sanity checks
	if (camera.angles.y < 0) camera.angles.y += 360;
	if (camera.angles.y > 359) camera.angles.y -= 360;

	// Check if sprinting
	if (DOS::KeyTest(KB_LTSHIFT))
		camera.movespeedkey = 4;
	else
		camera.movespeedkey = 2;

	// Set velocity
	camera.velocity.x = math.sin[camera.angles.y] * camera.movespeedkey;
	camera.velocity.y = math.cos[camera.angles.y] * camera.movespeedkey;
	camera.velocity.z = SCALAR(1.0f) * camera.movespeedkey;

	// Move forwards
	if (DOS::KeyTest(KB_W))
	{
		camera.origin.x -= camera.velocity.x;
		camera.origin.y -= camera.velocity.y;
	}

	// Move backwards
	if (DOS::KeyTest(KB_S))
	{
		camera.origin.x += camera.velocity.x;
		camera.origin.y += camera.velocity.y;
	}

	// Move leftwards
	if (DOS::KeyTest(KB_A))
	{
		camera.origin.x -= camera.velocity.y;
		camera.origin.y += camera.velocity.x;
	}

	// Move rightwards
	if (DOS::KeyTest(KB_D))
	{
		camera.origin.x += camera.velocity.y;
		camera.origin.y -= camera.velocity.x;
	}

	// Move upwards
	if (DOS::KeyTest(KB_Q))
		camera.origin.z += camera.velocity.z;

	// Move downwards
	if (DOS::KeyTest(KB_E))
		camera.origin.z -= camera.velocity.z;

	mx_prev = mx;
	my_prev = my;
}

//
// VoxCave
//

void VoxCave_Init(vec2i_t screen_dimensions)
{
	// Set starter values
	camera.origin.x = SCALAR(128);
	camera.origin.y = SCALAR(128);
	camera.origin.z = SCALAR(128);

	voxcave.horizon = screen_dimensions.y / 2;

	// Generate a cave
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			map1[y][x].height = 255;
			map1[y][x].color = 128;

			map2[y][x].height = 0;
			map2[y][x].color = 128;
		}
	}

	// Setup view buffers
	startumost = (int16_t *)calloc(screen_dimensions.x, sizeof(int16_t));
	startdmost = (int16_t *)calloc(screen_dimensions.x, sizeof(int16_t));

	// Initialize startdmost to the screen height
	for (int i = 0; i < screen_dimensions.x; i++)
	{
		startdmost[i] = screen_dimensions.y;
	}
}

void VoxCave_RenderColumn(Picture::pic_t *dst, rect_t area, int column, scalar_t draw_distance)
{
	// sanity check
	if (startumost[column] > startdmost[column]) return;

	// drawable area
	int draw_w = area.x2 - area.x1;
	int draw_h = area.y2 - area.y1;

	// sin and cos of camera yaw
	scalar_t sn = math.sin[camera.angles.y];
	scalar_t cs = math.cos[camera.angles.y];
}

void VoxCave_Render(Picture::pic_t *dst, rect_t area)
{
	// Draw loop
	for (int sx = area.x1; sx < area.x1; sx++)
	{
		VoxCave_RenderColumn(dst, area, sx, camera.draw_distance);
	}	
}

//
// Voxels
//

void VoxelLoadMap1()
{
	f_heightmap = (uint8_t *)calloc(1024 * 1024, sizeof(uint8_t));
	f_colormap = (uint8_t *)calloc(1024 * 1024, sizeof(uint8_t));

	// Load heightmap
	FILE *hei = fopen("voxel/m1h.dat", "rb");

	fread(f_heightmap, sizeof(uint8_t), 1024 * 1024, hei);

	fclose(hei);

	// Load colormap
	FILE *col = fopen("voxel/m1c_vga.dat", "rb");

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

void VoxelInit(int screen_width)
{
	// Load the map from heightmap and colors
	//VoxelLoadMap1();
	//VoxelLoadMap2();
	//VoxelLoadMap3();

	VoxelLoadCasino();

	// Build y-buffer
	ybuffer = (int32_t *)calloc(screen_width, sizeof(int32_t));

	// Position (scalar units)
	camera.origin.x = SCALAR(32);
	camera.origin.y = SCALAR(32);
	camera.origin.z = SCALAR(32);

	// Angle (degrees)
	camera.angles.x = 100; // pitch
	camera.angles.y = 0; // yaw
	camera.angles.z = 0; // roll

	// Draw distance (scalar units)
	camera.draw_distance = SCALAR(256);
}

void VoxelShutdown()
{
	free(c_heightmap);
	free(c_colormap);
	free(f_heightmap);
	free(f_colormap);
	free(ybuffer);
}

void VoxelRender(Picture::pic_t *dst, rect_t area, vec3s_t p, int yaw, int horizon, scalar_t height_scale, scalar_t draw_distance, bool ceiling, vec2i_t map_size, uint8_t *colormap, uint8_t *heightmap)
{
	// Drawable area
	int draw_w = area.x2 - area.x1;
	int draw_h = area.y2 - area.y1;

	// sin and cos of yaw
	scalar_t sn = math.sin[yaw];
	scalar_t cs = math.cos[yaw];

	// Z, and current change in Z
	scalar_t z = SCALAR(0.05f);
	scalar_t dz = SCALAR(0.5f);

	// Initialize y-buffer
	for (int i = 0; i < draw_w; i++)
		ybuffer[i] = ceiling == true ? 0 : draw_h;

	// Render front to back
	while (z < draw_distance)
	{
		vec2s_t pleft, pright;

		pleft.x = (MUL(-cs, z) - MUL(sn, z)) + p.x;
		pleft.y = (MUL(sn, z) - MUL(cs, z)) + p.y;

		pright.x = (MUL(cs, z) - MUL(sn, z)) + p.x;
		pright.y = (MUL(-sn, z) - MUL(cs, z)) + p.y;

		scalar_t dx = DIV(pright.x - pleft.x, SCALAR(draw_w));
		scalar_t dy = DIV(pright.y - pleft.y, SCALAR(draw_w));

		for (int sx = 0; sx < draw_w; sx++)
		{
			vec2i_t pi;
			pi.x = ScalarToInteger(pleft.x);
			pi.y = ScalarToInteger(pleft.y);

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

			scalar_t h = SCALAR(heightmap[(pi.y * map_size.y) + pi.x]);
			scalar_t dh = ceiling == true ? h - p.z : p.z - h;

			uint8_t c = colormap[(pi.y * map_size.y) + pi.x];

			int line_height = ScalarToInteger(MUL(DIV(dh, z), height_scale)) + horizon;

			if (line_height > draw_h) break;
			line_height = CLAMP(line_height, area.y1, area.y2);

			if ((ceiling == true && line_height > ybuffer[sx]) || (ceiling == false && line_height < ybuffer[sx]))
			{
				//c = Colormap::Lookup(c, ScalarToInteger(z));
				Picture::DrawVerticalLine(dst, sx, line_height, ybuffer[sx], c);
				ybuffer[sx] = line_height;
			}

			pleft.x += dx;
			pleft.y += dy;
		}

		z += dz;
		//dz += SCALAR(0.02f);
	}
}

void VoxelRenderWrapper(Picture::pic_t *dst, rect_t area)
{
	// Throw in some collision detection while we're at it
	scalar_t minh = SCALAR(32);
	if (camera.origin.z < minh) camera.origin.z = minh;


	// map 1 floor
	//VoxelRender(dst, area, camera.origin, camera.angles.y, camera.angles.x, SCALAR(64), camera.draw_distance, false, VEC2I(1024, 1024), f_colormap, f_heightmap);

	// map 2 floor
	//VoxelRender(dst, area, camera.origin, camera.angles.y, 100, SCALAR(32), camera.draw_distance, false, VEC2I(64, 64), f_colormap, f_heightmap);

	// map 2 ceiling
	//VoxelRender(dst, area, camera.origin, camera.angles.y, 100, SCALAR(32), camera.draw_distance, true, VEC2I(64, 64), c_colormap, c_heightmap);

	// map 3 floor
	//VoxelRender(dst, area, camera.origin, camera.angles.y, 100, SCALAR(16), camera.draw_distance, false, VEC2I(150, 150), f_colormap, f_heightmap);

	// casino floor
	vec2i_t map_size = {512, 512};
	VoxelRender(dst, area, camera.origin, camera.angles.y, 100, SCALAR(128), camera.draw_distance, false, map_size, f_colormap, f_heightmap);
}

//
// Misc
//

// Mouse helper function
void ReadMouse(int16_t *buttons, vec2i_t *pos, int16_t speedlimit, rect_t area)
{
	int16_t mx, my, dmx, dmy;
	int16_t halfx = 160, halfy = 100;

	*buttons = DOS::MouseRead(&mx, &my);
	
	dmx = mx - halfx;
	dmy = my - halfy;

	pos->x += (dmx < 0) ? (dmx < -speedlimit ? -speedlimit : dmx) : (dmx > speedlimit ? speedlimit : dmx);
	pos->y += (dmy < 0) ? (dmy < -speedlimit ? -speedlimit : dmy) : (dmy > speedlimit ? speedlimit : dmy);

	// Clip mouse to area
	if (pos->x < area.x1) pos->x = area.x1;
	if (pos->x > area.x2) pos->x = area.x2;
	if (pos->y < area.y1) pos->y = area.y1;
	if (pos->y > area.y2) pos->y = area.y2;

	// 160x100 is the the middle of the 320x200 screen
	// (use this regardless of actual screen resolution)
	DOS::MouseSet(halfx, halfy);
}

//
// Main entry point
//

int main(int argc, char *argv[])
{
	// General variables
	int i;

	// Cycle variables
	int64_t frame_start, frame_end;
	int cycles, c;

	// Picture buffers
	Picture::pic_t pic_font;
	Picture::pic_t pic_bbuffer;
	//Picture::pic_t pic_background;
	Picture::pic_t pic_cursor;

	// Initialize DOS
	DOS::Initialize();

	// Initialize VESA
	if (VESA::Initialize(320, 200, 8) == false) return EXIT_FAILURE;
	VESA::VidInfo vidinfo = VESA::GetVidInfo();

	// Load colormap
	VESA::SetPalette("gfx/portal2d.pal");
	Colormap::Load("gfx/portal2d.tab");

	// Create picture buffers
	Console::Initialize();
	Picture::InitializeFrontBuffer();
	Picture::LoadBMP(&pic_font, "gfx/font8x8.bmp");
	Picture::LoadBMP(&pic_cursor, "local/cursor.bmp");
	//Picture::LoadBMP(&pic_background, "local/forest.bmp");
	Picture::Create(&pic_bbuffer, vidinfo.width, vidinfo.height, vidinfo.bpp, 0, 0);

	// Generate math tables
	for (i = 0; i < 360; i++)
	{
		math.sin[i] = SCALAR(sin(i / 180.0f * PI));
		math.cos[i] = SCALAR(cos(i / 180.0f * PI));
		math.tan[i] = SCALAR(tan(i / 180.0f * PI));
	}

	// Initialize voxel stuff
	VoxelInit(vidinfo.width);

	// Initialize VoxCave
	//VoxCave_Init(VEC2I(vidinfo.width, vidinfo.height));

	// Start counting time
	frame_end = DOS::TimerGet64();

	// Main loop
	while (!DOS::KeyTest(KB_ESC))
	{
		// Get start of frame time
		frame_start = DOS::TimerGet64();

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
		Picture::Clear(&pic_bbuffer, 0);

		// Voxels
		{
			// watcom...
			rect_t screen_area = {0, 0, pic_bbuffer.width, pic_bbuffer.height};

			// VoxCave renderer
			//VoxCave_Render(&pic_bbuffer, screen_area);

			// Voxel renderer
			VoxelRenderWrapper(&pic_bbuffer, screen_area);
		}

		// Render the console text
		Console::Render(&pic_bbuffer, &pic_font, 8);

		// Flip the rendering buffers
		Picture::CopyToFrontBuffer(&pic_bbuffer);

		// Get end of frame time
		#if (REX_COMPILER == COMPILER_DJGPP)
			frame_end = frame_end + cycles * UCLOCKS_PER_SEC / CYCLES;
		#endif

		#if (REX_COMPILER == COMPILER_WATCOM)
			frame_end = frame_end + cycles * CLOCKS_PER_SEC / CYCLES;
		#endif
	}

	// Shutdown VESA
	VESA::Shutdown();

	// Shutdown DOS
	DOS::Shutdown();

	// Cleanup memory
	Console::Shutdown();
	Picture::Destroy(&pic_font);
	Picture::Destroy(&pic_bbuffer);
	//Picture::Destroy(&pic_background);
	Picture::Destroy(&pic_cursor);
	Picture::ShutdownFrontBuffer();

	// Exit gracefully
	return EXIT_SUCCESS;
}
