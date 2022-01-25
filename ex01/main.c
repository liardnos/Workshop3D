#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SFML/Graphics.h>
#include <SFML/Window/Keyboard.h>

#include "mat4x4.h"
#include "utils.h"

#define PI 3.14159265359

typedef struct Triangle {
    uint32_t _p[4];
    uint8_t _color[4];
} Triangle_t;

typedef struct Point {
    float p[3];
} Point_t;

typedef struct Mesh {
    
    Triangle_t *tris;
    size_t tris_size;

    Point_t *points;
    size_t points_size;

} Mesh_t;

Mesh_t *mesh_create() {
    return malloc(sizeof(Mesh_t));
}

void mesh_destroy(Mesh_t * mesh) {
    free(mesh->points);
    free(mesh->tris);
    free(mesh);
}

float dotProduct3(float *vec1, float *vec2) {
    return vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2];
}

void crossProduct3(float *dest, float *vec1, float *vec2) {
    dest[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    dest[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    dest[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}


void normalize(float *vec) {
    float len = pow(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2], 0.5);
    vec[0] /= len;
    vec[1] /= len;
    vec[2] /= len;
}


int main() {

    sfVideoMode mode = {800, 800, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Max & Flo 3D workshop", sfResize | sfClose, NULL);
    if (!window)
        return 84;
    sfWindow_setVerticalSyncEnabled((sfWindow *)window, 1);

    /*
     4---5
    0---1|
    |6--|7
    2---3
    */


   // create mesh
    Point_t cube_points[8] = {
        {{-0.5, -0.5, -0.5}},
        {{ 0.5, -0.5, -0.5}},
        {{-0.5,  0.5, -0.5}},
        {{ 0.5,  0.5, -0.5}},
        {{-0.5, -0.5,  0.5}},
        {{ 0.5, -0.5,  0.5}},
        {{-0.5,  0.5,  0.5}},
        {{ 0.5,  0.5,  0.5}},
    };

    Triangle_t cube_tris[] = {
        {{1, 0, 2}, {255, 000, 000, 255}},
        {{3, 1, 2}, {255, 000, 000, 255}},
        {{5, 4, 0}, {000, 255, 000, 255}},
        {{1, 5, 0}, {000, 255, 000, 255}},
        {{5, 1, 7}, {000, 000, 255, 255}},
        {{3, 7, 1}, {000, 000, 255, 255}},
        {{2, 6, 3}, {255, 255, 000, 255}},
        {{7, 3, 6}, {255, 255, 000, 255}},
        {{4, 5, 6}, {255, 000, 255, 255}},
        {{7, 6, 5}, {255, 000, 255, 255}},
        {{0, 4, 2}, {000, 255, 255, 255}},
        {{6, 2, 4}, {000, 255, 255, 255}},
    };

    Mesh_t mesh_cube;
    mesh_cube.points_size = 8;
    mesh_cube.points = cube_points;
    mesh_cube.tris_size = 12;
    mesh_cube.tris = cube_tris;
    Mesh_t *mesh = &mesh_cube;
    //


    Mat4x4 *matWorld = mat4x4_create();

    for (int frame_count = 0; sfRenderWindow_isOpen(window); ++frame_count) {
        printf("frameframe_count=%i\n", frame_count);
        { // event sfml
            sfEvent event;
            while (sfRenderWindow_pollEvent(window, &event)) {
                /* Close window : exit */
                if (event.type == sfEvtClosed)
                    sfRenderWindow_close(window);
            }
        } //

        { // input clavier
            if (sfKeyboard_isKeyPressed(sfKeyZ)) {
                float vec[3] = {0, 0, 0.5};
                Mat4x4 *matMov = mat4x4_t(vec);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyS)) {
                float vec[3] = {0, 0, -0.5};
                Mat4x4 *matMov = mat4x4_t(vec);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyQ)) {
                float vec[3] = {-0.5, 0, 0};
                Mat4x4 *matMov = mat4x4_t(vec);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyD)) {
                float vec[3] = {0.5, 0, 0};
                Mat4x4 *matMov = mat4x4_t(vec);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyA)) {
                float vec[3] = {0, 0.5, 0};
                Mat4x4 *matMov = mat4x4_t(vec);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyE)) {
                float vec[3] = {0, -0.5, 0};
                Mat4x4 *matMov = mat4x4_t(vec);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyW)) {
                Mat4x4 *matMov = mat4x4_rz(PI/180);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyX)) {
                Mat4x4 *matMov = mat4x4_rz(-PI/180);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyJ)) {
                Mat4x4 *matMov = mat4x4_ry(-PI/180);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyL)) {
                Mat4x4 *matMov = mat4x4_ry(PI/180);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyI)) {
                Mat4x4 *matMov = mat4x4_rx(-PI/180);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
            if (sfKeyboard_isKeyPressed(sfKeyK)) {
                Mat4x4 *matMov = mat4x4_rx(PI/180);
                Mat4x4 *tmp = matWorld;
                matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
                Mat4x4_destroy(matMov);
                Mat4x4_destroy(tmp);
            }
        } //

        // projection
        Point_t *points_transformed = malloc(sizeof(Point_t)*mesh->points_size);
        for (size_t i = 0; i < mesh->points_size; ++i) {
            float *vec = mat4x4_MultiplyVector3(matWorld, mesh->points[i].p);
            memcpy(points_transformed[i].p, vec, sizeof(float)*3);
            free(vec);
        }
        //

        // sort all triangles
        typedef struct VecSort {
            uint id;
            int d;
        } VecSort_t;

        VecSort_t *sortBuffer = malloc(sizeof(VecSort_t)*mesh->tris_size*2);

        for (uint i = 0; mesh->tris_size > i; i++) {
            float pMid[3] = {
                points_transformed[mesh->tris[i]._p[0]].p[0] + points_transformed[mesh->tris[i]._p[1]].p[0] + points_transformed[mesh->tris[i]._p[2]].p[0],
                points_transformed[mesh->tris[i]._p[0]].p[1] + points_transformed[mesh->tris[i]._p[1]].p[1] + points_transformed[mesh->tris[i]._p[2]].p[1],
                points_transformed[mesh->tris[i]._p[0]].p[2] + points_transformed[mesh->tris[i]._p[1]].p[2] + points_transformed[mesh->tris[i]._p[2]].p[2],
            };

            sortBuffer[i].id = i;
            sortBuffer[i].d = -(pMid[0]*pMid[0] + pMid[1]*pMid[1] + pMid[2]*pMid[2]);
        }

        radixSortObjInt(sortBuffer, sizeof(VecSort_t), mesh->tris_size, (size_t)&((VecSort_t *)0)->d, sortBuffer+mesh->tris_size);
        //


        // FOV
        for (size_t i = 0; i < mesh->points_size; ++i) {
            points_transformed[i].p[0] /= points_transformed[i].p[2];
            points_transformed[i].p[1] /= points_transformed[i].p[2];
        }
        //

        sfConvexShape *shape = sfConvexShape_create();
        sfConvexShape_setPointCount(shape, 3);

        // for each triangle in mesh
        for (size_t i = 0; i < mesh->tris_size; ++i) {
            size_t id = sortBuffer[i].id;

            // get the 3 points
            float *vec_transformed[3] = {
                points_transformed[mesh->tris[id]._p[0]].p,
                points_transformed[mesh->tris[id]._p[1]].p,
                points_transformed[mesh->tris[id]._p[2]].p,
            };
            //

            // Center drawings
            sfVector2f vec[] = {
                {(vec_transformed[0][0]+1)*(mode.width/2), (vec_transformed[0][1]+1)*(mode.height/2)},
                {(vec_transformed[1][0]+1)*(mode.width/2), (vec_transformed[1][1]+1)*(mode.height/2)},
                {(vec_transformed[2][0]+1)*(mode.width/2), (vec_transformed[2][1]+1)*(mode.height/2)},
            };
            //

            sfColor color;
            memcpy(&color, mesh->tris[id]._color, sizeof(sfColor));


            // draw shape
            sfConvexShape_setPoint(shape, 0, vec[0]);
            sfConvexShape_setPoint(shape, 1, vec[1]);
            sfConvexShape_setPoint(shape, 2, vec[2]);
            sfConvexShape_setFillColor(shape, color);
            sfRenderWindow_drawConvexShape(window, shape, 0);
            //
        }
        //

        free(sortBuffer);
        free(points_transformed);

        sfConvexShape_destroy(shape);

        sfRenderWindow_display(window);
        sfRenderWindow_clear(window, (sfColor){0, 0, 0, 0});
    }
    
    Mat4x4_destroy(matWorld);
    sfRenderWindow_destroy(window);
    return 0;
}