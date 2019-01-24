#include "floor.h"

Floor* floor_initialize(u_char floorNumber, u_char maxFloors) {
    Floor* floor = malloc(sizeof(Floor));

    floor->width = FLOOR_WIDTH;
    floor->height = FLOOR_HEIGHT;
    floor->roomCount = randomNumberBetween(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX);

    floor->floorNumber = floorNumber;
    floor->maxFloors = maxFloors;

    floor_generate_empty_dots(floor);
    floor_generate_borders(floor);
    floor_generate_rooms(floor);
    floor_generate_staircases(floor);
    floor_generate_corridors(floor);

    return floor;
}

Floor* floor_terminate(Floor* floor) {
    u_char height;
    u_char width;

    for (height = 0; height < floor->height; height++) {
        for (width = 0; width < floor->width; width++) {
            free(floor->floorPlan[height][width]);
        }
    }

    u_char index;
    for (index = 0; index < floor->roomCount; index++) {
        if (floor->rooms[index] != NULL) {
            floor->rooms[index] = room_terminate(floor->rooms[index]);
        }
    }

    if (floor->stairDown != NULL) {
        floor->stairDown = staircase_terminate(floor->stairDown);
    }
    if (floor->stairUp != NULL) {
        floor->stairUp = staircase_terminate(floor->stairUp);
    }

    free(floor);
    return NULL;
}

void floor_generate_empty_dots(Floor* floor) {
    u_char height;
    u_char width;

    for (height = 0; height < floor->height; height++) {
        for (width = 0; width < floor->width; width++) {
            FloorDot* floorDot = malloc(sizeof(FloorDot));
            floorDot->type = type_rock;
            floorDot->x = width;
            floorDot->y = height;
            floorDot->hardness = 100;
            floorDot->character = FLOOR_ROCK;
            floorDot->internalObject = NULL;
            floor->floorPlan[height][width] = floorDot;
        }
    }
}

void floor_generate_borders(Floor* floor) {
    u_char height;
    u_char width;

    for (height = 0; height < floor->height; height++) {
        for (width = 0; width < floor->width; width++) {
            if ((width == 0 && height == 0) || (width == FLOOR_WIDTH - 1 && height == FLOOR_HEIGHT - 1) ||
                (width == FLOOR_WIDTH - 1 && height == 0) || (width == 0 && height == FLOOR_HEIGHT - 1)) {
                floor->floorPlan[height][width]->type = type_border;
                floor->floorPlan[height][width]->hardness = 255;
                floor->floorPlan[height][width]->character = FLOOR_CORNER_WALL;
            } else if (height == 0 || height == FLOOR_HEIGHT - 1) {
                floor->floorPlan[height][width]->type = type_border;
                floor->floorPlan[height][width]->hardness = 255;
                floor->floorPlan[height][width]->character = FLOOR_NORTH_SOUTH_WALL;
            } else if (width == 0 || width == FLOOR_WIDTH - 1) {
                floor->floorPlan[height][width]->type = type_border;
                floor->floorPlan[height][width]->hardness = 255;
                floor->floorPlan[height][width]->character = FLOOR_EAST_WEST_WALL;
            }
        }
    }
}

