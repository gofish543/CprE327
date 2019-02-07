#ifndef DUNGEON_H
#define DUNGEON_H

struct Dungeon;
typedef struct Dungeon Dungeon;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resource.h"
#include "player.h"
#include "save_load.h"
#include "floor.h"
#include "settings.h"

#define DUNGEON_FLOORS_MIN 3
#define DUNGEON_FLOORS_MAX 6

struct Dungeon {
    u_char floorCount;
    u_char currentFloor;

    Player* player;
    Floor** floors;
    Settings* settings;
};

Dungeon* dungeon_initialize(Settings* settings);
Dungeon* dungeon_terminate(Dungeon* dungeon);
int dungeon_load_from_program(Dungeon* dungeon);
void dungeon_place_character(Dungeon* dungeon);

void dungeon_print_current_floor(Dungeon* dungeon);
void dungeon_print_current_floor_hardness(Dungeon* dungeon);
void dungeon_print_floor(Dungeon* dungeon, u_char floor);
void dungeon_print_floor_hardness(Dungeon* dungeon, u_char floor);

#endif
