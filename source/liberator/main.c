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
// LAST EDITED:		October 14th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

rex_window_external *window1;

void main(int argc, char *argv[])
{
	Rex_Startup();

	window1 = Rex_WindowExternal_Add("Liberator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEFT);

	while (REX_TRUE)
	{
		SDL_GetWindowSize(window1->window, &window1->width, &window1->height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.0, (GLdouble)window1->width / (GLdouble)window1->height, 0.01f, 1000);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, -3);
		glRotatef(50, 1, 0, 0);
		glRotatef(70, 0, 1, 0);

		Rex_IO_ReadDevices();

		if (KEY_DOWN(SDL_SCANCODE_Q))
			Rex_Shutdown();

		Rex_ExternalWindow_ClearGL(REX_RGBA_GRY, 255);

		// Render a cube
		glBegin(GL_QUADS);
			// Top face
			glColor3f(   0.0f, 1.0f,  0.0f );  // Green
			glVertex3f(  1.0f, 1.0f, -1.0f );  // Top-right of top face
			glVertex3f( -1.0f, 1.0f, -1.0f );  // Top-left of top face
			glVertex3f( -1.0f, 1.0f,  1.0f );  // Bottom-left of top face
			glVertex3f(  1.0f, 1.0f,  1.0f );  // Bottom-right of top face

			// Bottom face
			glColor3f(   1.0f,  0.5f,  0.0f ); // Orange
			glVertex3f(  1.0f, -1.0f, -1.0f ); // Top-right of bottom face
			glVertex3f( -1.0f, -1.0f, -1.0f ); // Top-left of bottom face
			glVertex3f( -1.0f, -1.0f,  1.0f ); // Bottom-left of bottom face
			glVertex3f(  1.0f, -1.0f,  1.0f ); // Bottom-right of bottom face

			// Front face
			glColor3f(   1.0f,  0.0f, 0.0f );  // Red
			glVertex3f(  1.0f,  1.0f, 1.0f );  // Top-Right of front face
			glVertex3f( -1.0f,  1.0f, 1.0f );  // Top-left of front face
			glVertex3f( -1.0f, -1.0f, 1.0f );  // Bottom-left of front face
			glVertex3f(  1.0f, -1.0f, 1.0f );  // Bottom-right of front face

			// Back face
			glColor3f(   1.0f,  1.0f,  0.0f ); // Yellow
			glVertex3f(  1.0f, -1.0f, -1.0f ); // Bottom-Left of back face
			glVertex3f( -1.0f, -1.0f, -1.0f ); // Bottom-Right of back face
			glVertex3f( -1.0f,  1.0f, -1.0f ); // Top-Right of back face
			glVertex3f(  1.0f,  1.0f, -1.0f ); // Top-Left of back face

			// Left face
			glColor3f(   0.0f,  0.0f,  1.0f);  // Blue
			glVertex3f( -1.0f,  1.0f,  1.0f);  // Top-Right of left face
			glVertex3f( -1.0f,  1.0f, -1.0f);  // Top-Left of left face
			glVertex3f( -1.0f, -1.0f, -1.0f);  // Bottom-Left of left face
			glVertex3f( -1.0f, -1.0f,  1.0f);  // Bottom-Right of left face

			// Right face
			glColor3f(   1.0f,  0.0f,  1.0f);  // Violet
			glVertex3f(  1.0f,  1.0f,  1.0f);  // Top-Right of left face
			glVertex3f(  1.0f,  1.0f, -1.0f);  // Top-Left of left face
			glVertex3f(  1.0f, -1.0f, -1.0f);  // Bottom-Left of left face
			glVertex3f(  1.0f, -1.0f,  1.0f);  // Bottom-Right of left face
		glEnd();

		Rex_ExternalWindow_SwapBuffer(window1);
	}

	Rex_Shutdown();
}
