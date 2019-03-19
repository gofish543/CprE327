#include "corridor.h"

Corridor::Corridor(Floor* floor, u_short id, u_char x, u_char y) : Terrain(floor, id, x, y) {
    this->immutable = false;
    this->walkable = true;
    this->rock = false;

    this->character = CORRIDOR_CHARACTER;
    this->hardness = CORRIDOR_HARDNESS;
}

Corridor::~Corridor() = default;

/** GETTERS **/
/** GETTERS **/

/** SETTERS **/
/** SETTERS **/