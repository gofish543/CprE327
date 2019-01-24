#ifndef FLOOR_H
#define FLOOR_H

#include "resource.h"
#include "room.h"
#include "staircase.h"

#define FLOOR_NORTH_SOUTH_WALL '-'
#define FLOOR_EAST_WEST_WALL '|'
#define FLOOR_CORNER_WALL '+'
#define FLOOR_ROCK ' '
#define FLOOR_CORRIDOR '#'

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
