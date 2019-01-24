#include "room.h"

struct Room* room_initialize(unsigned short startX, unsigned short startY) {
    struct Room* room = malloc(sizeof(struct Room));

    room->width = randomNumberBetween(ROOM_MIN_WIDTH, ROOM_MAX_WIDTH);
    room->height = randomNumberBetween(ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT);

    room->startX = startX;
    room->startY = startY;

    room->character = ROOM_CHARACTER;

    return room;
}

struct Room* room_terminate(struct Room* room) {
    free(room);

    return NULL;
}