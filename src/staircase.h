#ifndef STAIRCASE_H
#define STAIRCASE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character_listings.h"

typedef struct {
    u_char x;
    u_char y;

    u_char fromFloor;
    u_char toFloor;

    bool isUp;
    bool isDown;
} Staircase;

Staircase* staircase_initialize(u_char x, u_char y, u_char fromFloor, u_char toFloor);
Staircase* staircase_terminate(Staircase* staircase);

#endif
