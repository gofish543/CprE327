#include "staircase.h"

Staircase* staircase_initialize(u_char x, u_char y, u_char fromFloor, u_char toFloor) {
    Staircase* staircase = malloc(sizeof(Staircase));

    staircase->x = x;
    staircase->y = y;

    staircase->fromFloor = fromFloor;
    staircase->toFloor = toFloor;

    if (fromFloor < toFloor) {
        staircase->isDown = false;
        staircase->isUp = true;
    } else if (fromFloor > toFloor) {
        staircase->isDown = true;
        staircase->isUp = false;
    } else {
        staircase->isDown = false;
        staircase->isUp = false;
    }

    return staircase;
}

Staircase* staircase_terminate(Staircase* staircase) {
    free(staircase);

    return NULL;
}