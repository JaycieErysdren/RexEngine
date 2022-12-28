// ========================================================
//
// FILE:			/source/rex/core/glrend/test.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: GLRend Test implementation
//
// LAST EDITED:		December 27th, 2022
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

// OSMesa
#include "thrdprty/osmesa/osmesa.h"
#include "thrdprty/osmesa/GL/glu.h"

//
//
// Rex namespace: GLRend Test
//
//

namespace Rex
{

void Sphere(float radius, int slices, int stacks)
{
	GLUquadric *q = gluNewQuadric();
	gluQuadricNormals(q, GLU_SMOOTH);
	gluSphere(q, radius, slices, stacks);
	gluDeleteQuadric(q);
}

void GL_Test()
{
	OSMesaContext context;
	void *buffer;
	static int w = 640;
	static int h = 480;
	static int sz = w * h * 4 * sizeof(GLubyte);

	context = OSMesaCreateContextExt(OSMESA_RGBA, 16, 0, 0, NULL);
	buffer = malloc(sz);

	OSMesaMakeCurrent(context, buffer, GL_UNSIGNED_BYTE, w, h);

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat red_mat[]   = { 1.0, 0.2, 0.2, 1.0 };
	GLfloat green_mat[] = { 0.2, 1.0, 0.2, 1.0 };
	GLfloat blue_mat[]  = { 0.2, 0.2, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.5, 2.5, -2.5, 2.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(20.0, 1.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef(0.75, 0.0, -1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue_mat);
	GLUquadric *q = gluNewQuadric();
	gluQuadricNormals(q, GLU_SMOOTH);
	gluSphere(q, 1.0, 20, 20);
	gluDeleteQuadric(q);
	glPopMatrix();

	glPopMatrix();

	glFinish();

	FILE *file = fopen("glbuf.dat", "wb");

	fwrite(buffer, sz, 1, file);

	fclose(file);

	free(buffer);
	OSMesaDestroyContext(context);
}

} // namespace Rex
