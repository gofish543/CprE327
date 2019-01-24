#include "map.h"

struct Map* map_initialize() {
    struct Map* map = malloc(sizeof(struct Map));

    map->floorCount = randomNumberBetween(MAP_FLOORS_MIN, MAP_FLOORS_MAX);
    map->currentFloor = 0;

    int index;
    for (index = 0; index < map->floorCount; index++) {
        map->floors[index] = floor_initialize();
    }

    return map;
}

struct Map* map_terminate(struct Map* map) {
    int index;
    for(index = 0; index< map->floorCount; index++) {
        map->floors[index] = floor_terminate(map->floors[index]);
    }

    free(map);
    return NULL;
}