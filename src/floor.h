#ifndef FLOOR_H
#define FLOOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resource.h"
#include "room.h"
#include "staircase.h"
#include "character_listings.h"

#define FLOOR_WIDTH 80
#define FLOOR_HEIGHT 21

#define FLOOR_ROOMS_MIN 6
#define FLOOR_ROOMS_MAX 10

enum FloorDotType {
    type_unknown = UNKNOWN_CHARACTER,
    type_border = 'B',
    type_rock = FLOOR_ROCK,
    type_corridor = FLOOR_CORRIDOR,
    type_room = ROOM_CHARACTER,
    type_staircaseUp = STAIRCASE_UP,
    type_staircaseDown = STAIRCASE_DOWN
};

typedef struct {
    u_char x;
    u_char y;

    char character;
    u_char hardness;

    enum FloorDotType type;
    void* internalObject;
} FloorDot;

typedef struct {
    u_char width;
    u_char height;
    u_char roomCount;

    u_char floorNumber;
    u_char maxFloors;

    FloorDot* floorPlan[FLOOR_HEIGHT][FLOOR_WIDTH];
    Staircase* stairUp;
    Staircase* stairDown;
    Room* rooms[FLOOR_ROOMS_MAX];
} Floor;

Floor* floor_initialize(u_char floorNumber, u_char maxFloors);
Floor* floor_terminate(Floor* floor);

void floor_generate_empty_dots(Floor* floor);
void floor_generate_borders(Floor* floor);
void floor_generate_rooms(Floor* floor);
void floor_generate_staircases(Floor* floor);
void floor_generate_corridors(Floor* floor);

#endif
