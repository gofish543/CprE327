#include "room.h"

Room::Room(Floor* floor, u_short id, u_char x, u_char y, u_char startingX, u_char startingY, u_char width, u_char height)
        : Terrain(floor, id, x, y) {
    this->width = width;
    this->height = height;
    this->startingX = startingX;
    this->startingY = startingY;

    this->hardness = ROOM_HARDNESS;
    this->character = ROOM_CHARACTER;
    this->type = TERRAIN_ROOM;
}

u_char Room::randomXInside() {
    return u_char(Dice::RandomNumberBetween(this->getStartX(), this->getEndX()));
}

u_char Room::randomYInside() {
    return u_char(Dice::RandomNumberBetween(this->getStartY(), this->getEndY()));
}

u_char Room::getStartX() {
    return this->startingX;
}

u_char Room::getStartY() {
    return this->startingY;
}

u_char Room::getEndX() {
    return this->startingX + this->width - 1;
}

u_char Room::getEndY() {
    return this->startingY + this->height - 1;
}

u_char Room::getArea() {
    printf("%d\n", this->width * this->height);
    return u_char(this->width * this->height);
}

/** GETTERS **/
u_char Room::getWidth() {
    return this->width;
}

u_char Room::getHeight() {
    return this->height;
}

u_char Room::getStartingX() {
    return this->startingX;
}

u_char Room::getStartingY() {
    return this->startingY;
}
/** GETTERS **/

/** SETTERS **/
/** SETTERS */