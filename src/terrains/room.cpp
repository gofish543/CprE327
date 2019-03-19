#include "room.h"

Room::Room(Floor* floor, u_short id, u_char x, u_char y, u_char startingX, u_char startingY, u_char width, u_char height) : Terrain(floor, id, x, y) {
    this->width = width;
    this->height = height;
    this->startingX = startingX;
    this->startingY = startingY;

    this->isWalkable = true;
    this->isRock = false;
    this->isImmutable = false;

    this->hardness = ROOM_HARDNESS;
    this->character = ROOM_CHARACTER;
}

u_char Room::getWidth() {
    return this->width;
}

u_char Room::getHeight() {
    return this->height;
}

u_char Room::getStartingX(){
    return this->startingX;
}

u_char Room::getStartingY() {
    return this->startingY;
}