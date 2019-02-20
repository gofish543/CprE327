#include "floor.h"

Floor* floor_load_initialize(FloorLoadStructure* floorLoadStructure) {
    Floor* floor = malloc(sizeof(Floor));

    floor->dungeon = floorLoadStructure->dungeon;
    floor->width = floorLoadStructure->width;
    floor->height = floorLoadStructure->height;
    floor->roomCount = floorLoadStructure->roomCount;
    floor->stairUpCount = floorLoadStructure->stairUpCount;
    floor->stairDownCount = floorLoadStructure->stairDownCount;
    floor->monsterCount = floorLoadStructure->monsterCount;

    floor->floorNumber = floorLoadStructure->floorNumber;

    floor->rooms = malloc(floor->roomCount * sizeof(Room*));
    floor->stairUp = malloc(floor->stairUpCount * sizeof(Staircase*));
    floor->stairDown = malloc(floor->stairDownCount * sizeof(Staircase*));
    floor->monsters = malloc(floor->monsterCount * sizeof(Monster*));

    u_char height;
    u_char width;

    for (height = 0; height < floor->height; height++) {
        for (width = 0; width < floor->width; width++) {
            enum FloorDotType type = type_rock;
            switch (floorLoadStructure->floorPlanCharacters[height][width]) {
                case ROCK_CHARACTER:
                    type = type_rock;
                    break;
                case PLAYER_CHARACTER:
                    type = type_player;
                    break;
                case CORRIDOR_CHARACTER:
                    type = type_corridor;
                    break;
                case ROOM_CHARACTER:
                    type = type_room;
                    break;
                case STAIRCASE_UP_CHARACTER:
                    type = type_staircase_up;
                    break;
                case STAIRCASE_DOWN_CHARACTER:
                    type = type_staircase_down;
                    break;
            }
            floor->floorPlan[height][width] = floor_dot_initialize(width, height, type, floorLoadStructure->floorPlanHardness[height][width], floorLoadStructure->floorPlanCharacters[height][width]);
        }
    }

    u_short index;
    for (index = 0; index < floorLoadStructure->roomCount; index++) {
        floor->rooms[index] = room_initialize(floorLoadStructure->roomsX[index], floorLoadStructure->roomsY[index], floorLoadStructure->roomsWidth[index], floorLoadStructure->roomsHeight[index]);
    }

    for (index = 0; index < floorLoadStructure->stairUpCount; index++) {
        floor->stairUp[index] = staircase_initialize(floorLoadStructure->stairUpX[index], floorLoadStructure->stairUpY[index], floorLoadStructure->floorNumber, floorLoadStructure->floorNumber + 1);
    }

    for (index = 0; index < floorLoadStructure->stairDownCount; index++) {
        floor->stairDown[index] = staircase_initialize(floorLoadStructure->stairDownX[index], floorLoadStructure->stairDownY[index], floorLoadStructure->floorNumber, floorLoadStructure->floorNumber - 1);
    }

    if (floor_generate_borders(floor) != 0) {
        printf("Failed to generate borders\n");
        exit(1);
        return null;
    }

    return floor;
}

Floor* floor_initialize(Dungeon* dungeon, u_char floorNumber, u_short stairUpCount, u_short stairDownCount) {
    Floor* floor = malloc(sizeof(Floor));

    floor->dungeon = dungeon;
    floor->floorNumber = floorNumber;
    floor->width = FLOOR_WIDTH;
    floor->height = FLOOR_HEIGHT;
    floor->roomCount = randomNumberBetween(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX);
    floor->stairUpCount = stairUpCount;
    floor->stairDownCount = stairDownCount;
    if (floor->dungeon->settings->doNumberOfMonsters) {
        floor->monsterCount = floor->dungeon->settings->numberOfMonsters;
    } else {
        floor->monsterCount = randomNumberBetween(FLOOR_MONSTERS_MIN, FLOOR_MONSTERS_MAX);
    }

    floor->rooms = malloc(floor->roomCount * sizeof(Room*));
    floor->stairUp = malloc(floor->stairUpCount * sizeof(Staircase*));
    floor->stairDown = malloc(floor->stairDownCount * sizeof(Staircase*));
    floor->monsters = malloc(floor->monsterCount * sizeof(Monster*));

    if (floor_generate_empty_dots(floor) != 0 ||
        floor_generate_borders(floor) != 0 ||
        floor_generate_rooms(floor) != 0 ||
        floor_generate_staircases(floor) != 0 ||
        floor_generate_corridors(floor) != 0) {
        printf("Failed to generate floor %d", floorNumber);
        exit(1);
        return null;
    }

    return floor;
}

