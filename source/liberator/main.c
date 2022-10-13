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
// LAST EDITED:		October 12th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

rex_window_external *window1;
rex_rgba red = {255, 0, 0, 255};

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

	glDepthFunc(GL_LESS);

	glMatrixMode(GL_MODELVIEW);

	while (REX_TRUE)
	{
		Rex_IO_ReadDevices();

		if (KEY_DOWN(SDL_SCANCODE_Q))
			Rex_WindowExternal_Remove(window1);

		Rex_ExternalWindow_ClearGL(REX_RGBA_GRY, 255);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glTranslatef(-1.5f, 0.0f, -6.0f);

		glBegin(GL_TRIANGLES);
			// Front
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f( 0.0f, 1.0f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);
		
			// Right
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
		
			// Back
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
		
			// Left
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f( 0.0f, 1.0f, 0.0f);
			glColor3f(0.0f,0.0f,1.0f);
			glVertex3f(-1.0f,-1.0f,-1.0f);
			glColor3f(0.0f,1.0f,0.0f);
			glVertex3f(-1.0f,-1.0f, 1.0f);
		glEnd();

		Rex_ExternalWindow_SwapBuffer(window1);
	}

	Rex_Shutdown();
}
