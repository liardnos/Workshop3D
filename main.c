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


Mesh_t * LoadFromObjectFile(char *filename) {
    FILE *fptr = fopen(filename, "r");
    if (!fptr)
        return 0;

    Mesh_t *mesh = malloc(sizeof(Mesh_t));
    mesh->points_size = 0;
    mesh->tris_size = 0;

    while (1) {
        char *line = 0;
        size_t len2 = 0;
        size_t len = getline(&line, &len2, fptr);
        if ((long int)len <= 0) {
            free(line);
            break;
        }
        char *line_back = line;
        if (line[0] == 'v') {
            mesh->points_size++;
        } else if (line[0] == 'f') {
            mesh->tris_size++;
        }
        free(line_back);
    }

    mesh->points = malloc(mesh->points_size * sizeof(Point_t));
    mesh->tris = malloc(mesh->tris_size * sizeof(Triangle_t));

    rewind(fptr);

    size_t point_count = 0;
    size_t tri_count = 0;
    while (1) {
        char *line = 0;
        size_t len2 = 0;
        size_t len = getline(&line, &len2, fptr);
        if ((long int)len <= 0) {
            free(line);
            break;
        }
        char *line_back = line;
        if (line[0] == 'v') {
            line += 2;
            char *endptr = line;

            mesh->points[point_count].p[0] = strtof(line, &endptr);
            line = endptr;
            mesh->points[point_count].p[1] = strtof(line, &endptr);
            line = endptr;
            mesh->points[point_count].p[2] = strtof(line, &endptr);
            line = endptr;
            //printf("%f %f %f\n", mesh->points[point_count].p[0], mesh->points[point_count].p[1], mesh->points[point_count].p[2]);

            point_count++;
        } else if (line[0] == 'f') {
            uint f[3];
            line += 2;
            char *endptr = line;

            mesh->tris[tri_count]._p[0] = strtol(line, &endptr, 0)-1;
            line = endptr;
            mesh->tris[tri_count]._p[1] = strtol(line, &endptr, 0)-1;
            line = endptr;
            mesh->tris[tri_count]._p[2] = strtol(line, &endptr, 0)-1;
            line = endptr;

            //printf("%i %i %i\n", mesh->tris[tri_count]._p[0], mesh->tris[tri_count]._p[1], mesh->tris[tri_count]._p[2]);

            mesh->tris[tri_count]._color[0] = 255;(unsigned char)rand();
            mesh->tris[tri_count]._color[1] = 255;(unsigned char)rand();
            mesh->tris[tri_count]._color[2] = 255;(unsigned char)rand();
            mesh->tris[tri_count]._color[3] = 255;
            
            tri_count++;
        }
        free(line_back);
    }

    printf("filename: %s\n", filename);
    printf("points: %li\n", mesh->points_size);
    printf("tris  : %li\n", mesh->tris_size);
    return mesh;
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

size_t concatMeshs(Mesh_t *meshDest, Mesh_t *mesh1, Mesh_t *mesh2) {
    meshDest->points_size = mesh1->points_size + mesh2->points_size;
    meshDest->points = malloc(meshDest->points_size*sizeof(Point_t));
    meshDest->tris_size = mesh1->tris_size + mesh2->tris_size;
    meshDest->tris = malloc(meshDest->tris_size*sizeof(Triangle_t));

    memcpy(meshDest->points                   , mesh1->points, mesh1->points_size*sizeof(Point_t));
    memcpy(meshDest->points+mesh1->points_size, mesh2->points, mesh2->points_size*sizeof(Point_t));

    memcpy(meshDest->tris                 , mesh1->tris, mesh1->tris_size*sizeof(Triangle_t));
    memcpy(meshDest->tris+mesh1->tris_size, mesh2->tris, mesh2->tris_size*sizeof(Triangle_t));
    
    for (size_t i = 0; i < mesh2->tris_size; ++i) {
        (meshDest->tris+mesh1->tris_size)[i]._p[0] += mesh1->points_size;
        (meshDest->tris+mesh1->tris_size)[i]._p[1] += mesh1->points_size;
        (meshDest->tris+mesh1->tris_size)[i]._p[2] += mesh1->points_size;
    }

    return mesh1->points_size;
}

int main() {
    Mesh_t *mesh_axis = LoadFromObjectFile("axis.obj");


    /*
     4---5
    0---1|
    |6--|7
    2---3
    */

    Point_t cube_points[8] = {
        {-0.5, -0.5, -0.5},
        { 0.5, -0.5, -0.5},
        {-0.5,  0.5, -0.5},
        { 0.5,  0.5, -0.5},
        {-0.5, -0.5,  0.5},
        { 0.5, -0.5,  0.5},
        {-0.5,  0.5,  0.5},
        { 0.5,  0.5,  0.5},
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

    Mesh_t mesh_cube;;
    mesh_cube.points_size = 8;
    mesh_cube.points = cube_points;
    mesh_cube.tris_size = 12;
    mesh_cube.tris = cube_tris;

    Mesh_t *mesh = mesh_create();
    size_t mesh_cube_offset = concatMeshs(mesh, mesh_axis, &mesh_cube);
    mesh_destroy(mesh_axis);

    sfVideoMode mode = {800, 800, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Max & Flo 3D workshop", sfResize | sfClose, NULL);
    if (!window)
        return 84;
    
    sfWindow_setVerticalSyncEnabled((sfWindow *)window, 1);

    Mat4x4 *matWorld = mat4x4_create();

    for (int frame_count = 0; sfRenderWindow_isOpen(window); ++frame_count) {
        {
            sfEvent event;
            while (sfRenderWindow_pollEvent(window, &event)) {
                /* Close window : exit */
                if (event.type == sfEvtClosed)
                    sfRenderWindow_close(window);
            }
        }

        printf("frameframe_count=%i\n", frame_count);
        Mat4x4_print(matWorld);
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

        // calc player pos
        Mat4x4 *matWorld_inv = mat4x4_invert(matWorld);
        float playerPos[3] = {0, 0, 0};
        float *tmp = mat4x4_MultiplyVector3(matWorld_inv, playerPos);
        memcpy(playerPos, tmp, sizeof(playerPos));
        free(tmp);

        // calc player direction
        float playerDir[3] = {0, 0, 1};
        tmp = mat4x4_MultiplyVector3(matWorld_inv, playerDir);
        memcpy(playerDir, tmp, sizeof(playerPos));
        playerDir[0] -= playerPos[0];
        playerDir[1] -= playerPos[1];
        playerDir[2] -= playerPos[2];
        free(tmp);
        free(matWorld_inv);


        printf("player pos=%f %f %f\n", playerPos[0], playerPos[1], playerPos[2]);
        printf("player dir=%f %f %f\n", playerDir[0], playerDir[1], playerDir[2]);

        // set light source
        float lightSource[3] = {
            playerPos[0] - playerDir[0]*10, 
            playerPos[1] - playerDir[1]*10,
            playerPos[2] - playerDir[2]*10,
        };
        //

        // move light cube
        for (size_t i = 0; i < 8; i++) {
            mesh->points[mesh_cube_offset+i].p[0] = cube_points[i].p[0] + playerPos[0] - playerDir[0]*10;
            mesh->points[mesh_cube_offset+i].p[1] = cube_points[i].p[1] + playerPos[1] - playerDir[1]*10;
            mesh->points[mesh_cube_offset+i].p[2] = cube_points[i].p[2] + playerPos[2] - playerDir[2]*10;
        }
        //



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
        for (size_t i = 0; i < mesh->tris_size; ++i) {
            size_t id = sortBuffer[i].id;

            float *vec_transformed[3] = {
                points_transformed[mesh->tris[id]._p[0]].p,
                points_transformed[mesh->tris[id]._p[1]].p,
                points_transformed[mesh->tris[id]._p[2]].p
            };

            float *vec_original[3] = {
                mesh->points[mesh->tris[id]._p[0]].p,
                mesh->points[mesh->tris[id]._p[1]].p,
                mesh->points[mesh->tris[id]._p[2]].p
            };

            // is shape partialy behind?
            if (vec_transformed[0][2] > 0 || vec_transformed[1][2] > 0 || vec_transformed[2][2] > 0)
                continue;
            //

            { 
                // calc triangle normal (perpenticular segment)
                float vecs[2][3] = {
                    {
                        vec_transformed[0][0] - vec_transformed[1][0],
                        vec_transformed[0][1] - vec_transformed[1][1],
                        vec_transformed[0][2] - vec_transformed[1][2],
                    },
                    {
                        vec_transformed[0][0] - vec_transformed[2][0],
                        vec_transformed[0][1] - vec_transformed[2][1],
                        vec_transformed[0][2] - vec_transformed[2][2],
                    },
                };

                normalize(vecs[0]);
                normalize(vecs[1]);
                float normale[3];
                crossProduct3(normale, vecs[0], vecs[1]);
                normalize(normale);
                //

                // is shape facing backward?
                float cameraDirection[3] = {0, 0, 1};
                float direction = dotProduct3(normale, cameraDirection);

                if (direction < 0)
                    continue;
                //
            } 

            // FOV
            sfVector2f vec[] = {
                {(vec_transformed[0][0]+1)*(mode.width/2), (vec_transformed[0][1]+1)*(mode.height/2)},
                {(vec_transformed[1][0]+1)*(mode.width/2), (vec_transformed[1][1]+1)*(mode.height/2)},
                {(vec_transformed[2][0]+1)*(mode.width/2), (vec_transformed[2][1]+1)*(mode.height/2)},
            };
            //

            sfColor color;
            memcpy(&color, mesh->tris[id]._color, sizeof(sfColor));


            { // light
                float vecs[2][3] = {
                    {
                        vec_original[0][0] - vec_original[1][0],
                        vec_original[0][1] - vec_original[1][1],
                        vec_original[0][2] - vec_original[1][2],
                    },
                    {
                        vec_original[0][0] - vec_original[2][0],
                        vec_original[0][1] - vec_original[2][1],
                        vec_original[0][2] - vec_original[2][2],
                    },
                };

                normalize(vecs[0]);
                normalize(vecs[1]);
                float normale[3];
                crossProduct3(normale, vecs[0], vecs[1]);
                normalize(normale);
                float light_direction[3] = {
                    lightSource[0] - (vec_original[0][0] + vec_original[1][0] + vec_original[2][0])/3,
                    lightSource[1] - (vec_original[0][1] + vec_original[1][1] + vec_original[2][1])/3,
                    lightSource[2] - (vec_original[0][2] + vec_original[1][2] + vec_original[2][2])/3,
                };
                normalize(light_direction);


                // shape sun exposure
                float direction = dotProduct3(normale, light_direction);
                if (direction < 0)
                    direction = 0;
                //
                printf("%f\n", direction);
                
                color.r *= (direction*0.8+0.1);
                color.g *= (direction*0.8+0.1);
                color.b *= (direction*0.8+0.1);

            } //


            // draw shape
            sfConvexShape_setPoint(shape, 0, vec[0]);
            sfConvexShape_setPoint(shape, 1, vec[1]);
            sfConvexShape_setPoint(shape, 2, vec[2]);

            sfConvexShape_setFillColor(shape, color);
            sfRenderWindow_drawConvexShape(window, shape, 0);
            //
        }

        free(sortBuffer);
        free(points_transformed);

        sfConvexShape_destroy(shape);

        sfRenderWindow_display(window);
        sfRenderWindow_clear(window, (sfColor){0, 0, 0, 0});
    }

    Mat4x4_destroy(matWorld);
    mesh_destroy(mesh);
    sfRenderWindow_destroy(window);
    return 0;
}