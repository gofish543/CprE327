#include "terrain.h"

Terrain::Terrain(Floor* floor, u_short id, u_char x, u_char y) {
    this->floor = floor;
    this->id = id;
    this->x = x;
    this->y = y;

    this->character = UNKNOWN_CHARACTER;
    this->hardness = 0;

    this->type = 0;
}

Terrain::~Terrain() = default;

bool Terrain::isBorder() {
    return this->type & TERRAIN_BORDER;
}

bool Terrain::isCorridor() {
    return this->type & TERRAIN_CORRIDOR;
}

bool Terrain::isRock() {
    return this->type & TERRAIN_ROCK;

}

bool Terrain::isRoom() {
    return this->type & TERRAIN_ROOM;
}

bool Terrain::isStaircase() {
    return this->type & TERRAIN_STAIRCASE;
}

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

u_char Terrain::getType() {
    return this->type;
}
/** GETTERS **/

/** SETTERS **/
Terrain* Terrain::setCharacter(u_char character) {
    this->character = character;

    return this;
}

Terrain* Terrain::setHardness(u_char hardness) {
    this->hardness = hardness;

    return this;
}
/** SETTERS **/