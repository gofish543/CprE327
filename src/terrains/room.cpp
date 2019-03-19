#include "room.h"

Room::Room(Floor* floor, u_short id, u_char x, u_char y, u_char startingX, u_char startingY, u_char width, u_char height)
        : Terrain(floor, id, x, y) {
    this->width = width;
    this->height = height;
    this->startingX = startingX;
    this->startingY = startingY;

    this->walkable = true;
    this->rock = false;
    this->immutable = false;

    this->hardness = ROOM_HARDNESS;
    this->character = ROOM_CHARACTER;
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
Room* Room::setWidth(u_char width) {
    this->width = width;

    return this;
}

Room* Room::setHeight(u_char height) {
    this->height = height;

    return this;
}

Room* Room::setStartingX(u_char startingX) {
    this->startingX = startingX;

    return this;
}

Room* Room::setStartingY(u_char startingY) {
    this->startingY = startingY;

    return this;
}
/** SETTERS */