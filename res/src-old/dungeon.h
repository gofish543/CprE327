#ifndef DUNGEON_H
#define DUNGEON_H

using namespace app;

class Dungeon;

#define DUNGEON_TEXT_LINES 3

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

class Dungeon {
public:
    Dungeon(Settings* incomingSettings);
    ~Dungeon();

    Floor* getFloor();
    EventManager* getEventManager();
    Player* getPlayer();
    Settings* getSettings();
    WINDOW* getWindow();

    Floor** getFloors();

    u_char getFloorCount();
    char* getTextLine(u_char index);

protected:

private:
    Floor** floors;
    Floor* floor;
    EventManager* eventManager;
    Player* player;
    Settings* settings;

    WINDOW* window;

    u_char floorCount;
    char* textLines[DUNGEON_TEXT_LINES];
};

//Dungeon* dungeon_initialize(Settings* settings);
//Dungeon* dungeon_terminate(Dungeon* dungeon);
//
//int dungeon_prepend_message(Dungeon* dungeon, const char* message, ...);
//int dungeon_append_message(Dungeon* dungeon, const char* message, ...);

#endif
