#ifndef FLOOR_H
#define FLOOR_H

struct Floor;
typedef struct Floor Floor;

struct FloorDot;
typedef struct FloorDot FloorDot;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resource.h"
#include "terrains/room.h"
#include "terrains/staircase.h"
#include "character_listings.h"
#include "dungeon.h"
#include "characters/monster.h"
#include "terrains/terrain.h"

#define FLOOR_WIDTH 80
#define FLOOR_HEIGHT 21

#define FLOOR_ROOMS_MIN 6
#define FLOOR_ROOMS_MAX 10

#define FLOOR_STAIRS_MIN 3
#define FLOOR_STAIRS_MAX 6

#define FLOOR_MONSTERS_MIN 3
#define FLOOR_MONSTERS_MAX 6

enum FloorDotType {
    type_unknown = UNKNOWN_CHARACTER,
    type_border = GENERAL_WALL_CHARACTER,
    type_rock = ROCK_CHARACTER,
    type_corridor = CORRIDOR_CHARACTER,
    type_room = ROOM_CHARACTER,
    type_staircase_up = STAIRCASE_UP_CHARACTER,
    type_staircase_down = STAIRCASE_DOWN_CHARACTER,
    type_player = PLAYER_CHARACTER,
    type_monster = MONSTER_CHARACTER
};

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

    Staircase** upStairs;
    Staircase** downStairs;
    Room** rooms;
    Monster** monsters;
};

typedef struct {
    Dungeon* dungeon;

    u_char width;
    u_char height;
    u_short roomCount;
    u_short stairUpCount;
    u_short stairDownCount;
    u_short monsterCount;

    u_char floorNumber;

    u_char floorPlanCharacters[FLOOR_HEIGHT][FLOOR_WIDTH];
    u_char floorPlanHardness[FLOOR_HEIGHT][FLOOR_WIDTH];

    u_char* stairUpX;
    u_char* stairUpY;

    u_char* stairDownX;
    u_char* stairDownY;

    u_char* roomsX;
    u_char* roomsY;
    u_char* roomsWidth;
    u_char* roomsHeight;

    u_char* monsterX;
    u_char* monsterY;
    u_char* monsterClassification;
    u_char* monsterSpeed;
} FloorLoadStructure;

Floor* floor_load_initialize(FloorLoadStructure* floorLoadStructure);
Floor* floor_initialize(Dungeon* dungeon, u_char floorNumber, u_short stairUpCount, u_short stairDownCount);
Floor* floor_terminate(Floor* floor);

FloorDot* floor_dot_initialize(u_char x, u_char y, enum FloorDotType floorDotType, u_char hardness, u_char character);
FloorDot* floor_dot_terminate(FloorDot* floorDot);

int floor_generate_empty_dots(Floor* floor);
int floor_generate_borders(Floor* floor);
int floor_generate_rooms(Floor* floor);
int floor_generate_staircases(Floor* floor);
int floor_generate_corridors(Floor* floor);
int floor_generate_monsters(Floor* floor);

#endif
