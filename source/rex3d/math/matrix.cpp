// ========================================================
//
// FILE:			/source/math/matrix.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Matrix namespace implementation
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

// Rex3D header
#include "rex3d.hpp"

//
//
// Matrix namespace
//
//

// Matrix namespace definition (private)
namespace Matrix
{

//
//
// Variables
//
//

//
//
// Functions
//
//

// Copies the contents of one matrix to another.
void Copy(matrix_t dst, matrix_t src)
{
	memcpy(dst, src, sizeof(matrix_t));
}

// Returns a identity matrix.
void Identity(matrix_t dst)
{
	matrix_t src =
	{
		{i2f(1), i2f(0), i2f(0), i2f(0)},
		{i2f(0), i2f(1), i2f(0), i2f(0)},
		{i2f(0), i2f(0), i2f(1), i2f(0)},
		{i2f(0), i2f(0), i2f(0), i2f(1)}
	};

	Copy(dst, src);
}

void Position(matrix_t dst, int x, int y, int z)
{
	Identity(dst);
	dst[0][3] = x;
	dst[1][3] = y;
	dst[2][3] = z;
}

void Scale(matrix_t dst, int x, int y, int z)
{
	Identity(dst);
	dst[0][0] = x;
	dst[1][1] = y;
	dst[2][2] = z;
}

void RotateX(matrix_t dst, int angle)
{
	Identity(dst);
	dst[1][1] =   (dst[2][2] = fixcos(angle));
	dst[1][2] = - (dst[2][1] = fixsin(angle));
}

void RotateY(matrix_t dst, int angle)
{
	Identity(dst);
	dst[0][0] =   (dst[2][2] = fixcos(angle));
	dst[0][2] = - (dst[2][0] = fixsin(angle));
}

void RotateZ(matrix_t dst, int angle)
{
	Identity(dst);
	dst[0][0] =   (dst[1][1] = fixcos(angle));
	dst[0][1] = - (dst[1][0] = fixsin(angle));
}

void Multiply(matrix_t dst, matrix_t ma1, matrix_t ma2)
{
	int i, j;
	matrix_t tmp;

	for (i = 4; i--;)
	{
		for (j = 4; j--;)
		{
			tmp[i][j] =
				fixmul(ma2[i][0], ma1[0][j]) +
				fixmul(ma2[i][1], ma1[1][j]) +
				fixmul(ma2[i][2], ma1[2][j]) +
				fixmul(ma2[i][3], ma1[3][j]);
		}
	}

	Copy(dst, tmp);
}

void Transpose(matrix_t dst, matrix_t src)
{
	int x, y;

	for (y = 4; y--;) for (x = 4; x--;) dst[x][y] = src[y][x];
}

} // namespace Matrix
