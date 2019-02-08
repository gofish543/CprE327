#ifndef MONSTER_H
#define MONSTER_H

struct Monster;
typedef struct Monster Monster;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character_listings.h"
#include "floor.h"
#include "dungeon.h"
#include "resource.h"

struct Monster {
    u_char floor;
    FloorDot* dot;
    FloorDot* standingOn;

    bool canTunnel;

    Dungeon* dungeon;
};

Monster* monster_initialize(Dungeon* dungeon, u_char x, u_char y, u_char floor, bool canTunnel);
Monster* monster_terminate(Monster* monster);

void monster_move_to(Monster* monster, u_char toX, u_char toY);
void monsters_move(Floor* floor);

#endif
