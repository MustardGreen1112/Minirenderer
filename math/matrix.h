#ifndef MINIRENDERER_MATH_MATRIX_H_
#define MINIRENDERER_MATH_MATRIX_H_

#include "vector.h"
#include <initializer_list>

struct mat3x3 {
	float elements[3][3];
	mat3x3() {
		int i = 0, j = 0;
		for (const auto& row : elements) {
			j = 0;
			for (float col : row) {
				elements[i][j] = 0;
				j++;
			}
			i++;
		}
	};
	mat3x3(std::initializer_list<std::initializer_list<float>> values) {
		int i = 0, j = 0;
		for (const auto& row : values) {
			j = 0;
			for (float value : row) {
				elements[i][j] = value;
				j++;
			}
			i++;
		}
	}
	mat3x3(vector3 r1, vector3 r2, vector3 r3)
	{
		float* r1Ptr = (float*)(&r1);
		float* r2Ptr = (float*)(&r2);
		float* r3Ptr = (float*)(&r3);

		for (int i = 0; i < 3; i++) {
			elements[0][i] = *(r1Ptr + i);
			elements[1][i] = *(r2Ptr + i);
			elements[2][i] = *(r3Ptr + i);
		}
	}

};

struct mat4x4 {
	float elements[4][4];
	mat4x4() {
		int i = 0, j = 0;
		for (const auto& row : elements) {
			j = 0;
			for (float col : row) {
				elements[i][j] = 0;
				j++;
			}
			i++;
		}
	};
	mat4x4(std::initializer_list<std::initializer_list<float>> values) {
		int i = 0, j = 0;
		for (const auto& row : values) {
			j = 0;
			for (float value : row) {
				elements[i][j] = value;
				j++;
			}
			i++;
		}
	}
	mat4x4(vector4 r1, vector4 r2, vector4 r3, vector4 r4)
	{
		float* r1Ptr = (float*)(&r1);
		float* r2Ptr = (float*)(&r2);
		float* r3Ptr = (float*)(&r3);
		float* r4Ptr = (float*)(&r4);

		for (int i = 0; i < 4; i++) {
			elements[0][i] = *(r1Ptr + i);
			elements[1][i] = *(r2Ptr + i);
			elements[2][i] = *(r3Ptr + i);
			elements[3][i] = *(r4Ptr + i);
		}
	}
};

///////////////////////////////////////////////////
// 
// Constant matrices variables
//
///////////////////////////////////////////////////
const mat3x3 MATRIX3x3_IDENTITY = mat3x3
{
	{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	}
};

const mat3x3 MATRIX3x3_ZERO = mat3x3
{
	{
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	}
};

const mat4x4 MATRIX4x4_IDENTITY = mat4x4
{
	{
	{1.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f}
	}
};

const mat4x4 MATRIX4x4_ZERO = mat4x4
{
	{
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f}
	}
};
////////////////////////////////////////////////////
//
// Convert
//
////////////////////////////////////////////////////
mat4x4 mat3x3_to_mat4x4(mat3x3 m);
mat3x3 mat4x4_to_mat3x3(mat4x4 m);

////////////////////////////////////////////////////
//
// Debug
//
////////////////////////////////////////////////////
void mat3x3_print(mat3x3 m);
void mat4x4_print(mat4x4 m);

////////////////////////////////////////////////////
//
// Math
//
////////////////////////////////////////////////////
mat3x3 mul(mat3x3 m1, mat3x3 m2);
vector3 mul(mat3x3 m, vector3 v);
mat4x4 mul(mat4x4 m1, mat4x4 m2);
vector4 mul(mat4x4 m, vector4 v);

mat3x3 transpose(mat3x3 m);
mat4x4 transpose(mat4x4 m);

inline mat3x3 mat3x3_multiply_scalar(mat3x3 m, float scalar) {
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			m.elements[row][column] *= scalar;
		}
	}
	return m;
}
inline mat4x4 mat4x4_multiply_scalar(mat4x4 m, float scalar) {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			m.elements[row][column] *= scalar;
		}
	}
	return m;
}

mat4x4 inverse(mat4x4 m);

////////////////////////////////////////////////////
//
// Transform Matrix
//
////////////////////////////////////////////////////
mat4x4 mat4x4_translate(vector3 translation);
mat4x4 mat4x4_scale(vector3 scaling);
/// <summary>
/// Rotate in eular angle.
/// </summary>
/// <param name="angle"> is degree. </param>
/// <returns></returns>
mat4x4 mat4x4_rotate_x(float angle);
mat4x4 mat4x4_rotate_y(float angle);
mat4x4 mat4x4_rotate_z(float angle);
mat4x4 mat4x4_rotate(vector3 axis, float angle);


///////////////////////////////////////////////////
//
// Coordinate Transform
// 
///////////////////////////////////////////////////

/// <summary>
/// Transform world space position to view space. 
/// (Notice: world space is right-handed while view space is left-handed,
/// causing the forward direction of z is back to view direction. 
/// </summary>
/// <param name="from"> Camera position in world space. </param>
/// <param name="to"> World position. </param>
/// <param name="up"> Usually (0,1,0) unless pitch or raw. </param>
/// <returns></returns>
mat4x4 mat4x4_W2V(vector3 from, vector3 to, vector3 up);
///
/// \brief Constructs an orthogonal projection matrix, follow OpenGL convention.
/// Notice: View space is left-handed while clip space is right-handed.
/// This function returns a coordinates in right-handed coordinate.
///
/// \param right Coordinate of the right clipping plane .
/// \param top Coordinates of the top clipping plane.
/// \param near The distance to the near depth clipping plane.
/// \param far The distance to the far depth clipping plane.
/// \return The orthogonal projection matrix.
///
mat4x4 mat4x4_orthographic(float right, float top, float near, float far);
///
/// \brief Constructs a perspective projection matrix, follow OpenGL convention.
///
/// \param fov The vertical field of view in radians.
/// \param aspect The aspect ration (width divided by height).
/// \param near The distance to the near depth clipping plane.
/// \param far The distance to the far depth clipping plane.
/// \return The perspective projection matrix.
///
mat4x4 mat4x4_perspective(float fov, float aspect, float near, float far);
/// <summary>
/// World space is right-handed while Light space is left-handed.
/// </summary>
/// <param name="lightDir">Lightdir will be normalized in this function.</param>
/// <returns></returns>
mat4x4 mat4x4_W2L(vector3 lightDir);


#endif