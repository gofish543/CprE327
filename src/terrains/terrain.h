#ifndef TERRAIN_H
#define TERRAIN_H

struct Terrain;
typedef struct Terrain Terrain;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../floor.h"
#include "room.h"
#include "staircase.h"

struct Terrain {
    Floor* floor;
    Room* room;
    Staircase* staircase;

    u_char character;
    u_char hardness;

    u_char x;
    u_char y;

    bool isWalkable;
    bool isImmutable;
    bool isRock;
};

Terrain* terrain_initialize(Floor* floor, u_char x, u_char y, u_char character, u_char hardness);
Terrain* terrain_terminate(Terrain* terrain);

bool terrain_is_empty(Terrain* terrain);

#endif
