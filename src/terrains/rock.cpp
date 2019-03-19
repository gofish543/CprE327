#include "rock.h"

Rock::Rock(Floor* floor, u_short id, u_char x, u_char y, u_char hardness) : Terrain(floor, id, x, y) {
    this->isImmutable = false;
    this->isWalkable = false;
    this->isRock = true;

    this->character = ROCK_CHARACTER;
    this->hardness = hardness;
}