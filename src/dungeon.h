#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resource.h"
#include "floor.h"

#define DUNGEON_FLOORS_MIN 3
#define DUNGEON_FLOORS_MAX 6

typedef struct {
    u_char floorCount;
    u_char currentFloor;

    Floor** floors;
} Dungeon;

Dungeon* dungeon_initialize();
Dungeon* dungeon_terminate(Dungeon* dungeon);

bool dungeon_load_from_file(Dungeon* dungeon);
void print_current_floor(Dungeon* dungeon);

#endif
