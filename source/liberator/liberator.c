// ========================================================
//
// FILE:			/source/liberator/main.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Liberator program entry point.
//
// LAST EDITED:		October 19th, 2022
//
// ========================================================

// Include liberator header
#include "liberator.h"

rex_window_external *window_main;
SDL_Surface *window_main_text1;

rex_coord3i cube_vertices[] = {
	// Top face
	{64, 64, -64},
	{-64, 64, -64},
	{-64, 64, 64},
	{64, 64, 64},
	// Bottom face
	{64, -64, -64},
	{-64, -64, -64},
	{-64, -64, 64},
	{64, -64, 64},
	// Front face
	{64, 64, 64},
	{-64, 64, 64},
	{-64, -64, 64},
	{64, -64, 64},
	// Back face
	{64, -64, -64},
	{-64, -64, -64},
	{-64, 64, -64},
	{64, 64, -64},
	// Left face
	{-64, 64, 64},
	{-64, 64, -64},
	{-64, -64, -64},
	{-64, -64, 64},
	// Right face
	{64, 64, 64},
	{64, 64, -64},
	{64, -64, -64},
	{64, -64, 64}
};

rex_rgb cube_colors[] = {
	// Top face
	REX_RGB(0, 255, 0),
	REX_RGB(0, 255, 0),
	REX_RGB(0, 255, 0),
	REX_RGB(0, 255, 0),
	// Bottom face
	REX_RGB(255, 128, 0),
	REX_RGB(255, 128, 0),
	REX_RGB(255, 128, 0),
	REX_RGB(255, 128, 0),
	// Front face
	REX_RGB(255, 0, 0),
	REX_RGB(255, 0, 0),
	REX_RGB(255, 0, 0),
	REX_RGB(255, 0, 0),
	// Back face
	REX_RGB(255, 255, 0),
	REX_RGB(255, 255, 0),
	REX_RGB(255, 255, 0),
	REX_RGB(255, 255, 0),
	// Left face
	REX_RGB(0, 0, 255),
	REX_RGB(0, 0, 255),
	REX_RGB(0, 0, 255),
	REX_RGB(0, 0, 255),
	// Right face
	REX_RGB(255, 0, 255),
	REX_RGB(255, 0, 255),
	REX_RGB(255, 0, 255),
	REX_RGB(255, 0, 255)
};

// Camera angle
rex_vector3i camera_angle;

// Camera position
rex_vector3i camera_position;

// Set camera position
void Liberator_Camera_SetPosition(rex_int x, rex_int y, rex_int z)
{
	camera_position[0] = x;
	camera_position[1] = y;
	camera_position[2] = z;
}

// Set camera angle
void Liberator_Camera_SetAngle(rex_int x, rex_int y, rex_int z)
{
	camera_angle[0] = x;
	camera_angle[1] = y;
	camera_angle[2] = z;
}

// Handle mouse inputs
void Liberator_Mouse_OrbitControls(void)
{
	if (KEY_DOWN(KEY_R))
	{
		Liberator_Camera_SetPosition(0, 0, -256);
		Liberator_Camera_SetAngle(45, 45, 0);
	}
	else if (MOUSE_DOWN(MOUSE_LEFT)) // Orbit
	{
		if (rex_mouse_delta[0]) camera_angle[0] += rex_mouse_delta[0] / 2;
		if (rex_mouse_delta[1]) camera_angle[1] += rex_mouse_delta[1] / 2;

		if (camera_angle[1] > 90) camera_angle[1] = 90;
		if (camera_angle[1] < -90) camera_angle[1] = -90;
	}
	else if (MOUSE_DOWN(MOUSE_RIGHT)) // Zoom
	{
		if (rex_mouse_delta[1]) camera_position[2] -= rex_mouse_delta[1];

		if (camera_position[2] > 0) camera_position[2] = 0;
	}
	else if (MOUSE_DOWN(MOUSE_MIDDLE)) // Pan
	{
		if (rex_mouse_delta[0]) camera_position[0] += rex_mouse_delta[0];
		if (rex_mouse_delta[1]) camera_position[1] -= rex_mouse_delta[1];
	}

	// cap this because of weirdness with reading it upon startup
	if (rex_mouse_scroll_delta[1] < 16) camera_position[2] += rex_mouse_scroll_delta[1] * 16;
}

