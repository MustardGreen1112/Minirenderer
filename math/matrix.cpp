#include "matrix.h"
#include <stdio.h>

mat4x4 mat3x3_to_mat4x4(mat3x3 m) {
    mat4x4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (row == 3 || col == 3) 
                result.elements[row][col] = 0.0f;
            else
                result.elements[row][col] = m.elements[row][col];
        }
    }
    return result;
}
mat3x3 mat4x4_to_mat3x3(mat4x4 m) {
    mat3x3 result;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
                result.elements[row][col] = m.elements[row][col];
        }
    }
    return result;
}

void mat3x3_print(mat3x3 m) {
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            printf("%8.3f ", m.elements[row][column]);
        }
        printf("\n");
    }
}
void mat4x4_print(mat4x4 m) {
    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            printf("%8.3f ", m.elements[row][column]);
        }
        printf("\n");
    }
}

mat3x3 mul(mat3x3 m1, mat3x3 m2) {
    mat3x3 result;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            for (int k = 0; k < 3; k++) {
                result.elements[row][col] += m1.elements[row][k] * m2.elements[k][col];
            }
        }
    }
    return result;
}
vector3 mul(mat3x3 m, vector3 v) {
    vector3 result = VECTOR3_ZERO;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            result.elements[row] += m.elements[row][col] * v.elements[col];
        }
    }
    return result;
}
mat4x4 mul(mat4x4 m1, mat4x4 m2) {
    mat4x4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int k = 0; k < 4; k++) {
                result.elements[row][col] += m1.elements[row][k] * m2.elements[k][col];
            }
        }
    }
    return result;
}
vector4 mul(mat4x4 m, vector4 v) {
    vector4 result = VECTOR4_ZERO;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.elements[row] += m.elements[row][col] * v.elements[col];
        }
    }
    return result;
}

mat3x3 transpose(mat3x3 m) {
    mat3x3 result;
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            result.elements[column][row] = m.elements[row][column];
        }
    }
    return result;
}
mat4x4 transpose(mat4x4 m) {
    mat4x4 result;
    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            result.elements[column][row] = m.elements[row][column];
        }
    }
    return result;
}

extern mat3x3 mat3x3_multiply_scalar(mat3x3 m, float scalar);
extern mat4x4 mat4x4_multiply_scalar(mat4x4 m, float scalar);