void floor_generate_rooms(Floor* floor) {
    u_char startX;
    u_char startY;

    u_char roomWidth;
    u_char roomHeight;

    u_char height;
    u_char width;

    u_char index;

    u_char maxDoWhile;
    bool valid;
    for (index = 0; index < floor->roomCount; index++) {
        maxDoWhile = 0;
        // Attempt to find a starting point randomly, do while
        do {
            valid = true;
            maxDoWhile++;

            // Find something inside the gameplay box....
            startX = randomNumberBetween(0 + 1, floor->width - 1);
            startY = randomNumberBetween(0 + 1, floor->height - 1);

            roomWidth = randomNumberBetween(ROOM_MIN_WIDTH, ROOM_MAX_WIDTH);
            roomHeight = randomNumberBetween(ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT);

            // Need to check boundaries one off to make sure they are open spaces
            for (height = startY - 1; height < (startY + roomHeight + 1); height++) {
                // As long as we are still valid, keep going
                if (valid) {
                    // Need to check boundaries one off to make sure they are open spaces
                    for (width = startX - 1; width < (startX + roomWidth + 1); width++) {
                        // Check to make sure we are still within bounds
                        if (height > floor->height - 1 || width > floor->width - 1) {
                            // This do while fails, try another round
                            valid = false;
                            break;
                        }

                        // Check if the index has a non rock
                        if (floor->floorPlan[height][width]->type != type_rock && floor->floorPlan[height][width]->type != type_border) {
                            // If non rock found, try another round
                            valid = false;
                            break;
                        }
                    }
                }
                    // Or valid is false and this for loop needs to terminate now
                else {
                    break;
                }
            }

            if (valid) {
                break;
            }
        } while (maxDoWhile < 101);

        if (maxDoWhile < 101) {
            // Valid room found, create it
            floor->rooms[index] = room_initialize(startX, startY, roomWidth, roomHeight);

            for (height = startY; height < startY + roomHeight; height++) {
                for (width = startX; width < startX + roomWidth; width++) {
                    floor->floorPlan[height][width]->type = type_room;
                    floor->floorPlan[height][width]->internalObject = floor->rooms[index];
                    floor->floorPlan[height][width]->hardness = 0;
                    floor->floorPlan[height][width]->character = ROOM_CHARACTER;
                }
            }
        } else {
            printf("Failed to create room %d...\n", index);
        }
    }
}

void floor_generate_staircases(Floor* floor) {
    floor->stairUp = NULL;
    floor->stairDown = NULL;

    bool makeUp = true;
    bool makeDown = true;

    // Can't make a down floor on the bottom floor
    if (floor->floorNumber == 0) {
        makeDown = false;
    }
    // Can't make up floor on the top floor
    if (floor->floorNumber == floor->maxFloors - 1) {
        makeUp = false;
    }

    if (makeUp) {
        u_char stairUpRoom = randomNumberBetween(0, floor->roomCount - 1);

        u_char stairUpX = randomNumberBetween(floor->rooms[stairUpRoom]->startX, floor->rooms[stairUpRoom]->startX + floor->rooms[stairUpRoom]->width - 1);
        u_char stairUpY = randomNumberBetween(floor->rooms[stairUpRoom]->startY, floor->rooms[stairUpRoom]->startY + floor->rooms[stairUpRoom]->height - 1);

        floor->stairUp = staircase_initialize(stairUpX, stairUpY, floor->floorNumber, floor->floorNumber + 1);

        floor->floorPlan[stairUpY][stairUpX]->hardness = 0;
        floor->floorPlan[stairUpY][stairUpX]->type = type_staircaseUp;
        floor->floorPlan[stairUpY][stairUpX]->character = STAIRCASE_UP;
        floor->floorPlan[stairUpY][stairUpX]->internalObject = floor->stairUp;
    }

    if (makeDown) {
        u_char stairDownRoom = randomNumberBetween(0, floor->roomCount - 1);

        u_char stairDownX = randomNumberBetween(floor->rooms[stairDownRoom]->startX, floor->rooms[stairDownRoom]->startX + floor->rooms[stairDownRoom]->width - 1);
        u_char stairDownY = randomNumberBetween(floor->rooms[stairDownRoom]->startY, floor->rooms[stairDownRoom]->startY + floor->rooms[stairDownRoom]->height - 1);

        floor->stairDown = staircase_initialize(stairDownX, stairDownY, floor->floorNumber, floor->floorNumber + 1);

        floor->floorPlan[stairDownY][stairDownX]->hardness = 0;
        floor->floorPlan[stairDownY][stairDownX]->type = type_staircaseDown;
        floor->floorPlan[stairDownY][stairDownX]->character = STAIRCASE_DOWN;
        floor->floorPlan[stairDownY][stairDownX]->internalObject = floor->stairDown;
    }
}

void floor_generate_corridors(Floor* floor) {
    return;
}