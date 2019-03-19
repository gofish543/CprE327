#include "floor.h"

Floor* floor_initialize(Dungeon* dungeon, u_char floorNumber, u_short roomCount, u_short stairUpCount, u_short stairDownCount, u_char numberOfMonsters) {
    Floor* floor =(Floor*) malloc(sizeof(Floor));

    floor->dungeon = dungeon;
    floor->floorNumber = floorNumber;

    floor->roomCount = roomCount;
    floor->stairUpCount = stairUpCount;
    floor->stairDownCount = stairDownCount;

    if (floor->dungeon->settings->doNumberOfMonsters) {
        floor->monsterCount = floor->dungeon->settings->numberOfMonsters;
    } else {
        floor->monsterCount = numberOfMonsters;
    }

    floor->rooms =(Room**) malloc(floor->roomCount * sizeof(Room*));
    floor->upStairs = (Staircase**)malloc(floor->stairUpCount * sizeof(Staircase*));
    floor->downStairs = (Staircase**)malloc(floor->stairDownCount * sizeof(Staircase*));
    floor->monsters = (Monster**)malloc(floor->monsterCount * sizeof(Monster*));

    if (
            floor_generate_empty_characters(floor) ||
            floor_generate_empty_terrains(floor) ||
            floor_generate_borders(floor) ||
            floor_generate_rooms(floor) ||
            floor_generate_staircases(floor) ||
            floor_generate_corridors(floor)
            ) {
        print_error(dungeon->window, dungeon->settings->doNCursesPrint, "Failed to generate floor %d\n", floorNumber);
        exit(1);
        return null;
    }

    return floor;
}



Floor* floor_terminate(Floor* floor) {
    u_short index;
    u_char height;
    u_char width;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            floor->terrains[height][width] = terrain_terminate(floor->terrains[height][width]);
        }
    }
    for (index = 0; index < floor->roomCount; index++) {
        floor->rooms[index] = room_terminate(floor->rooms[index]);
    }

    for (index = 0; index < floor->monsterCount; index++) {
        floor->monsters[index] = monster_terminate(floor->monsters[index]);
    }

    for (index = 0; index < floor->stairDownCount; index++) {
        floor->downStairs[index] = staircase_terminate(floor->downStairs[index]);
    }

    for (index = 0; index < floor->stairUpCount; index++) {
        floor->upStairs[index] = staircase_terminate(floor->upStairs[index]);
    }

    free(floor->upStairs);
    free(floor->downStairs);
    free(floor->rooms);
    free(floor->monsters);
    free(floor);

    return null;
}

u_char floor_character_at(Floor* floor, u_char x, u_char y) {
    if (floor->characters[y][x] == null) {
        return floor->terrains[y][x]->character;
    } else {
        return floor->characters[y][x]->character;
    }
}

int floor_generate_empty_characters(Floor* floor) {
    u_char height;
    u_char width;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            floor->characters[height][width] = null;
        }
    }

    return 0;
}

int floor_generate_empty_terrains(Floor* floor) {
    u_char height;
    u_char width;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            floor->terrains[height][width] = terrain_initialize(floor, width, height, ROCK_CHARACTER, random_number_between(ROCK_HARDNESS_MIN, ROCK_HARDNESS_MAX));
            floor->terrains[height][width]->isRock = true;
        }
    }

    return 0;
}

