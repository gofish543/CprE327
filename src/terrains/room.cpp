#include "room.h"

Room* room_initialize(u_char startX, u_char startY, u_char width, u_char height) {
    Room* room = (Room*) malloc(sizeof(Room));

    room->width = width;
    room->height = height;

    room->startX = startX;
    room->startY = startY;

    return room;
}

Room* room_terminate(Room* room) {
    free(room);

    return NULL;
}