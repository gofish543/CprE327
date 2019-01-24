#include "escape_from_darkness.h"

int main(int argc, char *argv[]) {
    struct Map* map = NULL;

    initialize(&map);

    int index;
    printf("Floors Selected: %d\n", map->floorCount);
    for(index = 0; index < map->floorCount; index++) {
        printf("Rooms on floor %d: %d\n", index, map->floors[index]->roomCount);
    }

    terminate(&map);
    return 0;
}

void initialize(struct Map** map) {
   *map = map_initialize();
}

void terminate(struct Map** map) {
    *map = map_terminate(*map);
}