int floor_generate_borders(Floor* floor) {
    u_char height;
    u_char width;

    // Set NORTH and SOUTH walls
    for (width = 0; width < FLOOR_WIDTH; width++) {
        floor->terrains[0][width]->isImmutable = true;
        floor->terrains[0][width]->isWalkable = false;
        floor->terrains[0][width]->isRock = false;
        floor->terrains[0][width]->hardness = BORDER_HARDNESS;
        floor->terrains[0][width]->character = NORTH_SOUTH_WALL_CHARACTER;

        floor->terrains[FLOOR_HEIGHT - 1][width]->isImmutable = true;
        floor->terrains[FLOOR_HEIGHT - 1][width]->isWalkable = false;
        floor->terrains[FLOOR_HEIGHT - 1][width]->isRock = false;
        floor->terrains[FLOOR_HEIGHT - 1][width]->hardness = BORDER_HARDNESS;
        floor->terrains[FLOOR_HEIGHT - 1][width]->character = NORTH_SOUTH_WALL_CHARACTER;
    }

    // Set EAST and WEST walls
    for (height = 0; height < FLOOR_HEIGHT; height++) {
        floor->terrains[height][0]->isImmutable = true;
        floor->terrains[height][0]->isWalkable = false;
        floor->terrains[height][0]->isRock = false;
        floor->terrains[height][0]->hardness = BORDER_HARDNESS;
        floor->terrains[height][0]->character = EAST_WEST_WALL_CHARACTER;

        floor->terrains[height][FLOOR_WIDTH - 1]->isImmutable = true;
        floor->terrains[height][FLOOR_WIDTH - 1]->isWalkable = false;
        floor->terrains[height][FLOOR_WIDTH - 1]->isRock = false;
        floor->terrains[height][FLOOR_WIDTH - 1]->hardness = BORDER_HARDNESS;
        floor->terrains[height][FLOOR_WIDTH - 1]->character = EAST_WEST_WALL_CHARACTER;
    }

    // Set top left corner
    floor->terrains[0][0]->character = CORNER_WALL_CHARACTER;

    // Set top right corner
    floor->terrains[0][FLOOR_WIDTH - 1]->character = CORNER_WALL_CHARACTER;

    // Set bottom left corner
    floor->terrains[FLOOR_HEIGHT - 1][0]->character = CORNER_WALL_CHARACTER;

    // Set bottom right corner
    floor->terrains[FLOOR_HEIGHT - 1][FLOOR_WIDTH - 1]->character = CORNER_WA
            LL_CHARACTER;

    return 0;
}

