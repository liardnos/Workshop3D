#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "mat4x4.h"


Mat4x4 *mat4x4_create() {
    Mat4x4 *mat = malloc(sizeof(Mat4x4));   
    memset(mat, 0, sizeof(Mat4x4));
    mat->val[0] = 1;
    mat->val[5] = 1;
    mat->val[10] = 1;
    mat->val[15] = 1;
    return mat;
}

void Mat4x4_destroy(Mat4x4 *mat) {
    free(mat);
}

Mat4x4 *mat4x4_MultiplyMat4x4(Mat4x4 *mat1, Mat4x4 *mat2) {
    Mat4x4 *mat = malloc(sizeof(Mat4x4));   
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            mat->val[r*4 + c] = mat1->val[r*4 + 0] * mat2->val[0*4 + c] + mat1->val[r*4 + 1] * mat2->val[1*4 + c] + mat1->val[r*4 + 2] * mat2->val[2*4 + c] + mat1->val[r*4 + 3] * mat2->val[3*4 + c];
    return mat;
}

float *mat4x4_MultiplyVector4(Mat4x4 *mat, float *vector) {
    float *res = malloc(sizeof(float)*4);
    res[0] = vector[0]*mat->val[0*4+0] + vector[1]*mat->val[1*4+0] + vector[2]*mat->val[2*4+0] + vector[3]*mat->val[3*4+0];
    res[1] = vector[0]*mat->val[0*4+1] + vector[1]*mat->val[1*4+1] + vector[2]*mat->val[2*4+1] + vector[3]*mat->val[3*4+1];
    res[2] = vector[0]*mat->val[0*4+2] + vector[1]*mat->val[1*4+2] + vector[2]*mat->val[2*4+2] + vector[3]*mat->val[3*4+2];
    res[3] = vector[0]*mat->val[0*4+3] + vector[1]*mat->val[1*4+3] + vector[2]*mat->val[2*4+3] + vector[3]*mat->val[3*4+3];    
    return res;
}

float *mat4x4_MultiplyVector3(Mat4x4 *mat, float *vector) {
    float *res = malloc(sizeof(float)*3);
    res[0] = vector[0]*mat->val[0*4+0] + vector[1]*mat->val[1*4+0] + vector[2]*mat->val[2*4+0];
    res[1] = vector[0]*mat->val[0*4+1] + vector[1]*mat->val[1*4+1] + vector[2]*mat->val[2*4+1];
    res[2] = vector[0]*mat->val[0*4+2] + vector[1]*mat->val[1*4+2] + vector[2]*mat->val[2*4+2];
    return res;
}

Mat4x4 *mat4x4_rx(float a) {
    Mat4x4 *mat = mat4x4_create();
    mat->val[1*4+1] = cosf(a);
    mat->val[1*4+2] = sinf(a);
    mat->val[2*4+1] = -sinf(a);
    mat->val[2*4+2] = cosf(a);
    return mat;
}   

Mat4x4 *mat4x4_ry(float a) {
    Mat4x4 *mat = mat4x4_create();
    mat->val[0*4+0] = cosf(a);
    mat->val[0*4+2] = sinf(a);
    mat->val[2*4+0] = -sinf(a);
    mat->val[2*4+2] = cosf(a);
    return mat;
}

Mat4x4 *mat4x4_rz(float a) {
    Mat4x4 *mat = mat4x4_create();
    mat->val[0*4+0] = cosf(a);
    mat->val[0*4+1] = sinf(a);
    mat->val[1*4+0] = -sinf(a);
    mat->val[1*4+1] = cosf(a);
    return mat;
}

Mat4x4 *mat4x4_t(float *vector) {
    Mat4x4 *mat = mat4x4_create();
    mat->val[3*4+0] = vector[0];
    mat->val[3*4+1] = vector[1];
    mat->val[3*4+2] = vector[2];
    return mat;
}

Mat4x4 *mat4x4_scale(float s) {
    Mat4x4 *mat = mat4x4_create();
    mat->val[0] = s;
    mat->val[5] = s;
    mat->val[10] = s;
    mat->val[15] = 1;
    return mat;
}

