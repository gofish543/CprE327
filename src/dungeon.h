#ifndef DUNGEON_H
#define DUNGEON_H

struct Dungeon;
typedef struct Dungeon Dungeon;

#define DUNGEON_FLOORS_MIN 3
#define DUNGEON_FLOORS_MAX 5

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "terrains/terrain.h"
#include "terrains/room.h"
#include "terrains/staircase.h"
#include "characters/player.h"
#include "events.h"
#include "resource.h"
#include "floor.h"
#include "settings.h"
#include "output.h"
#include "save_load.h"

struct Dungeon {
    u_char floorCount;
    Floor* floor;

    EventManager* eventManager;

    Player* player;
    Floor** floors;
    Settings* settings;

    char* textLine1;
    char* textLine2;
    char* textLine3;

    WINDOW* window;
};

Dungeon* dungeon_initialize(Settings* settings);
Dungeon* dungeon_terminate(Dungeon* dungeon);

int dungeon_prepend_message(Dungeon* dungeon, const char* message, ...);
int dungeon_append_message(Dungeon* dungeon, const char* message, ...);

#endif
