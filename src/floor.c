#include "floor.h"

Floor* floor_initialize() {
    Floor* floor = malloc(sizeof(Floor));

    floor->width = FLOOR_WIDTH;
    floor->height = FLOOR_HEIGHT;
    floor->roomCount = randomNumberBetween(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX);

    int index;
    for (index = 0; index < floor->roomCount; index++) {
//        floor->rooms[index] = room_initialize(0, 0);
    }

//    floor->staircase_down = staircase_initialize(0, 0, 0, 0);
//    floor->staircase_up = staircase_initialize(0, 0, 0, 0);

    floor_generate_empty_dots(floor);
    floor_generate_borders(floor);

    return floor;
}

Floor* floor_terminate(Floor* floor) {
    int width;
    int height;
    for (height = 0; height < floor->height; height++) {
        for (width = 0; width < floor->width; width++) {
            if (floor->floorPlan[height][width]->internalObject != NULL) {
                free(floor->floorPlan[height][width]->internalObject);
            }
            free(floor->floorPlan[height][width]);
        }
    }

    int index;
    for (index = 0; index < floor->roomCount; index++) {
//        floor->rooms[index] = room_terminate(floor->rooms[index]);
    }

//    floor->staircase_down = staircase_terminate(floor->staircase_down);
//    floor->staircase_up = staircase_terminate(floor->staircase_up);

    free(floor);
    return NULL;
}

void floor_generate_empty_dots(Floor* floor) {
    int height;
    int width;

    for (height = 0; height < floor->height; height++) {
        for (width = 0; width < floor->width; width++) {
            FloorDot* floorDot = malloc(sizeof(FloorDot));
            floorDot->type = Unknown;
            floorDot->x = width;
            floorDot->y = height;
            floorDot->hardness = 0;
            floorDot->character = UNKNOWN_CHARACTER;
            floor->floorPlan[height][width] = floorDot;
        }
    }
}

void floor_generate_borders(Floor* floor) {
    int width;
    int height;

    for (height = 0; height < floor->height; height++) {
        for (width = 0; width < floor->width; width++) {
            floor->floorPlan[height][width]->type = Border;
            floor->floorPlan[height][width]->hardness = 255;
            if ((width == 0 && height == 0) || (width == FLOOR_WIDTH - 1 && height == FLOOR_HEIGHT - 1) ||
                (width == FLOOR_WIDTH - 1 && height == 0) || (width == 0 && height == FLOOR_HEIGHT - 1)) {
                floor->floorPlan[height][width]->character = FLOOR_CORNER_WALL;
            } else if (height == 0 || height == FLOOR_HEIGHT - 1) {
                floor->floorPlan[height][width]->character = FLOOR_NORTH_SOUTH_WALL;
            } else if (width == 0 || width == FLOOR_WIDTH - 1) {
                floor->floorPlan[height][width]->character = FLOOR_EAST_WEST_WALL;
            }
        }
    }
}