#ifndef ROOM_H
#define ROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resource.h"
#include "character_listings.h"

#define ROOM_MIN_WIDTH 4
#define ROOM_MIN_HEIGHT 3
#define ROOM_MAX_WIDTH 7
#define ROOM_MAX_HEIGHT 6

struct Room {
    unsigned short width;
    unsigned short height;

    unsigned short startX;
    unsigned short startY;

    char character;
};

struct Room* room_initialize(unsigned short startX, unsigned short startY);

struct Room* room_terminate(struct Room* room);

#endif
