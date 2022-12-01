// ========================================================
//
// FILE:			/source/math/matrix.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Matrix namespace header
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

//
//
// Matrix namespace
//
//

// Matrix namespace definition (public)
namespace Matrix
{
	//
	//
	// Functions
	//
	//

	void Copy(matrix_t dst, matrix_t src);
	void Identity(matrix_t dst);
	void Position(matrix_t dst, int x, int y, int z);
	void Scale(matrix_t dst, int x, int y, int z);
	void RotateX(matrix_t dst, int angle);
	void RotateY(matrix_t dst, int angle);
	void RotateZ(matrix_t dst, int angle);
	void Transpose(matrix_t dst, matrix_t src);
	void Multiply(matrix_t dst, matrix_t ma1, matrix_t ma2);
}
