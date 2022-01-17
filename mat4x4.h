#pragma once

struct Mat4x4 {
    float val[16];
};

typedef struct Mat4x4 Mat4x4;

Mat4x4 *mat4x4_create();
void Mat4x4_destroy(Mat4x4 *mat);
Mat4x4 *mat4x4_MultiplyMat4x4(Mat4x4 *mat1, Mat4x4 *mat2);
float *mat4x4_MultiplyVector(Mat4x4 *mat, float *vector);
Mat4x4 *mat4x4_rx(float a);
Mat4x4 *mat4x4_ry(float a);
Mat4x4 *mat4x4_rz(float a);
Mat4x4 *mat4x4_t(float *vector);
Mat4x4 *mat4x4_scale(float s);
Mat4x4 *mat4x4_invert(Mat4x4 *mat);
void Mat4x4_print(Mat4x4* matrix);