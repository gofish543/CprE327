#include "terrain.h"

Terrain* terrain_initialize(Floor* floor, u_char x, u_char y, u_char character, u_char hardness) {
    Terrain* terrain = malloc(sizeof(Terrain));

    terrain->floor = floor;
    terrain->x = x;
    terrain->y = y;
    terrain->character = character;
    terrain->hardness = hardness;

    terrain->staircase = null;
    terrain->room = null;

    terrain->isWalkable = false;
    terrain->isImmutable = false;
    terrain->isRock = false;

    return terrain;
}

Terrain* terrain_terminate(Terrain* terrain) {
    free(terrain);

    return null;
}

bool terrain_is_empty(Terrain* terrain) {
    return terrain->staircase == null && terrain->room == null && !terrain->isImmutable;
}