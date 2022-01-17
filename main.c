#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <SFML/Graphics.h>
#include <SFML/Window/Keyboard.h>

#include "mat4x4.h"

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

            point_count++;
        } else if (line[0] == 'f') {
            uint f[3];
            line += 2;
            char *endptr = line;

            mesh->tris[point_count]._p[0] = strtof(line, &endptr);
            line = endptr;
            mesh->tris[point_count]._p[1] = strtof(line, &endptr);
            line = endptr;
            mesh->tris[point_count]._p[2] = strtof(line, &endptr);
            line = endptr;

            mesh->tris[point_count]._color[0] = (unsigned char)rand();
            mesh->tris[point_count]._color[1] = (unsigned char)rand();
            mesh->tris[point_count]._color[2] = (unsigned char)rand();
            mesh->tris[point_count]._color[3] = 255;
            
            tri_count++;
        }
        free(line_back);
    }

    printf("filename: %s\n", filename);
    printf("points: %li\n", mesh->points_size);
    printf("tris  : %li\n", mesh->tris_size);
    return mesh;
}

int main() {
    Mesh_t *mesh = LoadFromObjectFile("axis.obj");

    sfVideoMode mode = {800, 600, 32};
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
            float vec[3] = {0, 0, 1};
            Mat4x4 *matMov = mat4x4_t(vec);
            Mat4x4 *tmp = matWorld;
            matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
            Mat4x4_destroy(matMov);
            Mat4x4_destroy(tmp);
        }
        if (sfKeyboard_isKeyPressed(sfKeyS)) {
            float vec[3] = {0, 0, -1};
            Mat4x4 *matMov = mat4x4_t(vec);
            Mat4x4 *tmp = matWorld;
            matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
            Mat4x4_destroy(matMov);
            Mat4x4_destroy(tmp);
        }
        if (sfKeyboard_isKeyPressed(sfKeyQ)) {
            float vec[3] = {1, 0, 0};
            Mat4x4 *matMov = mat4x4_t(vec);
            Mat4x4 *tmp = matWorld;
            matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
            Mat4x4_destroy(matMov);
            Mat4x4_destroy(tmp);
        }
        if (sfKeyboard_isKeyPressed(sfKeyD)) {
            float vec[3] = {-1, 0, 0};
            Mat4x4 *matMov = mat4x4_t(vec);
            Mat4x4 *tmp = matWorld;
            matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
            Mat4x4_destroy(matMov);
            Mat4x4_destroy(tmp);
        }
        if (sfKeyboard_isKeyPressed(sfKeyE)) {
            float vec[3] = {0, 1, 0};
            Mat4x4 *matMov = mat4x4_t(vec);
            Mat4x4 *tmp = matWorld;
            matWorld = mat4x4_MultiplyMat4x4(matWorld, matMov);
            Mat4x4_destroy(matMov);
            Mat4x4_destroy(tmp);
        }
        if (sfKeyboard_isKeyPressed(sfKeyA)) {
            float vec[3] = {0, -1, 0};
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
        Point_t *buffer = malloc(sizeof(Point_t)*mesh->points_size);
        for (size_t i = 0; i < mesh->points_size; ++i) {
            //float *vec = mat4x4_MultiplyVector4(mesh->points[i].p);
            //buffer[i] = 1;
        }

        sfConvexShape *shape = sfConvexShape_create();
        sfConvexShape_setPointCount(shape, 3);
        sfVector2f vec[] = {
            {100, 100},
            {200, 100},
            {100, 200}
        };
        sfConvexShape_setPoint(shape, 0, vec[0]);
        sfConvexShape_setPoint(shape, 1, vec[1]);
        sfConvexShape_setPoint(shape, 2, vec[2]);
        sfColor color = {255, 255, 255, 255};
        sfConvexShape_setFillColor(shape, color);
        sfRenderWindow_drawConvexShape(window, shape, 0);

        sfConvexShape_destroy(shape);

        sfRenderWindow_display(window);
    }

    Mat4x4_destroy(matWorld);
    mesh_destroy(mesh);
    sfRenderWindow_destroy(window);
    return 0;
}