Floor* floor_terminate(Floor* floor) {
    u_short index;
    u_char height;
    u_char width;

    for (height = 0; height < floor->height; height++) {
        for (width = 0; width < floor->width; width++) {
            floor->floorPlan[height][width] = floor_dot_terminate(floor->floorPlan[height][width]);
        }
    }
    for (index = 0; index < floor->roomCount; index++) {
        floor->rooms[index] = room_terminate(floor->rooms[index]);
    }

    for (index = 0; index < floor->monsterCount; index++) {
        floor->monsters[index] = monster_terminate(floor->monsters[index]);
    }

    for (index = 0; index < floor->stairDownCount; index++) {
        floor->stairDown[index] = staircase_terminate(floor->stairDown[index]);
    }

    for (index = 0; index < floor->stairUpCount; index++) {
        floor->stairUp[index] = staircase_terminate(floor->stairUp[index]);
    }

    free(floor->stairUp);
    free(floor->stairDown);
    free(floor->rooms);
    free(floor->monsters);
    free(floor);

    return null;
}

FloorDot* floor_dot_initialize(u_char x, u_char y, enum FloorDotType floorDotType, u_char hardness, u_char character) {
    FloorDot* floorDot = malloc(sizeof(FloorDot));

    floorDot->x = x;
    floorDot->y = y;

    floorDot->type = floorDotType;

    floorDot->hardness = hardness;
    floorDot->character = character;

    return floorDot;
}

FloorDot* floor_dot_terminate(FloorDot* floorDot) {
    free(floorDot);

    return null;
}

int floor_generate_empty_dots(Floor* floor) {
    u_char height;
    u_char width;

    for (height = 0; height < floor->height; height++) {
        for (width = 0; width < floor->width; width++) {
            floor->floorPlan[height][width] = floor_dot_initialize(width, height, type_rock, randomNumberBetween(ROCK_HARDNESS_MIN, ROCK_HARDNESS_MAX), ROCK_CHARACTER);
        }
    }

    return 0;
}

int floor_generate_borders(Floor* floor) {
    u_char height;
    u_char width;

    // Set NORTH and SOUTH walls
    for (width = 0; width < floor->width; width++) {
        floor->floorPlan[0][width]->type = type_border;
        floor->floorPlan[0][width]->hardness = BORDER_HARDNESS;
        floor->floorPlan[0][width]->character = NORTH_SOUTH_WALL_CHARACTER;

        floor->floorPlan[floor->height - 1][width]->type = type_border;
        floor->floorPlan[floor->height - 1][width]->hardness = BORDER_HARDNESS;
        floor->floorPlan[floor->height - 1][width]->character = NORTH_SOUTH_WALL_CHARACTER;
    }

    // Set EAST and WEST walls
    for (height = 0; height < floor->height; height++) {
        floor->floorPlan[height][0]->type = type_border;
        floor->floorPlan[height][0]->hardness = BORDER_HARDNESS;
        floor->floorPlan[height][0]->character = EAST_WEST_WALL_CHARACTER;

        floor->floorPlan[height][floor->width - 1]->type = type_border;
        floor->floorPlan[height][floor->width - 1]->hardness = BORDER_HARDNESS;
        floor->floorPlan[height][floor->width - 1]->character = EAST_WEST_WALL_CHARACTER;
    }

    // Set top left corner
    floor->floorPlan[0][0]->type = type_border;
    floor->floorPlan[0][0]->hardness = BORDER_HARDNESS;
    floor->floorPlan[0][0]->character = CORNER_WALL_CHARACTER;

    // Set top right corner
    floor->floorPlan[0][floor->width - 1]->type = type_border;
    floor->floorPlan[0][floor->width - 1]->hardness = BORDER_HARDNESS;
    floor->floorPlan[0][floor->width - 1]->character = CORNER_WALL_CHARACTER;

    // Set bottom left corner
    floor->floorPlan[floor->height - 1][0]->type = type_border;
    floor->floorPlan[floor->height - 1][0]->hardness = BORDER_HARDNESS;
    floor->floorPlan[floor->height - 1][0]->character = CORNER_WALL_CHARACTER;

    // Set bottom right corner
    floor->floorPlan[floor->height - 1][floor->width - 1]->type = type_border;
    floor->floorPlan[floor->height - 1][floor->width - 1]->hardness = BORDER_HARDNESS;
    floor->floorPlan[floor->height - 1][floor->width - 1]->character = CORNER_WALL_CHARACTER;

    return 0;
}

