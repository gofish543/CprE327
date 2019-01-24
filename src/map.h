#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resource.h"
#include "floor.h"

#define MAP_FLOORS_MIN 3
#define MAP_FLOORS_MAX 6

struct Map {
    unsigned short floorCount;
    unsigned short currentFloor;

    struct Floor* floors[MAP_FLOORS_MAX];
};

struct Map* map_initialize();
struct Map* map_terminate(struct Map* map);

#endif