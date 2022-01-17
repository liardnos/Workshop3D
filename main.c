#include <SFML/Graphics.h>
#include <SFML/Window/Keyboard.h>

#include "mat4x4.h"

#define PI 3.14159265359

int main() {
    Mat4x4 *matWorld = mat4x4_create();

    while (1) {
        if (sfKeyboard_isKeyPressed(sfKeyZ)) {
            mat4x4_tz(1);
        }
        if (sfKeyboard_isKeyPressed(sfKeyS)) {
            mat4x4_tz(-1);
        }
        if (sfKeyboard_isKeyPressed(sfKeyQ)) {
            mat4x4_tx(1);
        }
        if (sfKeyboard_isKeyPressed(sfKeyD)) {
            mat4x4_tx(-1);
        }
        if (sfKeyboard_isKeyPressed(sfKeyE)) {
            mat4x4_ty(1);
        }
        if (sfKeyboard_isKeyPressed(sfKeyA)) {
            mat4x4_ty(-1);
        }
        if (sfKeyboard_isKeyPressed(sfKeyW)) {
            mat4x4_rz(PI/180);
        }
        if (sfKeyboard_isKeyPressed(sfKeyX)) {
            mat4x4_rz(-PI/180);
        }
    }
    return 0;
}