Mat4x4 *mat4x4_invert(Mat4x4 *mat) {
    Mat4x4 *matrix = mat4x4_create();
    float inv[16], det;

    inv[0]  =  mat->val[5] * mat->val[10] * mat->val[15] - mat->val[5] * mat->val[11] * mat->val[14] - mat->val[9] * mat->val[6] * mat->val[15] + mat->val[9] * mat->val[7] * mat->val[14] + mat->val[13] * mat->val[6] * mat->val[11] - mat->val[13] * mat->val[7] * mat->val[10];
    inv[4]  = -mat->val[4] * mat->val[10] * mat->val[15] + mat->val[4] * mat->val[11] * mat->val[14] + mat->val[8] * mat->val[6] * mat->val[15] - mat->val[8] * mat->val[7] * mat->val[14] - mat->val[12] * mat->val[6] * mat->val[11] + mat->val[12] * mat->val[7] * mat->val[10];
    inv[8]  =  mat->val[4] * mat->val[9]  * mat->val[15] - mat->val[4] * mat->val[11] * mat->val[13] - mat->val[8] * mat->val[5] * mat->val[15] + mat->val[8] * mat->val[7] * mat->val[13] + mat->val[12] * mat->val[5] * mat->val[11] - mat->val[12] * mat->val[7] * mat->val[9];
    inv[12] = -mat->val[4] * mat->val[9]  * mat->val[14] + mat->val[4] * mat->val[10] * mat->val[13] + mat->val[8] * mat->val[5] * mat->val[14] - mat->val[8] * mat->val[6] * mat->val[13] - mat->val[12] * mat->val[5] * mat->val[10] + mat->val[12] * mat->val[6] * mat->val[9];
    inv[1]  = -mat->val[1] * mat->val[10] * mat->val[15] + mat->val[1] * mat->val[11] * mat->val[14] + mat->val[9] * mat->val[2] * mat->val[15] - mat->val[9] * mat->val[3] * mat->val[14] - mat->val[13] * mat->val[2] * mat->val[11] + mat->val[13] * mat->val[3] * mat->val[10];
    inv[5]  =  mat->val[0] * mat->val[10] * mat->val[15] - mat->val[0] * mat->val[11] * mat->val[14] - mat->val[8] * mat->val[2] * mat->val[15] + mat->val[8] * mat->val[3] * mat->val[14] + mat->val[12] * mat->val[2] * mat->val[11] - mat->val[12] * mat->val[3] * mat->val[10];
    inv[9]  = -mat->val[0] * mat->val[9]  * mat->val[15] + mat->val[0] * mat->val[11] * mat->val[13] + mat->val[8] * mat->val[1] * mat->val[15] - mat->val[8] * mat->val[3] * mat->val[13] - mat->val[12] * mat->val[1] * mat->val[11] + mat->val[12] * mat->val[3] * mat->val[9];
    inv[13] =  mat->val[0] * mat->val[9]  * mat->val[14] - mat->val[0] * mat->val[10] * mat->val[13] - mat->val[8] * mat->val[1] * mat->val[14] + mat->val[8] * mat->val[2] * mat->val[13] + mat->val[12] * mat->val[1] * mat->val[10] - mat->val[12] * mat->val[2] * mat->val[9];
    inv[2]  =  mat->val[1] * mat->val[6]  * mat->val[15] - mat->val[1] * mat->val[7]  * mat->val[14] - mat->val[5] * mat->val[2] * mat->val[15] + mat->val[5] * mat->val[3] * mat->val[14] + mat->val[13] * mat->val[2] * mat->val[7]  - mat->val[13] * mat->val[3] * mat->val[6];
    inv[6]  = -mat->val[0] * mat->val[6]  * mat->val[15] + mat->val[0] * mat->val[7]  * mat->val[14] + mat->val[4] * mat->val[2] * mat->val[15] - mat->val[4] * mat->val[3] * mat->val[14] - mat->val[12] * mat->val[2] * mat->val[7]  + mat->val[12] * mat->val[3] * mat->val[6];
    inv[10] =  mat->val[0] * mat->val[5]  * mat->val[15] - mat->val[0] * mat->val[7]  * mat->val[13] - mat->val[4] * mat->val[1] * mat->val[15] + mat->val[4] * mat->val[3] * mat->val[13] + mat->val[12] * mat->val[1] * mat->val[7]  - mat->val[12] * mat->val[3] * mat->val[5];
    inv[14] = -mat->val[0] * mat->val[5]  * mat->val[14] + mat->val[0] * mat->val[6]  * mat->val[13] + mat->val[4] * mat->val[1] * mat->val[14] - mat->val[4] * mat->val[2] * mat->val[13] - mat->val[12] * mat->val[1] * mat->val[6]  + mat->val[12] * mat->val[2] * mat->val[5];
    inv[3]  = -mat->val[1] * mat->val[6]  * mat->val[11] + mat->val[1] * mat->val[7]  * mat->val[10] + mat->val[5] * mat->val[2] * mat->val[11] - mat->val[5] * mat->val[3] * mat->val[10] - mat->val[9]  * mat->val[2] * mat->val[7]  + mat->val[9]  * mat->val[3] * mat->val[6];
    inv[7]  =  mat->val[0] * mat->val[6]  * mat->val[11] - mat->val[0] * mat->val[7]  * mat->val[10] - mat->val[4] * mat->val[2] * mat->val[11] + mat->val[4] * mat->val[3] * mat->val[10] + mat->val[8]  * mat->val[2] * mat->val[7]  - mat->val[8]  * mat->val[3] * mat->val[6];
    inv[11] = -mat->val[0] * mat->val[5]  * mat->val[11] + mat->val[0] * mat->val[7]  * mat->val[9]  + mat->val[4] * mat->val[1] * mat->val[11] - mat->val[4] * mat->val[3] * mat->val[9]  - mat->val[8]  * mat->val[1] * mat->val[7]  + mat->val[8]  * mat->val[3] * mat->val[5];
    inv[15] =  mat->val[0] * mat->val[5]  * mat->val[10] - mat->val[0] * mat->val[6]  * mat->val[9]  - mat->val[4] * mat->val[1] * mat->val[10] + mat->val[4] * mat->val[2] * mat->val[9]  + mat->val[8]  * mat->val[1] * mat->val[6]  - mat->val[8]  * mat->val[2] * mat->val[5];

    det = mat->val[0] * inv[0] + mat->val[1] * inv[4] + mat->val[2] * inv[8] + mat->val[3] * inv[12];
    if (det == 0) 
        return matrix;
    det = 1.0 / det;

    int i;
    for (i = 0; i < 16; i++)
        matrix->val[i] = inv[i] * det;

    return matrix;
}

void Mat4x4_print(Mat4x4* matrix) {
    for (uint i = 0; i < 16; i++) {
        printf("%f ", matrix->val[i]);
        if (!((i+1) % 4)) 
            printf("\n");
    }
}