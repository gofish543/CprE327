#include "floor.h"

struct Floor* floor_initialize() {
    struct Floor* floor = malloc(sizeof(struct Floor*));
    floor->width = FLOOR_WIDTH;
    floor->height = FLOOR_HEIGHT;
    floor->roomCount = randomNumberBetween(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX);

    int index;
    for(index = 0; index < floor->roomCount; index++) {
//        floor.rooms[index] = room_initialize();
    }
//    floor.staircase_down = staircase_initialize(-1);
//    floor.staircase_up = staircase_initialize(1);

    printf("%d\n", floor->roomCount);
    return floor;
}

void floor_terminate(struct Floor* floor) {
    printf("%d\n", floor->roomCount);
    int index;
    for(index = 0; index < floor->roomCount; index++) {
//        room_terminate(floor.rooms[index]);
    }

//    staircase_terminate(floor.staircase_down);
//    staircase_terminate(floor.staircase_up);
    free(floor);
}