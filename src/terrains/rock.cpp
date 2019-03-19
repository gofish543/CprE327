#include "rock.h"

Rock::Rock(Floor* floor, u_short id, u_char x, u_char y, u_char hardness) : Terrain(floor, id, x, y) {
    this->immutable = false;
    this->walkable = false;
    this->rock = true;

    this->character = ROCK_CHARACTER;
    this->hardness = hardness;
}

Rock::~Rock() = default;

/** GETTERS **/
/** GETTERS **/

/** SETTERS **/
/** SETTERS **/