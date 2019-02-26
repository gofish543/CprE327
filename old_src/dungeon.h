#ifndef DUNGEON_H
#define DUNGEON_H

struct Dungeon;
typedef struct Dungeon Dungeon;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resource.h"
#include "save_load.h"
#include "floor.h"
#include "settings.h"
#include "events.h"
#include "output.h"
#include "characters/player.h"

#define DUNGEON_FLOORS_MIN 1
#define DUNGEON_FLOORS_MAX 1

struct Dungeon {
    EventManager* eventManager;

    u_char floorCount;
    Floor* floor;

    Player* player;
    Floor** floors;
    Settings* settings;

    u_char* textLine1;
    u_char* textLine2;
    u_char* textLine3;
};

Dungeon* dungeon_initialize(Settings* settings);
Dungeon* dungeon_terminate(Dungeon* dungeon);

int dungeon_prepend_message(Dungeon* dungeon, const char* message);
int dungeon_append_message(Dungeon* dungeon, const char* message);

#endif
