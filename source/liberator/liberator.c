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
// LAST EDITED:		October 18th, 2022
//
// ========================================================

// Include engine header
#include "liberator.h"

rex_window_external *window_main;
SDL_Surface *window_main_text1;

rex_int cube_vertices[] = {
	// Top face
	64, 64, -64,
	-64, 64, -64,
	-64, 64, 64,
	64, 64, 64,
	// Bottom face
	64, -64, -64,
	-64, -64, -64,
	-64, -64, 64,
	64, -64, 64,
	// Front face
	64, 64, 64,
	-64, 64, 64,
	-64, -64, 64,
	64, -64, 64,
	// Back face
	64, -64, -64,
	-64, -64, -64,
	-64, 64, -64,
	64, 64, -64,
	// Left face
	-64, 64, 64,
	-64, 64, -64,
	-64, -64, -64,
	-64, -64, 64,
	// Right face
	64, 64, 64,
	64, 64, -64,
	64, -64, -64,
	64, -64, 64
};

// Camera angle
rex_vector3d camera_angle;

// Camera position
rex_vector3d camera_position;

// Set camera position
void Liberator_Camera_SetPosition(rex_double x, rex_double y, rex_double z)
{
	camera_position[0] = x;
	camera_position[1] = y;
	camera_position[2] = z;
}

// Set camera angle
void Liberator_Camera_SetAngle(rex_double x, rex_double y, rex_double z)
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
}

// Main function
void main(int argc, char *argv[])
{
	Rex_Startup();

	window_main = Rex_WindowExternal_Add(
		"Liberator",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		rex_desktop_size[0] / 2, rex_desktop_size[1] / 2,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEFT);

	Liberator_Camera_SetPosition(0, 0, -256);
	Liberator_Camera_SetAngle(45, 45, 0);

	rex_udouble frame_start;
	rex_udouble frame_end;
	rex_float frame_elapsed;

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

		// Render a cube
		glBegin(GL_QUADS);
			// Top face
			glColor3ub(0, 255, 0); // Green
			glVertex3i(64, 64, -64); // Top-right of top face
			glVertex3i(-64, 64, -64); // Top-left of top face
			glVertex3i(-64, 64, 64); // Bottom-left of top face
			glVertex3i(64, 64, 64); // Bottom-right of top face

			// Bottom face
			glColor3ub(255, 128, 0); // Orange
			glVertex3i(64, -64, -64); // Top-right of bottom face
			glVertex3i(-64, -64, -64); // Top-left of bottom face
			glVertex3i(-64, -64, 64); // Bottom-left of bottom face
			glVertex3i(64, -64, 64); // Bottom-right of bottom face

			// Front face
			glColor3ub(255, 0, 0); // Red
			glVertex3i(64, 64, 64); // Top-Right of front face
			glVertex3i(-64, 64, 64); // Top-left of front face
			glVertex3i(-64, -64, 64); // Bottom-left of front face
			glVertex3i(64, -64, 64); // Bottom-right of front face

			// Back face
			glColor3ub(255, 255, 0); // Yellow
			glVertex3i(64, -64, -64); // Bottom-Left of back face
			glVertex3i(-64, -64, -64); // Bottom-Right of back face
			glVertex3i(-64, 64, -64); // Top-Right of back face
			glVertex3i(64, 64, -64); // Top-Left of back face

			// Left face
			glColor3ub(0, 0, 255); // Blue
			glVertex3i(-64, 64, 64); // Top-Right of left face
			glVertex3i(-64, 64, -64); // Top-Left of left face
			glVertex3i(-64, -64, -64); // Bottom-Left of left face
			glVertex3i(-64, -64, 64); // Bottom-Right of left face

			// Right face
			glColor3ub(255, 0, 255); // Violet
			glVertex3i(64, 64, 64); // Top-Right of left face
			glVertex3i(64, 64, -64); // Top-Left of left face
			glVertex3i(64, -64, -64); // Bottom-Left of left face
			glVertex3i(64, -64, 64); // Bottom-Right of left face
		glEnd();

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

	Rex_Shutdown();
}
