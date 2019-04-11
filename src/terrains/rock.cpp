#include "rock.h"

Rock::Rock(Floor* floor, u_char x, u_char y, u_char hardness) : Terrain(floor, x, y) {
    this->character = ROCK_CHARACTER;
    this->hardness = hardness;
    this->type = TERRAIN_ROCK;
}

Rock::~Rock() = default;

/** GETTERS **/
/** GETTERS **/

/** SETTERS **/
/** SETTERS **/