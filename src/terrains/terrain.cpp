#include "terrain.h"

Terrain::Terrain(Floor* floor, u_short id, u_char x, u_char y) {
    this->floor = floor;
    this->id = id;
    this->x = x;
    this->y = y;

    this->character = UNKNOWN_CHARACTER;
    this->hardness = 0;

    this->immutable = false;
    this->rock = false;
    this->walkable = false;
}

Terrain::~Terrain() = default;

/** GETTERS **/
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

bool Terrain::isWalkable() {
    return this->walkable;
}

bool Terrain::isImmutable() {
    return this->immutable;
}

bool Terrain::isRock() {
    return this->rock;
}
/** GETTERS **/

/** SETTERS **/
Terrain* Terrain::setFloor(Floor* floor) {
    this->floor = floor;

    return this;
}

Terrain* Terrain::setId(u_short id) {
    this->id = id;

    return this;
}
Terrain* Terrain::setX(u_char x) {
    this->x = x;

    return this;
}

Terrain* Terrain::setY(u_char y) {
    this->y = y;

    return this;
}

Terrain* Terrain::setCharacter(u_char character) {
    this->character = character;

    return this;
}

Terrain* Terrain::setHardness(u_char hardness) {
    this->hardness = hardness;

    return this;
}

Terrain* Terrain::setWalkable(bool walkable) {
    this->walkable = walkable;

    return this;
}

Terrain* Terrain::setImmutable(bool immutable) {
    this->immutable = immutable;

    return this;
}

Terrain* Terrain::setRock(bool rock) {
    this->rock = rock;

    return this;
}
/** SETTERS **/