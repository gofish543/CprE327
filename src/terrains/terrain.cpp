#include "terrain.h"

Terrain::Terrain(Floor* floor, u_short id, u_char x, u_char y) {
    this->floor = floor;
    this->id = id;
    this->x = x;
    this->y = y;

    this->character = UNKNOWN_CHARACTER;
    this->hardness = 0;

    this->isImmutable = false;
    this->isRock = false;
    this->isWalkable = false;
}

Terrain* Terrain::setCharacter(u_char character) {
    this->character = character;

    return this;
}

Terrain* Terrain::setHardness(u_char hardness) {
    this->hardness = hardness;

    return this;
}

Floor* Terrain::getFloor() {
    return this->floor;
}

u_short Terrain::getId() {
    return this->id;
}

u_char Terrain::getX() {
    return this->x;
}

u_char Terrain::getY() {
    return this->y;
}

u_char Terrain::getCharacter() {
    return this->character;
}

u_char Terrain::getHardness() {
    return this->hardness;
}