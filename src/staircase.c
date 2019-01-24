#include "staircase.h"

struct Staircase* staircase_initialize(unsigned short x, unsigned short y, unsigned short fromFloor, unsigned short toFloor) {
    struct Staircase* staircase = malloc(sizeof(struct Staircase));

    staircase->x = x;
    staircase->y = y;

    staircase->fromFloor = fromFloor;
    staircase->toFloor = toFloor;

    if(fromFloor < toFloor) {
        staircase->isDown = false;
        staircase->isUp = true;
    }
    else if(fromFloor > toFloor) {
        staircase->isDown = true;
        staircase->isUp = false;
    }
    else {
        staircase->isDown = false;
        staircase->isUp = false;
    }

    return staircase;
}

struct Staircase* staircase_terminate(struct Staircase* staircase) {
    free(staircase);

    return NULL;
}