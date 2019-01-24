#include "escape_from_darkness.h"

int main(int argc, char *argv[]) {
    struct Map* map = NULL;

    initialize(&map);

    printf("Floors Selected: %d\n", map->floorCount);

    terminate(&map);
    return 0;
}

void initialize(struct Map** map) {
   *map = map_initialize();
}

void terminate(struct Map** map) {
    map_terminate(*map);
    *map = NULL;
}