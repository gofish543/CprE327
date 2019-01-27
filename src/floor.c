#include "floor.h"

Floor* floor_initialize(u_char floorNumber, u_char maxFloors, u_char stairsPerFloor) {
    Floor* floor = malloc(sizeof(Floor));

    floor->width = FLOOR_WIDTH;
    floor->height = FLOOR_HEIGHT;
    floor->roomCount = randomNumberBetween(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX);
    floor->stairCount = stairsPerFloor;

    floor->rooms = malloc(floor->roomCount * sizeof(Room*));
    floor->stairUp = malloc(floor->stairCount * sizeof(Staircase*));
    floor->stairDown = malloc(floor->stairCount * sizeof(Staircase*));

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

    for (index = 0; index < floor->stairCount; index++) {
        if (floor->stairDown[index] != NULL) {
            floor->stairDown[index] = staircase_terminate(floor->stairDown[index]);
        }
    }

    for (index = 0; index < floor->stairCount; index++) {
        if (floor->stairUp[index] != NULL) {
            floor->stairUp[index] = staircase_terminate(floor->stairUp[index]);
        }
    }


    free(floor->stairUp);
    free(floor->stairDown);
    free(floor->rooms);
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
        u_char stairUpX;
        u_char stairUpY;
        u_char stairUpRoom;

        u_char index;
        for (index = 0; index < floor->stairCount; index++) {
            stairUpRoom = randomNumberBetween(0, floor->roomCount - 1);

            // Select random spots until they are only surrounded by room space
            do {
                // Select random spot inside the room, not on edge
                stairUpX = randomNumberBetween(floor->rooms[stairUpRoom]->startX + 1, floor->rooms[stairUpRoom]->startX + floor->rooms[stairUpRoom]->width - 2);
                stairUpY = randomNumberBetween(floor->rooms[stairUpRoom]->startY + 1, floor->rooms[stairUpRoom]->startY + floor->rooms[stairUpRoom]->height - 2);
            } while (
                    floor->floorPlan[stairUpY - 1][stairUpX]->character != ROOM_CHARACTER ||
                    floor->floorPlan[stairUpY + 1][stairUpX]->character != ROOM_CHARACTER ||
                    floor->floorPlan[stairUpY][stairUpX - 1]->character != ROOM_CHARACTER ||
                    floor->floorPlan[stairUpY][stairUpX + 1]->character != ROOM_CHARACTER
                    );

            floor->stairUp[index] = staircase_initialize(stairUpX, stairUpY, floor->floorNumber, floor->floorNumber + 1);

            floor->floorPlan[stairUpY][stairUpX]->hardness = 0;
            floor->floorPlan[stairUpY][stairUpX]->type = type_staircaseUp;
            floor->floorPlan[stairUpY][stairUpX]->character = STAIRCASE_UP;
            floor->floorPlan[stairUpY][stairUpX]->internalObject = floor->stairUp;
        }
    }

    if (makeDown) {
        u_char stairDownX;
        u_char stairDownY;
        u_char stairDownRoom;

        u_char index;
        for (index = 0; index < floor->stairCount; index++) {
            stairDownRoom = randomNumberBetween(0, floor->roomCount - 1);

            // Select random spots until they are only surrounded by room space
            do {
                // Select random spot inside the room, not on edge
                stairDownX = randomNumberBetween(floor->rooms[stairDownRoom]->startX + 1, floor->rooms[stairDownRoom]->startX + floor->rooms[stairDownRoom]->width - 2);
                stairDownY = randomNumberBetween(floor->rooms[stairDownRoom]->startY + 1, floor->rooms[stairDownRoom]->startY + floor->rooms[stairDownRoom]->height - 2);
            } while (
                    floor->floorPlan[stairDownY - 1][stairDownX]->character != ROOM_CHARACTER ||
                    floor->floorPlan[stairDownY + 1][stairDownX]->character != ROOM_CHARACTER ||
                    floor->floorPlan[stairDownY][stairDownX - 1]->character != ROOM_CHARACTER ||
                    floor->floorPlan[stairDownY][stairDownX + 1]->character != ROOM_CHARACTER
                    );

            floor->stairDown[index] = staircase_initialize(stairDownX, stairDownY, floor->floorNumber, floor->floorNumber + 1);

            floor->floorPlan[stairDownY][stairDownX]->hardness = 0;
            floor->floorPlan[stairDownY][stairDownX]->type = type_staircaseDown;
            floor->floorPlan[stairDownY][stairDownX]->character = STAIRCASE_DOWN;
            floor->floorPlan[stairDownY][stairDownX]->internalObject = floor->stairDown;
        }
    }
}