int floor_generate_rooms(Floor* floor) {
    u_char maxDoWhileAttempts = 100;
    u_char doWhileAttempts;

    u_char startX;
    u_char startY;

    u_char roomWidth;
    u_char roomHeight;

    u_char height;
    u_char width;

    u_short index;

    bool valid;
    for (index = 0; index < floor->roomCount; index++) {
        doWhileAttempts = 0;
        // Attempt to find a starting point randomly, do while
        do {
            valid = true;
            doWhileAttempts++;

            // Find something inside the game play box....
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
        } while (doWhileAttempts < maxDoWhileAttempts);

        if (doWhileAttempts < maxDoWhileAttempts) {
            // Valid room found, create it
            floor->rooms[index] = room_initialize(startX, startY, roomWidth, roomHeight);

            for (height = startY; height < startY + roomHeight; height++) {
                for (width = startX; width < startX + roomWidth; width++) {
                    floor->floorPlan[height][width]->type = type_room;
                    floor->floorPlan[height][width]->hardness = ROOM_HARDNESS;
                    floor->floorPlan[height][width]->character = ROOM_CHARACTER;
                }
            }
        } else {
            printf("Failed to create room %d...\n", index);
            return 1;
        }
    }

    return 0;
}

int floor_generate_staircases(Floor* floor) {
    // Can't make a down floor on the bottom floor
    if (floor->floorNumber == 0) {
        floor->stairDownCount = 0;
    }
    // Can't make up floor on the top floor
    if (floor->floorNumber == floor->dungeon->floorCount - 1) {
        floor->stairUpCount = 0;
    }

    u_short index;

    u_char stairX;
    u_char stairY;
    u_short stairRoom;

    for (index = 0; index < floor->stairUpCount; index++) {
        stairRoom = randomNumberBetween(0, floor->roomCount - 1);

        // Select random spots until they are only surrounded by room space
        do {
            // Select random spot inside the room, not on edge
            stairX = randomNumberBetween(floor->rooms[stairRoom]->startX + 1, floor->rooms[stairRoom]->startX + floor->rooms[stairRoom]->width - 2);
            stairY = randomNumberBetween(floor->rooms[stairRoom]->startY + 1, floor->rooms[stairRoom]->startY + floor->rooms[stairRoom]->height - 2);
        } while (
                floor->floorPlan[stairY - 1][stairX]->character != ROOM_CHARACTER ||
                floor->floorPlan[stairY + 1][stairX]->character != ROOM_CHARACTER ||
                floor->floorPlan[stairY][stairX - 1]->character != ROOM_CHARACTER ||
                floor->floorPlan[stairY][stairX + 1]->character != ROOM_CHARACTER
                );

        floor->stairUp[index] = staircase_initialize(stairX, stairY, floor->floorNumber, floor->floorNumber + 1);

        floor->floorPlan[stairY][stairX]->hardness = STAIRCASE_HARDNESS;
        floor->floorPlan[stairY][stairX]->type = type_staircase_up;
        floor->floorPlan[stairY][stairX]->character = STAIRCASE_UP_CHARACTER;
    }

    for (index = 0; index < floor->stairDownCount; index++) {
        stairRoom = randomNumberBetween(0, floor->roomCount - 1);

        // Select random spots until they are only surrounded by room space
        do {
            // Select random spot inside the room, not on edge
            stairX = randomNumberBetween(floor->rooms[stairRoom]->startX + 1, floor->rooms[stairRoom]->startX + floor->rooms[stairRoom]->width - 2);
            stairY = randomNumberBetween(floor->rooms[stairRoom]->startY + 1, floor->rooms[stairRoom]->startY + floor->rooms[stairRoom]->height - 2);
        } while (
                floor->floorPlan[stairY - 1][stairX]->character != ROOM_CHARACTER ||
                floor->floorPlan[stairY + 1][stairX]->character != ROOM_CHARACTER ||
                floor->floorPlan[stairY][stairX - 1]->character != ROOM_CHARACTER ||
                floor->floorPlan[stairY][stairX + 1]->character != ROOM_CHARACTER
                );

        floor->stairDown[index] = staircase_initialize(stairX, stairY, floor->floorNumber, floor->floorNumber + 1);

        floor->floorPlan[stairY][stairX]->hardness = STAIRCASE_HARDNESS;
        floor->floorPlan[stairY][stairX]->type = type_staircase_down;
        floor->floorPlan[stairY][stairX]->character = STAIRCASE_DOWN_CHARACTER;
    }

    return 0;
}

