// ========================================================
//
// FILE:			/source/testapp/testapp.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		TestApp entry point
//
// LAST EDITED:		January 5th, 2023
//
// ========================================================

// Header
#include "testapp.hpp"

int w_width = 640;
int w_height = 480;

#ifdef AGAHDSD

// Blit an SDL surface onto an OpenGL window
void Blit_Surface(SDL_Surface *srf, SDL_Rect *src, SDL_Rect *dst)
{
	// Sanity check
	if (srf == NULL) return;
	if (src == NULL) return;
	if (dst == NULL) return;

	GLuint texture;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, w_width, w_height, 0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, srf->w, srf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, srf->pixels);

	glBegin(GL_QUADS);
		glTexCoord2f((GLfloat)src->x / srf->w, (GLfloat)src->y / srf->h); glVertex2i(dst->x, dst->y);
		glTexCoord2f((GLfloat)(src->x + src->w) / srf->w, (GLfloat)src->y / srf->h); glVertex2i(dst->x + dst->w, dst->y);
		glTexCoord2f((GLfloat)(src->x + src->w) / srf->w, (GLfloat)(src->y + src->h) / srf->h); glVertex2i(dst->x + dst->w, dst->y + dst->h);
		glTexCoord2f((GLfloat)src->x / srf->w, (GLfloat)(src->y + src->h) / srf->h); glVertex2i(dst->x, dst->y + dst->h);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

#endif

//
// Main entry point
//

int RexMain(int argc, char **argv)
{
	// Initialize Rex Engine
	Rex::Init();

	// Initialize Rex Engine Graphics
	Rex::Init_Graphics(640, 480, 8, "TestApp");

	// Test message
	Rex::Message("Test", "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");

	// Quit Rex Engine Graphics
	Rex::Quit_Graphics();

	// Quit Rex Engine
	Rex::Quit();

	// Exit gracefully
	return EXIT_SUCCESS;
}