mat4x4 inverse(mat4x4 m) {
    const float a11 = m.elements[0][0], a12 = m.elements[0][1],
        a13 = m.elements[0][2], a14 = m.elements[0][3];
    const float a21 = m.elements[1][0], a22 = m.elements[1][1],
        a23 = m.elements[1][2], a24 = m.elements[1][3];
    const float a31 = m.elements[2][0], a32 = m.elements[2][1],
        a33 = m.elements[2][2], a34 = m.elements[2][3];
    const float a41 = m.elements[3][0], a42 = m.elements[3][1],
        a43 = m.elements[3][2], a44 = m.elements[3][3];

    // Uses the adjugate of the matrix to calculates the inverse.
    mat4x4 adj;
    adj.elements[0][0] = a22 * a33 * a44 + a23 * a34 * a42 + a24 * a32 * a43 -
        a24 * a33 * a42 - a23 * a32 * a44 - a22 * a34 * a43;
    adj.elements[0][1] = -a12 * a33 * a44 - a13 * a34 * a42 - a14 * a32 * a43 +
        a14 * a33 * a42 + a13 * a32 * a44 + a12 * a34 * a43;
    adj.elements[0][2] = a12 * a23 * a44 + a13 * a24 * a42 + a14 * a22 * a43 -
        a14 * a23 * a42 - a13 * a22 * a44 - a12 * a24 * a43;
    adj.elements[0][3] = -a12 * a23 * a34 - a13 * a24 * a32 - a14 * a22 * a33 +
        a14 * a23 * a32 + a13 * a22 * a34 + a12 * a24 * a33;

    adj.elements[1][0] = -a21 * a33 * a44 - a23 * a34 * a41 - a24 * a31 * a43 +
        a24 * a33 * a41 + a23 * a31 * a44 + a21 * a34 * a43;
    adj.elements[1][1] = a11 * a33 * a44 + a13 * a34 * a41 + a14 * a31 * a43 -
        a14 * a33 * a41 - a13 * a31 * a44 - a11 * a34 * a43;
    adj.elements[1][2] = -a11 * a23 * a44 - a13 * a24 * a41 - a14 * a21 * a43 +
        a14 * a23 * a41 + a13 * a21 * a44 + a11 * a24 * a43;
    adj.elements[1][3] = a11 * a23 * a34 + a13 * a24 * a31 + a14 * a21 * a33 -
        a14 * a23 * a31 - a13 * a21 * a34 - a11 * a24 * a33;

    adj.elements[2][0] = a21 * a32 * a44 + a22 * a34 * a41 + a24 * a31 * a42 -
        a24 * a32 * a41 - a22 * a31 * a44 - a21 * a34 * a42;
    adj.elements[2][1] = -a11 * a32 * a44 - a12 * a34 * a41 - a14 * a31 * a42 +
        a14 * a32 * a41 + a12 * a31 * a44 + a11 * a34 * a42;
    adj.elements[2][2] = a11 * a22 * a44 + a12 * a24 * a41 + a14 * a21 * a42 -
        a14 * a22 * a41 - a12 * a21 * a44 - a11 * a24 * a42;
    adj.elements[2][3] = -a11 * a22 * a34 - a12 * a24 * a31 - a14 * a21 * a32 +
        a14 * a22 * a31 + a12 * a21 * a34 + a11 * a24 * a32;

    adj.elements[3][0] = -a21 * a32 * a43 - a22 * a33 * a41 - a23 * a31 * a42 +
        a23 * a32 * a41 + a22 * a31 * a43 + a21 * a33 * a42;
    adj.elements[3][1] = a11 * a32 * a43 + a12 * a33 * a41 + a13 * a31 * a42 -
        a13 * a32 * a41 - a12 * a31 * a43 - a11 * a33 * a42;
    adj.elements[3][2] = -a11 * a22 * a43 - a12 * a23 * a41 - a13 * a21 * a42 +
        a13 * a22 * a41 + a12 * a21 * a43 + a11 * a23 * a42;
    adj.elements[3][3] = a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 -
        a13 * a22 * a31 - a12 * a21 * a33 - a11 * a23 * a32;

    float determinant = a11 * adj.elements[0][0] + a21 * adj.elements[0][1] +
        a31 * adj.elements[0][2] + a41 * adj.elements[0][3];

    if (determinant == 0.0f) {
        // The matrix is not invertible.
        return MATRIX4x4_ZERO;
    }
    return mat4x4_multiply_scalar(adj, 1.0f / determinant);
}

mat4x4 mat4x4_translate(vector3 translation) {
    mat4x4 result = MATRIX4x4_IDENTITY;
    result.elements[0][3] = translation.x;
    result.elements[1][3] = translation.y;
    result.elements[2][3] = translation.z;
    return result;
}
mat4x4 mat4x4_scale(vector3 scaling) {
    mat4x4 result = MATRIX4x4_IDENTITY;
    result.elements[0][0] = scaling.x;
    result.elements[1][1] = scaling.y;
    result.elements[2][2] = scaling.z;
    return result;
}


mat4x4 mat4x4_rotate_x(float angle)
{
    mat4x4 result = MATRIX4x4_IDENTITY;
    float radius = (angle / 180.0f) * PI;
    float c = cosf(radius);
    float s = sinf(radius);
    result.elements[1][1] = c;
    result.elements[1][2] = -s;
    result.elements[2][1] = s;
    result.elements[2][2] = c;
    return result;
}

mat4x4 mat4x4_rotate_y(float angle)
{
    mat4x4 result = MATRIX4x4_IDENTITY;
    float radius = (angle / 180.0f) * PI;
    float c = cosf(radius);
    float s = sinf(radius);
    result.elements[0][0] = c;
    result.elements[0][2] = s;
    result.elements[2][0] = -s;
    result.elements[2][2] = c;
    return result;
}