void floor_generate_corridors(Floor* floor) {
    bool upValid;
    bool downValid;
    bool leftValid;
    bool rightValid;

    u_char firstRoomX;
    u_char firstRoomY;

    u_char secondRoomX;
    u_char secondRoomY;

    u_char tempX;
    u_char tempY;

    u_char index;
    for (index = 0; index < floor->roomCount - 1; index++) {
        // First we want to select a random spot within the room, but it needs to be on the border
        do {
            firstRoomX = randomNumberBetween(floor->rooms[index]->startX, floor->rooms[index]->startX + floor->rooms[index]->width - 1);
            firstRoomY = randomNumberBetween(floor->rooms[index]->startY, floor->rooms[index]->startY + floor->rooms[index]->height - 1);

            upValid = floor->floorPlan[firstRoomY - 1][firstRoomX]->type == type_rock;
            downValid = floor->floorPlan[firstRoomY + 1][firstRoomX]->type == type_rock;
            leftValid = floor->floorPlan[firstRoomY][firstRoomX - 1]->type == type_rock;
            rightValid = floor->floorPlan[firstRoomY][firstRoomX + 1]->type == type_rock;
        } while (upValid || downValid || leftValid || rightValid);

        // Second we want to select a random spot within the next room, but it needs to be on the border
        do {
            secondRoomX = randomNumberBetween(floor->rooms[index + 1]->startX, floor->rooms[index + 1]->startX + floor->rooms[index + 1]->width - 1);
            secondRoomY = randomNumberBetween(floor->rooms[index + 1]->startY, floor->rooms[index + 1]->startY + floor->rooms[index + 1]->height - 1);

            upValid = floor->floorPlan[secondRoomY - 1][secondRoomX]->type == type_rock;
            downValid = floor->floorPlan[secondRoomY + 1][secondRoomX]->type == type_rock;
            leftValid = floor->floorPlan[secondRoomY][secondRoomX - 1]->type == type_rock;
            rightValid = floor->floorPlan[secondRoomY][secondRoomX + 1]->type == type_rock;
        } while (upValid || downValid || leftValid || rightValid);

        // Now connect them in the X direction
        tempX = firstRoomX;
        while (tempX != secondRoomX) {
            if (tempX > secondRoomX) {
                tempX--;
            } else if (tempX < secondRoomX) {
                tempX++;
            }

            if (floor->floorPlan[secondRoomY][tempX]->type == type_rock) {
                floor->floorPlan[secondRoomY][tempX]->type = type_corridor;
                floor->floorPlan[secondRoomY][tempX]->hardness = 0;
                floor->floorPlan[secondRoomY][tempX]->character = FLOOR_CORRIDOR;
                floor->floorPlan[secondRoomY][tempX]->internalObject = NULL;
            }
        }

        // Now connect them in the Y direction
        tempY = secondRoomY;
        while (tempY != firstRoomY) {
            if (tempY > firstRoomY) {
                tempY--;
            } else if (tempY < firstRoomY) {
                tempY++;
            }

            if (floor->floorPlan[tempY][firstRoomX]->type == type_rock) {
                floor->floorPlan[tempY][firstRoomX]->type = type_corridor;
                floor->floorPlan[tempY][firstRoomX]->hardness = 0;
                floor->floorPlan[tempY][firstRoomX]->character = FLOOR_CORRIDOR;
                floor->floorPlan[tempY][firstRoomX]->internalObject = NULL;
            }
        }

        // Handle the corner case
        if (floor->floorPlan[secondRoomY][firstRoomX]->type == type_rock) {
            floor->floorPlan[secondRoomY][firstRoomX]->type = type_corridor;
            floor->floorPlan[secondRoomY][firstRoomX]->hardness = 0;
            floor->floorPlan[secondRoomY][firstRoomX]->character = FLOOR_CORRIDOR;
            floor->floorPlan[secondRoomY][firstRoomX]->internalObject = NULL;
        }
    }
}





















