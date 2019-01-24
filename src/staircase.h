#ifndef STAIRCASE_H
#define STAIRCASE_H

#include <stdbool.h>

#define STAIRCASE_UP '<'
#define STAIRCASE_DOWN '>'

typedef struct {
    unsigned short startX;
    unsigned short startY;

    unsigned short fromFloor;
    unsigned short toFloor;

    bool isUp;
    bool isDown;
} STAIRCASE;

#endif
