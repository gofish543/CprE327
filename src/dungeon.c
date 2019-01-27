#include "dungeon.h"

Dungeon* dungeon_initialize() {
    Dungeon* dungeon = malloc(sizeof(Dungeon));

    // If failed loaded from file load up a new map
    if (!dungeon_load_from_file(dungeon)) {
        dungeon->currentFloor = 0;
        dungeon->floorCount = randomNumberBetween(DUNGEON_FLOORS_MIN, DUNGEON_FLOORS_MAX);
        // This creates a dynamically sized array. Going to need to do that because of load and save features later
        dungeon->floors = malloc(dungeon->floorCount  * sizeof(Floor*));

        u_char stairsPerFloor = randomNumberBetween(FLOOR_STAIRS_MIN, FLOOR_STAIRS_MAX);
        u_char index;
        for (index = 0; index < dungeon->floorCount; index++) {
            dungeon->floors[index] = floor_initialize(index, dungeon->floorCount, stairsPerFloor);
        }
    }

    return dungeon;
}

Dungeon* dungeon_terminate(Dungeon* dungeon) {
    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon->floors[index] = floor_terminate(dungeon->floors[index]);
    }

    free(dungeon->floors);
    free(dungeon);

    return NULL;
}

bool dungeon_load_from_file(Dungeon* dungeon) {
    return false;
}

void print_current_floor(Dungeon* dungeon) {
    u_char width;
    u_char height;

    for (height = 0; height < dungeon->floors[dungeon->currentFloor]->height; height++) {
        for (width = 0; width < dungeon->floors[dungeon->currentFloor]->width; width++) {
            printf("%c", dungeon->floors[dungeon->currentFloor]->floorPlan[height][width]->character);
        }
        printf("\n");
    }
}