int floor_generate_corridors(Floor* floor) {
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

    u_short index;
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
                floor->floorPlan[secondRoomY][tempX]->hardness = CORRIDOR_HARDNESS;
                floor->floorPlan[secondRoomY][tempX]->character = CORRIDOR_CHARACTER;
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
                floor->floorPlan[tempY][firstRoomX]->hardness = CORRIDOR_HARDNESS;
                floor->floorPlan[tempY][firstRoomX]->character = CORRIDOR_CHARACTER;
            }
        }

        // Handle the corner case
        if (floor->floorPlan[secondRoomY][firstRoomX]->type == type_rock) {
            floor->floorPlan[secondRoomY][firstRoomX]->type = type_corridor;
            floor->floorPlan[secondRoomY][firstRoomX]->hardness = CORRIDOR_HARDNESS;
            floor->floorPlan[secondRoomY][firstRoomX]->character = CORRIDOR_CHARACTER;
        }
    }

    return 0;
}

int floor_generate_monsters(Floor* floor) {
    u_short index;

    u_char monsterX;
    u_char monsterY;
    u_short monsterRoom;

    u_char speed;
    bool intelligent;
    bool telepathic;
    bool tunneler;
    bool erratic;

    for (index = 0; index < floor->monsterCount; index++) {
        monsterRoom = randomNumberBetween(0, floor->roomCount - 1);

        speed = randomNumberBetween(MONSTER_MIN_SPEED, MONSTER_MAX_SPEED);

        intelligent = randomNumberBetween(false, true);
        telepathic = randomNumberBetween(false, true);
        tunneler = randomNumberBetween(false, true);
        erratic = randomNumberBetween(false, true);


        // Select random spots until they are only surrounded by room space
        do {
            if (tunneler) {
                // Select random spot inside the map, not on edge
                monsterX = randomNumberBetween(1, floor->width - 2);
                monsterY = randomNumberBetween(1, floor->height - 2);
            } else {
                // Select random spot inside the room, not on edge
                monsterX = randomNumberBetween(floor->rooms[monsterRoom]->startX + 1, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 2);
                monsterY = randomNumberBetween(floor->rooms[monsterRoom]->startY + 1, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 2);
            }
        } while (
                floor->floorPlan[monsterY][monsterX]->type == type_player ||
                floor->floorPlan[monsterY][monsterX]->type == type_monster
                );

        floor->monsters[index] = monster_initialize(floor->dungeon, monsterX, monsterY, floor->floorNumber, speed, intelligent, telepathic, tunneler, erratic);
    }

    return 0;
}