// Print help text
void Liberator_PrintHelpText(void)
{
	printf("Liberator v0.0.1\n");
	printf("By Erysdren Media\n");
	printf("===============================");
	printf("\n");
	printf("Usage:\n");
	printf("Liberator -op0 pak0.pak\n");
	printf("\n");
	printf("Arguments:\n");
	printf("-opXXX - Select file operation\n");
	printf("\n");
	printf("File operations:\n");
	printf("0 - List file contents\n");
	printf("1 - Extract file contents\n");
	printf("2 - Convert file contents\n");
	printf("3 - View file contents\n");

	Rex_Shutdown();
}

// Main function
void main(int argc, char *argv[])
{
	rex_int error = REX_ERROR_NONE;
	rex_int operation = REX_FORMATOP_GETINFO;
	rex_byte filename[512];

	// If no arguments, print help text
	if (argc == 1) Liberator_PrintHelpText();

	// Process commandline arguments
	while (*++argv)
	{
		if (!strcmp(*argv, "-?"))
		{
			Liberator_PrintHelpText();
		}
		else if (sscanf(*argv, "-op%d", &operation) == 1)
		{
			Rex_Message("Input operation: %d - %s", operation, Rex_Formats_OptHelper(operation));
		}
		else if (**argv == '-')
		{
			Rex_Warning("Invalid switch %s", *argv);
		}
		else
		{
			sprintf(filename, *argv);
			Rex_Message("Input filename: %s", filename);
		}
	}

	// Startup engine
	Rex_Startup();

	// Determine operation to do
	if (operation == REX_FORMATOP_GETINFO)
	{
		error = Rex_Formats_idTech_MDL(REX_FORMATOP_GETINFO, filename);
		if (error) Rex_Failure("%s", Rex_GetError(error));

		Rex_Shutdown();
	}
	else if (operation == REX_FORMATOP_VIEW)
	{
		error = Rex_Formats_idTech_MDL(REX_FORMATOP_VIEW, filename);
		if (error) Rex_Failure("%s", Rex_GetError(error));
	}

	// Add main window
	window_main = Rex_WindowExternal_Add(
		"Liberator",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		rex_desktop_size[0] / 2, rex_desktop_size[1] / 2,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	// Setup GL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	// Set doublebuffer swap interval
	SDL_GL_SetSwapInterval(1);

	// Enable GL attributes
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	glEnable(GL_COLOR_ARRAY);
	glDepthFunc(GL_LEFT);

	// Reset camera position
	Liberator_Camera_SetPosition(0, 0, -256);
	Liberator_Camera_SetAngle(45, 45, 0);

	// Set vertex and color pointers
	glVertexPointer(3, GL_FLOAT, 0, gl_vertices_f);
	glColorPointer(3, GL_UNSIGNED_BYTE, 0, cube_colors);

	// Start counting time
	rex_udouble frame_start;
	rex_udouble frame_end;
	rex_float frame_elapsed;

	// Main loop
	while (REX_TRUE)
	{
		// Get start-of-frame time
		frame_start = SDL_GetTicks();

		SDL_GetWindowSize(window_main->window, &window_main->width, &window_main->height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90, (GLdouble)window_main->width / (GLdouble)window_main->height, 0.01, 1000);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslated(camera_position[0], camera_position[1], camera_position[2]);
		glRotated(camera_angle[1], 1, 0, 0); // X
		glRotated(camera_angle[0], 0, 1, 0); // Y

		// Read device states
		Rex_IO_ReadDevices();

		// Handle keyboard input
		if (KEY_DOWN(KEY_Q))
			Rex_Shutdown();

		// Handle mouse input
		Liberator_Mouse_OrbitControls();

		// Clear the screen
		Rex_ExternalWindow_ClearGL(REX_RGBA_GRY, 255);

		// Draw vertex arrays
		glDrawArrays(GL_TRIANGLES, 0, num_gl_vertices_f);

		// Regenerate text surface
		Rex_Fonts_GenerateTextSurface(&window_main_text1,
			font_dos_8x16,
			REX_RGBA_WHT,
			"FPS: %.2f\n\nCamera PosX: %d\nCamera PosY: %d\nCamera PosZ: %d\n\nCamera AngX: %d\nCamera AngY: %d\nCamera AngZ: %d",
			frame_elapsed,
			camera_position[0], camera_position[1], camera_position[2],
			camera_angle[0], camera_angle[1], camera_angle[2]
		);

		// Render text surface on screen
		Rex_ExternalWindow_RenderSurfaceGL(window_main, window_main_text1, 16, 16);

		// Swap GL buffer
		Rex_ExternalWindow_SwapBuffer(window_main);

		// Get end-of-frame time and calculate time elapsed
		frame_end = SDL_GetTicks();
		frame_elapsed = 1.0f / ((frame_end - frame_start) / 1000.0f);
	}

	// Shutdown engine
	Rex_Shutdown();
}
