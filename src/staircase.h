#ifndef STAIRCASE_H
#define STAIRCASE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character_listings.h"

struct Staircase {
    unsigned short x;
    unsigned short y;

    unsigned short fromFloor;
    unsigned short toFloor;

    bool isUp;
    bool isDown;

    char character;
};

struct Staircase* staircase_initialize(unsigned short x, unsigned short y, unsigned short fromFloor, unsigned short toFloor);
struct Staircase* staircase_terminate(struct Staircase* staircase);

#endif
