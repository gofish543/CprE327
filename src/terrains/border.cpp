#include "border.h"

Border::Border(Floor* floor, u_short id, u_char x, u_char y, u_char character) : Terrain(floor, id, x, y) {
    this->isImmutable = true;
    this->isWalkable = false;
    this->isRock = false;

    this->character = character;
    this->hardness = BORDER_HARDNESS;
}