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
#include "events.h"

#define DUNGEON_FLOORS_MIN 1
#define DUNGEON_FLOORS_MAX 1

struct Dungeon {
    u_char floorCount;
    u_char currentFloor;

    Player* player;
    Floor** floors;
    Settings* settings;
    EventManager* eventManager;

    char* textLine1;
    char* textLine2;
    char* textLine3;
};

Dungeon* dungeon_initialize(Settings* settings);
Dungeon* dungeon_terminate(Dungeon* dungeon);

int dungeon_prepend_message(Dungeon* dungeon, const char* message);
int dungeon_append_message(Dungeon* dungeon, const char* message);

#endif
