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

// SDL2
#include <SDL2/SDL_image.h>

// OpenGL
#include <GLFW/glfw3.h>
#include <GL/glut.h>

int w_width = 640;
int w_height = 480;

// Globals, ew
SDL_Surface *s_buffer;
GLFWwindow *window;

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

//
// Main entry point
//

int RexMain(int argc, char *argv[])
{
	if (Rex::Init() == false)
	{
		Rex::MessageBox("Error!", "Rex Engine initialization failed!");
		return EXIT_FAILURE;
	}

	// Initialize GLFW
	if (!glfwInit())
	{
		Rex::MessageBox("Error!", "GLFW failed to initialize!");
		return EXIT_FAILURE;
	}

	// GLFW Window Hints
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	// Create GLFW window and surface buffer
	window = glfwCreateWindow(w_width, w_height, "TestApp", NULL, NULL);
	glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
	glfwDefaultWindowHints();
	glfwMakeContextCurrent(window);
	glfwShowWindow(window);

	// Create SDL_Surface's
	SDL_Surface *s_shades01 = IMG_Load("./build/assets/shades/base/window.png");
	SDL_Surface *s_shades01_glow = IMG_Load("./build/assets/shades/base/window_glow.png");

	if (s_shades01 == NULL || s_shades01_glow == NULL)
	{
		Rex::MessageBox("Error!", "Failed to load images!");
		return EXIT_FAILURE;
	}

	//
	// Create SDL_Rect's
	//

	// upper right corner
	SDL_Rect r_src1 = {132, 0, 124, 48};
	SDL_Rect r_dst1 = {w_width - 124, 0, 124, 48};

	// upper left corner
	SDL_Rect r_src2 = {0, 0, 106, 48};
	SDL_Rect r_dst2 = {0, 0, 106, 48};

	// top left bar
	SDL_Rect r_src3 = {106, 0, 18, 48};
	SDL_Rect r_dst3 = {106, 0, w_width - 124 - 106, 48};

	// left bar
	SDL_Rect r_src4 = {0, 48, 21, 8};
	SDL_Rect r_dst4 = {0, 48, 21, w_height - 21 - 48};

	// inner box
	SDL_Rect r_src5 = {70, 176, 4, 4};
	SDL_Rect r_dst5 = {21, 48, w_width - 41 - 21, w_height - 21 - 48};

	// lower left corner
	SDL_Rect r_src6 = {0, 234, 22, 22};
	SDL_Rect r_dst6 = {0, w_height - 22, 22, 22};

	// right bar
	SDL_Rect r_src7 = {215, 48, 41, 8};
	SDL_Rect r_dst7 = {w_width - 41, 48, 41, w_height - 21 - 48};

	// bottom right corner
	SDL_Rect r_src8 = {215, 235, 41, 21};
	SDL_Rect r_dst8 = {w_width - 41, w_height - 21, 41, 21};

	// bottom bar
	SDL_Rect r_src9 = {21, 235, 8, 21};
	SDL_Rect r_dst9 = {21, w_height - 21, w_width - 41 - 21, 21};

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll for events
		glfwPollEvents();

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Blit_Surface(s_shades01, &r_src1, &r_dst1);
		Blit_Surface(s_shades01, &r_src2, &r_dst2);
		Blit_Surface(s_shades01, &r_src3, &r_dst3);
		Blit_Surface(s_shades01, &r_src4, &r_dst4);
		Blit_Surface(s_shades01, &r_src5, &r_dst5);
		Blit_Surface(s_shades01, &r_src6, &r_dst6);
		Blit_Surface(s_shades01, &r_src7, &r_dst7);
		Blit_Surface(s_shades01, &r_src8, &r_dst8);
		Blit_Surface(s_shades01, &r_src9, &r_dst9);

		Blit_Surface(s_shades01_glow, &r_src1, &r_dst1);
		Blit_Surface(s_shades01_glow, &r_src2, &r_dst2);
		Blit_Surface(s_shades01_glow, &r_src3, &r_dst3);
		Blit_Surface(s_shades01_glow, &r_src4, &r_dst4);
		Blit_Surface(s_shades01_glow, &r_src5, &r_dst5);
		Blit_Surface(s_shades01_glow, &r_src6, &r_dst6);
		Blit_Surface(s_shades01_glow, &r_src7, &r_dst7);
		Blit_Surface(s_shades01_glow, &r_src8, &r_dst8);
		Blit_Surface(s_shades01_glow, &r_src9, &r_dst9);

		// Swap buffers
		glfwSwapBuffers(window);
	}

	if (Rex::Quit() == false)
	{
		Rex::MessageBox("Error!", "Rex Engine shutdown failed!");
		return EXIT_FAILURE;
	}

	// Terminate GLFW
	glfwTerminate();

	// Exit gracefully
	return EXIT_SUCCESS;
}
