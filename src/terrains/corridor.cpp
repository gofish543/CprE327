#include "corridor.h"

Corridor::Corridor(Floor* floor, u_char x, u_char y) : Terrain(floor, x, y) {
    this->character = CORRIDOR_CHARACTER;
    this->hardness = CORRIDOR_HARDNESS;
    this->type = TERRAIN_CORRIDOR;
}

Corridor::~Corridor() = default;

/** GETTERS **/
/** GETTERS **/

/** SETTERS **/
/** SETTERS **/