#ifndef FLOOR_H
#define FLOOR_H

struct Floor;
typedef struct Floor Floor;

#define FLOOR_WIDTH 80
#define FLOOR_HEIGHT 21

#define FLOOR_ROOMS_MIN 8
#define FLOOR_ROOMS_MAX 8

#define FLOOR_STAIRS_MIN 2
#define FLOOR_STAIRS_MAX 4

#define FLOOR_MONSTERS_MIN 2
#define FLOOR_MONSTERS_MAX 4

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dungeon.h"
#include "resource.h"
#include "character_listings.h"
#include "./terrains/terrain.h"
#include "./terrains/room.h"
#include "./terrains/staircase.h"
#include "./characters/character.h"
#include "./characters/player.h"
#include "./characters/monster.h"

struct Floor {
    Dungeon* dungeon;
    u_char floorNumber;

    u_short roomCount;
    u_short stairUpCount;
    u_short stairDownCount;
    u_short monsterCount;

    Terrain* terrains[FLOOR_HEIGHT][FLOOR_WIDTH];
    Character* characters[FLOOR_HEIGHT][FLOOR_WIDTH];

    u_char tunnelerView[FLOOR_HEIGHT][FLOOR_WIDTH];
    u_char nonTunnelerView[FLOOR_HEIGHT][FLOOR_WIDTH];
    u_char cheapestPathToPlayer[FLOOR_HEIGHT][FLOOR_WIDTH];

    Monster** monsters;
    Staircase** upStairs;
    Staircase** downStairs;
    Room** rooms;
};

Floor* floor_initialize(Dungeon* dungeon, u_char floorNumber, u_short roomCount, u_short stairUpCount, u_short stairDownCount, u_char numberOfMonsters);
Floor* floor_terminate(Floor* floor);

u_char floor_character_at(Floor* floor, u_char x, u_char y);

int floor_generate_empty_characters(Floor* floor);
int floor_generate_empty_terrains(Floor* floor);
int floor_generate_borders(Floor* floor);
int floor_generate_rooms(Floor* floor);
int floor_generate_staircases(Floor* floor);
int floor_generate_corridors(Floor* floor);
int floor_generate_monsters(Floor* floor);

#endif