mat4x4 mat4x4_rotate_z(float angle)
{
    mat4x4 result = MATRIX4x4_IDENTITY;
    float radius = (angle / 180.0f) * PI;
    float c = cosf(radius);
    float s = sinf(radius);
    result.elements[0][0] = c;
    result.elements[0][1] = -s;
    result.elements[1][0] = s;
    result.elements[1][1] = c;
    return result;
}

mat4x4 mat4x4_rotate(vector3 axis, float angle)
{
    if (axis == vector3{0, 1, 0})
        return mat4x4_rotate_y(angle);
    if (axis == vector3{1, 0, 0})
        return mat4x4_rotate_x(angle);
    if (axis == vector3{0, 0, 1})
        return mat4x4_rotate_z(angle);
    mat4x4 result = MATRIX4x4_IDENTITY;
    float radius = (angle / 180.0f) * PI;
    float c = cosf(radius);
    float s = sinf(radius);
    axis = normalize(axis);
    float nc = 1 - c;
    float xy = axis.x * axis.y;
    float yz = axis.y * axis.z;
    float zx = axis.z * axis.x;
    float xs = axis.x * s;
    float ys = axis.y * s;
    float zs = axis.z * s;

    result.elements[0][0] = axis.x * axis.x * nc + c;
    result.elements[0][1] = xy * nc - zs;
    result.elements[0][2] = zx * nc + ys;

    result.elements[1][0] = xy * nc + zs;
    result.elements[1][1] = axis.y * axis.y * nc + c;
    result.elements[1][2] = yz * nc - xs;

    result.elements[2][0] = zx * nc - ys;
    result.elements[2][1] = yz * nc + xs;
    result.elements[2][2] = axis.z * axis.z * nc + c;

    return result;
}

mat4x4 mat4x4_W2V(vector3 from, vector3 to, vector3 up)
{
    mat4x4 result = MATRIX4x4_IDENTITY;
    vector3 z = normalize(from - to);
    vector3 x = normalize(cross(up, z));
    vector3 y = cross(z, x);
    result.elements[0][0] = x.x;
    result.elements[1][0] = y.x;
    result.elements[2][0] = z.x;

    result.elements[0][1] = x.y;
    result.elements[1][1] = y.y;
    result.elements[2][1] = z.y;

    result.elements[0][2] = x.z;
    result.elements[1][2] = y.z;
    result.elements[2][2] = z.z;

    result.elements[0][3] = -dot(x,from);
    result.elements[1][3] = -dot(y, from);
    result.elements[2][3] = -dot(z, from);
    return result;
}

mat4x4 mat4x4_orthographic(float right, float top, float near,
    float far) {
    mat4x4 result = MATRIX4x4_IDENTITY;
    float fn = far - near;
    result.elements[0][0] = 1.0f / right;
    result.elements[1][1] = 1.0f / top;
    result.elements[2][2] = -2.0f / fn;
    result.elements[2][3] = (-near - far) / fn;
    return result;
}

mat4x4 mat4x4_perspective(float fov, float aspect, float near,
    float far) {
    mat4x4 result = MATRIX4x4_ZERO;
    float fn = far - near;
    result.elements[1][1] = 1.0f / (tanf(fov / 2.0f));
    result.elements[0][0] = result.elements[1][1] / aspect;
    result.elements[2][2] = (-far - near) / fn;
    result.elements[2][3] = (-2.0f * far * near) / fn;
    result.elements[3][2] = -1.0f;
    return result;
}

mat4x4 mat4x4_W2L(vector3 lightDir) {
    mat4x4 result = MATRIX4x4_IDENTITY;
    vector3 z = normalize(lightDir);
    vector3 x = normalize( cross(vector3{0, 1.0f, 0}, z));
    vector3 y = cross(z, x);
    result.elements[0][0] = x.x;
    result.elements[1][0] = y.x;
    result.elements[2][0] = z.x;

    result.elements[0][1] = x.y;
    result.elements[1][1] = y.y;
    result.elements[2][1] = z.y;

    result.elements[0][2] = x.z;
    result.elements[1][2] = y.z;
    result.elements[2][2] = z.z;
    return result;
}