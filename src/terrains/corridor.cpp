#include "corridor.h"

Corridor::Corridor(Floor* floor, u_short id, u_char x, u_char y) : Terrain(floor,id, x, y) {
    this->isImmutable = false;
    this->isWalkable = true;
    this->isRock = false;

    this->character = CORRIDOR_CHARACTER;
    this->hardness = CORRIDOR_HARDNESS;
}