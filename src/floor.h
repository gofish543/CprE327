#ifndef FLOOR_H
#define FLOOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resource.h"
#include "room.h"
#include "staircase.h"
#include "character_listings.h"

#define FLOOR_WIDTH 80
#define FLOOR_HEIGHT 21

#define FLOOR_ROOMS_MIN 6
#define FLOOR_ROOMS_MAX 10

struct Floor {
    unsigned short width;
    unsigned short height;
    unsigned short roomCount;

    struct Staircase* staircase_down;
    struct Staircase* staircase_up;
    struct Room* rooms[FLOOR_ROOMS_MAX];
};

struct Floor* floor_initialize();

struct Floor* floor_terminate(struct Floor* floor);

#endif
