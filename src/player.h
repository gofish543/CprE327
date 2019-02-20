#ifndef PLAYER_H
#define PLAYER_H

struct Player;
typedef struct Player Player;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character_listings.h"
#include "floor.h"
#include "dungeon.h"
#include "events.h"
#include "actions.h"

#define PLAYER_SPEED 10

struct Player {
    u_char floor;
    FloorDot* dot;
    FloorDot* standingOn;

    u_char speed;

    bool isAlive;

    Dungeon* dungeon;

    // Statistical data
    u_int level;
    u_int monstersSlain;
    u_int daysSurvived;
    u_int tilesExplored;
};

Player* player_initialize(Dungeon* dungeon, u_char x, u_char y, u_char floor);
Player* player_terminate(Player* player);

int player_free(Player* player);
int player_move_to(Player* player, u_char toFloor, u_char toX, u_char toY);

int player_event(Event* event);

#endif
