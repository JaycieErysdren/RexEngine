// ========================================================
//
// FILE:			/source/math/math_x.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Fixed-point math header
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

//
//
// Functions
//
//

int fixinv(int a)
{
	return fixdiv(i2f(1), a); 
}

int fixmul(int a, int b)
{
	return ((int64_t)a * b) >> 16;
}

int fixdiv(int a, int b)
{
	return ((int64_t)a << 16) / b;
}

int fixdot3(int a1, int b1, int a2, int b2, int a3, int b3)
{
	return ((int64_t)a1 * b1 + (int64_t)a2 * b2 + (int64_t)a3 * b3) >> 16;
}

#ifdef UNDONE

//
// Fixed sin, cos, tan, cot
//

const float sinTable[64] =
{
	0.0122715, 0.0368072, 0.0613207, 0.0857973, 0.110222, 0.134581, 0.158858, 0.18304, 0.207111, 0.231058, 0.254866, 0.27852, 0.302006, 0.32531, 0.348419, 0.371317,
	0.393992, 0.41643, 0.438616, 0.460539, 0.482184, 0.503538, 0.52459, 0.545325, 0.565732, 0.585798, 0.605511, 0.624859, 0.643832, 0.662416, 0.680601, 0.698376,
	0.715731, 0.732654, 0.749136, 0.765167, 0.780737, 0.795837, 0.810457, 0.824589, 0.838225, 0.851355, 0.863973, 0.87607, 0.88764, 0.898674, 0.909168, 0.919114,
	0.928506, 0.937339, 0.945607, 0.953306, 0.960431, 0.966976, 0.97294, 0.978317, 0.983105, 0.987301, 0.990903, 0.993907, 0.996313, 0.998118, 0.999322, 0.999925
};

const float tanTable[64] =
{
	0.0122725, 0.0368322, 0.0614364, 0.0861149, 0.110898, 0.135816, 0.160901, 0.186185, 0.211702, 0.237484, 0.26357, 0.289995, 0.316799, 0.344023, 0.37171, 0.399908,
	0.428665, 0.458034, 0.48807, 0.518835, 0.550394, 0.582817, 0.616182, 0.650571, 0.686077, 0.722799, 0.760848, 0.800345, 0.841426, 0.884239, 0.928952, 0.975753,
	1.02485, 1.07648, 1.13092, 1.18846, 1.24946, 1.31432, 1.38351, 1.45756, 1.53711, 1.6229, 1.7158, 1.81688, 1.92739, 2.04889, 2.18325, 2.33282,
	2.50057, 2.69027, 2.90679, 3.15658, 3.44834, 3.79406, 4.2108, 4.72363, 5.37099, 6.21499, 7.36289, 9.0173, 11.6124, 16.277, 27.1502, 81.4832
};

fix32_t sinLookup(uint8_t index)
{
	return FIX32(sinTable[index]);
}

fix32_t tanLookup(uint8_t index)
{
	return FIX32(tanTable[index]);
}

fix32_t sinFixed(uint8_t brads)
{
	const uint8_t quadrant = ((brads & 0xC0) >> 6);
	const uint8_t index = ((brads & 0x3F) >> 0);

	switch (quadrant)
	{
		case 0: return sinLookup(index);
		case 1: return sinLookup(63 - index);
		case 2: return -sinLookup(index);
		case 3: return -sinLookup(63 - index);
		default: return 0;
	}
}

fix32_t cosFixed(uint8_t brads)
{
	const uint8_t quadrant = ((brads & 0xC0) >> 6);
	const uint8_t index = ((brads & 0x3F) >> 0);

	switch (quadrant)
	{
		case 0: return sinLookup(63 - index);
		case 1: return -sinLookup(index);
		case 2: return -sinLookup(63 - index);
		case 3: return sinLookup(index);
		default: return 0;
	}
}

fix32_t tanFixed(uint8_t brads)
{
	const uint8_t quadrant = ((brads & 0xC0) >> 6);
	const uint8_t index = ((brads & 0x3F) >> 0);

	switch (quadrant)
	{
		case 0: case 2: return tanLookup(index);
		case 1: case 3: return -tanLookup(63 - index);
		default: return 0;
	}
}

fix32_t cotFixed(uint8_t brads)
{
	const uint8_t quadrant = ((brads & 0xC0) >> 6);
	const uint8_t index = ((brads & 0x3F) >> 0);

	switch (quadrant)
	{
		case 0: case 2: return tanLookup(63 - index);
		case 1: case 3: return -tanLookup(index);
		default: return 0;
	}
}

#endif
