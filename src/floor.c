#include "floor.h"

struct Floor* floor_initialize() {
    struct Floor* floor = malloc(sizeof(struct Floor));
    floor->width = FLOOR_WIDTH;
    floor->height = FLOOR_HEIGHT;
    floor->roomCount = randomNumberBetween(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX);

    int index;
    for(index = 0; index < FLOOR_ROOMS_MAX; index++) {
        if(index < floor->roomCount) {
            floor->rooms[index] = room_initialize(0, 0);
        }
        else {
            floor->rooms[index] = NULL;
        }
    }

    floor->staircase_down = staircase_initialize(0,0, 0, 1);
    floor->staircase_up = staircase_initialize(0,0, 0,1);

    return floor;
}

struct Floor* floor_terminate(struct Floor* floor) {
    int index;
    for(index = 0; index < floor->roomCount; index++) {
        floor->rooms[index] = room_terminate(floor->rooms[index]);
    }

    floor->staircase_down = staircase_terminate(floor->staircase_down);
    floor->staircase_up = staircase_terminate(floor->staircase_up);

    free(floor);
    return NULL;
}