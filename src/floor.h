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

enum FloorDotType{Unknown, Border , Rock, Corridor, Room, StaircaseUp, StaircaseDown};

typedef struct {
    unsigned short x;
    unsigned short y;

    char character;
    unsigned char hardness;

    enum FloorDotType type;
    void* internalObject;
} FloorDot;

typedef struct {
    unsigned short width;
    unsigned short height;
    unsigned short roomCount;

    FloorDot* floorPlan[FLOOR_HEIGHT][FLOOR_WIDTH];
//    struct Staircase* staircase_down;
//    struct Staircase* staircase_up;
//    struct Room* rooms[FLOOR_ROOMS_MAX];
} Floor;

Floor* floor_initialize();
Floor* floor_terminate(Floor* floor);

void floor_generate_empty_dots(Floor* floor);
void floor_generate_borders(Floor* floor);

#endif
