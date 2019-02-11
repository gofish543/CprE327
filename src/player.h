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

struct Player {
    u_char floor;
    FloorDot* dot;
    FloorDot* standingOn;

    Dungeon* dungeon;
};

Player* player_initialize(Dungeon* dungeon, u_char x, u_char y, u_char floor);
Player* player_terminate(Player* player);

void player_free_standing_on(Player* player);
void player_move_to(Player* player, u_char toFloor, u_char toX, u_char toY);

#endif
