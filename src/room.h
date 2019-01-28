#ifndef ROOM_H
#define ROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "resource.h"
#include "character_listings.h"

#define ROOM_MIN_WIDTH 4
#define ROOM_MAX_WIDTH 8
#define ROOM_MIN_HEIGHT 4
#define ROOM_MAX_HEIGHT 8

typedef struct {
    u_char width;
    u_char height;

    u_char startX;
    u_char startY;
} Room;

Room* room_initialize(u_char startX, u_char startY, u_char width, u_char height);
Room* room_terminate(Room* room);

#endif
