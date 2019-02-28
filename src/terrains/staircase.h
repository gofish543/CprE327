#ifndef STAIRCASE_H
#define STAIRCASE_H

struct Staircase;
typedef struct Staircase Staircase;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../character_listings.h"
#include "../dungeon.h"
#include "../events.h"

struct Staircase {
    u_char x;
    u_char y;

    u_char fromFloor;
    u_char toFloor;

    bool isUp;
    bool isDown;
};

Staircase* staircase_initialize(u_char x, u_char y, u_char fromFloor, u_char toFloor);
Staircase* staircase_terminate(Staircase* staircase);

Dungeon* staircase_take(Dungoen* dungeon, Staircase* staircase);

#endif