int floor_generate_rooms(Floor* floor) {
    u_char startX;
    u_char startY;

    u_char roomWidth;
    u_char roomHeight;

    u_char height;
    u_char width;

    u_short index;

    bool collision;
    for (index = 0; index < floor->roomCount; index++) {
        // Attempt to find a starting point randomly, do while
        do {
            collision = false;
            // Find something inside the game play box....
            startX = random_number_between(1, FLOOR_WIDTH - 2);
            startY = random_number_between(1, FLOOR_HEIGHT - 2);

            roomWidth = random_number_between(ROOM_MIN_WIDTH, ROOM_MAX_WIDTH);
            roomHeight = random_number_between(ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT);

            // Need to check boundaries one off to make sure they are open spaces
            for (height = startY - 1; height < (startY + roomHeight + 1); height++) {
                // Need to check boundaries one off to make sure they are open spaces
                for (width = startX - 1; width < (startX + roomWidth + 1); width++) {
                    if (width >= FLOOR_WIDTH - 1 || height >= FLOOR_HEIGHT - 1 || floor->terrains[height][width]->room != null) {
                        collision = true;
                    }
                }
            }
        } while (collision);

        // Valid room found, create it
        floor->rooms[index] = room_initialize(startX, startY, roomWidth, roomHeight);

        for (height = startY; height < startY + roomHeight; height++) {
            for (width = startX; width < startX + roomWidth; width++) {
                floor->terrains[height][width]->room = floor->rooms[index];
                floor->terrains[height][width]->character = ROOM_CHARACTER;
                floor->terrains[height][width]->hardness = ROOM_HARDNESS;

                floor->terrains[height][width]->isWalkable = true;
                floor->terrains[height][width]->isRock = false;
                floor->terrains[height][width]->isImmutable = false;
            }
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
        stairRoom = random_number_between(0, floor->roomCount - 1);

        // Select random spots until they are only surrounded by room space
        do {
            // Select random spot inside the room, not on edge
            stairX = random_number_between(floor->rooms[stairRoom]->startX + 1, floor->rooms[stairRoom]->startX + floor->rooms[stairRoom]->width - 2);
            stairY = random_number_between(floor->rooms[stairRoom]->startY + 1, floor->rooms[stairRoom]->startY + floor->rooms[stairRoom]->height - 2);
        } while (floor->terrains[stairY][stairX]->staircase != null);

        floor->upStairs[index] = staircase_initialize(stairX, stairY, floor->floorNumber, floor->floorNumber + 1);

        floor->terrains[stairY][stairX]->staircase = floor->upStairs[index];
        floor->terrains[stairY][stairX]->hardness = STAIRCASE_HARDNESS;
        floor->terrains[stairY][stairX]->character = STAIRCASE_UP_CHARACTER;
    }

    for (index = 0; index < floor->stairDownCount; index++) {
        stairRoom = random_number_between(0, floor->roomCount - 1);

        // Select random spots until they are only surrounded by room space
        do {
            // Select random spot inside the room, not on edge
            stairX = random_number_between(floor->rooms[stairRoom]->startX + 1, floor->rooms[stairRoom]->startX + floor->rooms[stairRoom]->width - 2);
            stairY = random_number_between(floor->rooms[stairRoom]->startY + 1, floor->rooms[stairRoom]->startY + floor->rooms[stairRoom]->height - 2);
        } while (floor->terrains[stairY][stairX]->staircase != null);

        floor->downStairs[index] = staircase_initialize(stairX, stairY, floor->floorNumber, floor->floorNumber - 1);

        floor->terrains[stairY][stairX]->staircase = floor->downStairs[index];
        floor->terrains[stairY][stairX]->hardness = STAIRCASE_HARDNESS;
        floor->terrains[stairY][stairX]->character = STAIRCASE_DOWN_CHARACTER;
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
            firstRoomX = random_number_between(floor->rooms[index]->startX, floor->rooms[index]->startX + floor->rooms[index]->width - 1);
            firstRoomY = random_number_between(floor->rooms[index]->startY, floor->rooms[index]->startY + floor->rooms[index]->height - 1);

            upValid = floor->terrains[firstRoomY - 1][firstRoomX]->isRock;
            downValid = floor->terrains[firstRoomY + 1][firstRoomX]->isRock;
            leftValid = floor->terrains[firstRoomY][firstRoomX - 1]->isRock;
            rightValid = floor->terrains[firstRoomY][firstRoomX + 1]->isRock;
        } while (upValid || downValid || leftValid || rightValid);

        // Second we want to select a random spot within the next room, but it needs to be on the border
        do {
            secondRoomX = random_number_between(floor->rooms[index + 1]->startX, floor->rooms[index + 1]->startX + floor->rooms[index + 1]->width - 1);
            secondRoomY = random_number_between(floor->rooms[index + 1]->startY, floor->rooms[index + 1]->startY + floor->rooms[index + 1]->height - 1);

            upValid = floor->terrains[secondRoomY - 1][secondRoomX]->isRock;
            downValid = floor->terrains[secondRoomY + 1][secondRoomX]->isRock;
            leftValid = floor->terrains[secondRoomY][secondRoomX - 1]->isRock;
            rightValid = floor->terrains[secondRoomY][secondRoomX + 1]->isRock;
        } while (upValid || downValid || leftValid || rightValid);

        // Now connect them in the X direction
        tempX = firstRoomX;
        while (tempX != secondRoomX) {
            if (tempX > secondRoomX) {
                tempX--;
            } else if (tempX < secondRoomX) {
                tempX++;
            }

            if (floor->terrains[secondRoomY][tempX]->isRock) {
                floor->terrains[secondRoomY][tempX]->isWalkable = true;
                floor->terrains[secondRoomY][tempX]->isRock = false;
                floor->terrains[secondRoomY][tempX]->hardness = CORRIDOR_HARDNESS;
                floor->terrains[secondRoomY][tempX]->character = CORRIDOR_CHARACTER;
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

            if (floor->terrains[tempY][firstRoomX]->isRock) {
                floor->terrains[tempY][firstRoomX]->isWalkable = true;
                floor->terrains[tempY][firstRoomX]->isRock = false;
                floor->terrains[tempY][firstRoomX]->hardness = CORRIDOR_HARDNESS;
                floor->terrains[tempY][firstRoomX]->character = CORRIDOR_CHARACTER;
            }
        }

        // Handle the corner case
        if (floor->terrains[secondRoomY][firstRoomX]->isRock) {
            floor->terrains[secondRoomY][firstRoomX]->isWalkable = true;
            floor->terrains[secondRoomY][firstRoomX]->isRock = false;
            floor->terrains[secondRoomY][firstRoomX]->hardness = CORRIDOR_HARDNESS;
            floor->terrains[secondRoomY][firstRoomX]->character = CORRIDOR_CHARACTER;
        }
    }

    return 0;
}

int floor_generate_monsters(Floor* floor) {
    u_char width;
    u_char height;

    u_short index;

    u_char monsterX;
    u_char monsterY;
    u_short monsterRoom;

    u_char classification;
    u_char speed;

    Room* playerRoom = floor->terrains[floor->dungeon->player->character->y][floor->dungeon->player->character->x]->room;

    u_int placementAttempts = 0;
    u_short maxMonsters = 0;
    u_char roomIndex;

    // Find out the max number of monsters possible within the program
    for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
        if (floor->rooms[roomIndex] == playerRoom) {
            continue;
        } else {
            maxMonsters += (floor->rooms[roomIndex]->width * floor->rooms[roomIndex]->height);
        }
    }

    // If there are more monsters trying to be placed than monsters available, set the max number of monsters
    if (floor->monsterCount > maxMonsters) {
        floor->monsterCount = maxMonsters;
        // Reassign max monsters
        free(floor->monsters);
        floor->monsters = (Monster**) malloc(floor->monsterCount * sizeof(Monster*));
    }

    for (index = 0; index < floor->monsterCount; index++) {
        placementAttempts = 0;

        speed = random_number_between(MONSTER_MIN_SPEED, MONSTER_MAX_SPEED);
        classification = 0;

        // If Intelligent
        if (random_number_between(false, true)) {
            classification |= MONSTER_INTELLIGENT_VALUE;
        }
        // If Telepathic
        if (random_number_between(false, true)) {
            classification |= MONSTER_TELEPATHIC_VALUE;
        }
        // If Tunneler
        if (random_number_between(false, true)) {
            classification |= MONSTER_TUNNELER_VALUE;
        }
        // If Erratic
        if (random_number_between(false, true)) {
            classification |= MONSTER_ERRATIC_VALUE;
        }


        // Select random spots until they are only surrounded by room space
        do {
            do {
                monsterRoom = random_number_between(0, floor->roomCount - 1);
            } while (floor->rooms[monsterRoom] == playerRoom);

            // Select random spot inside the room
            monsterX = random_number_between(floor->rooms[monsterRoom]->startX, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 1);
            monsterY = random_number_between(floor->rooms[monsterRoom]->startY, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 1);

            placementAttempts++;
        } while (floor->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
                if (floor->rooms[roomIndex] == playerRoom) {
                    continue;
                }
                // Start looping and find the next open spot
                for (height = floor->rooms[roomIndex]->startY; height < floor->rooms[roomIndex]->startY + floor->rooms[roomIndex]->height; height++) {
                    for (width = floor->rooms[roomIndex]->startX; width < floor->rooms[roomIndex]->startX + floor->rooms[roomIndex]->width; width++) {
                        if (floor->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        floor->monsters[index] = monster_initialize(floor, monsterX, monsterY, classification, speed);
        floor->characters[monsterY][monsterX] = floor->monsters[index]->character;
    }

    return 0;
}