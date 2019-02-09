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
#include "room.h"
#include "staircase.h"
#include "character_listings.h"
#include "dungeon.h"
#include "monster.h"

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
    type_border = 'B',
    type_rock = ROCK_CHARACTER,
    type_corridor = CORRIDOR_CHARACTER,
    type_room = ROOM_CHARACTER,
    type_staircase_up = STAIRCASE_UP_CHARACTER,
    type_staircase_down = STAIRCASE_DOWN_CHARACTER,
    type_player = PLAYER_CHARACTER,
    type_monster = MONSTER_CHARACTER
};

struct FloorDot {
    u_char x;
    u_char y;

    u_char character;
    u_char hardness;

    enum FloorDotType type;
};

struct Floor {
    Dungeon* dungeon;
    u_char width;
    u_char height;
    u_short roomCount;
    u_short stairUpCount;
    u_short stairDownCount;
    u_short monsterCount;

    u_char floorNumber;

    FloorDot* floorPlan[FLOOR_HEIGHT][FLOOR_WIDTH];
    u_char tunnelerCost[FLOOR_HEIGHT][FLOOR_WIDTH];
    u_char nonTunnelerCost[FLOOR_HEIGHT][FLOOR_WIDTH];
    Staircase** stairUp;
    Staircase** stairDown;
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
} FloorLoadStructure;

Floor* floor_load_initialize(FloorLoadStructure* floorLoadStructure);
Floor* floor_initialize(Dungeon* dungeon, u_char floorNumber, u_short stairUpCount, u_short stairDownCount);
Floor* floor_terminate(Floor* floor);

FloorDot* floor_dot_initialize(u_char x, u_char y, enum FloorDotType floorDotType, u_char hardness, u_char character);
FloorDot* floor_dot_terminate(FloorDot* floorDot, bool force);

int floor_generate_empty_dots(Floor* floor);
int floor_generate_borders(Floor* floor);
int floor_generate_rooms(Floor* floor);
int floor_generate_staircases(Floor* floor);
int floor_generate_corridors(Floor* floor);
int floor_generate_monsters(Floor* floor);

#endif
