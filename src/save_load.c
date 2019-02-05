#include "save_load.h"

int save_to_file(void* dungeonPointer) {
    Dungeon* dungeon = (Dungeon*) dungeonPointer;
    if (dungeon->settings->doSave) {
        // Move current file to a tmp file in case save fails
        char* tempName = malloc((strlen(dungeon->settings->savePath) * sizeof(char)) + (strlen(".tmp") * sizeof(char)) + 1);
        sprintf(tempName, "%s%s", dungeon->settings->savePath, ".tmp");
        rename(dungeon->settings->savePath, tempName);
        free(tempName);

        // Open the file
        FILE* file = fopen(dungeon->settings->savePath, "wb+");

        if (file == NULL) {
            printf("Failed to open file at path %s\n", dungeon->settings->savePath);
            return 1;
        }

        char fileHeading[] = FILE_HEADING;
        u_int fileVersion = dungeon->settings->file_version;
        u_int fileVersion_be32 = htobe32(fileVersion);
        u_int fileSize = 0;

        // Write file heading
        if (error_check_fwrite(fileHeading, sizeof(char), strlen(fileHeading), file) != 0) {
            printf("Failed on the first one... lol fwrite is broke af\n");
            return 1;
        }
        // Write file version
        if (error_check_fwrite(&(fileVersion_be32), sizeof(fileVersion), 1, file) != 0) {
            return 1;
        }
        // Write file size, this will be over written later
        if (error_check_fwrite(&fileSize, sizeof(fileSize), 1, file) != 0) {
            return 1;
        }
        // Write player location
        if (error_check_fwrite(&(dungeon->player->x), sizeof(dungeon->player->x), 1, file) != 0) {
            return 1;
        }
        if (error_check_fwrite(&(dungeon->player->y), sizeof(dungeon->player->y), 1, file) != 0) {
            return 1;
        }
        // Write current floor of player
        // Write max floors
        if (fileVersion == 1) {
            if (error_check_fwrite(&(dungeon->player->floor), sizeof(dungeon->player->floor), 1, file) != 0) {
                return 1;
            }
            if (error_check_fwrite(&(dungeon->floorCount), sizeof(dungeon->floorCount), 1, file) != 0) {
                return 1;
            }
        }

        if (fileVersion == 1) {
            u_char index;
            for (index = 0; index < dungeon->floorCount; index++) {
                // Save each floor
                if (save_floor(file, dungeon, index) != 0) {
                    return 1;
                }
            }
        } else {
            // Only save 1 floor and be done
            if (save_floor(file, dungeon, dungeon->currentFloor) != 0) {
                return 1;
            }
        }

        fflush(file);
        // Seek to end of file
        fseek(file, 0, SEEK_END);
        // Get current file pointer
        fileSize = htobe32(ftell(file));
        // Seek back to file size data index
        fseek(file, (sizeof(char) * strlen(fileHeading)) + sizeof(fileVersion), SEEK_SET);
        if (error_check_fwrite(&fileSize, sizeof(fileSize), 1, file)) {
            return 1;
        }

        fclose(file);
        file = NULL;

        tempName = malloc((strlen(dungeon->settings->savePath) * sizeof(char)) + (strlen(".tmp") * sizeof(char)) + 1);
        sprintf(tempName, "%s%s", dungeon->settings->savePath, ".tmp");
        remove(tempName);
        free(tempName);
    }

    return 0;
}

int save_floor(FILE* file, void* dungeonPointer, u_char floorIndex) {
    Dungeon* dungeon = (Dungeon*) dungeonPointer;

    u_int index;

    // Write dungeon floor plan
    u_char height;
    u_char width;
    for (height = 0; height < dungeon->floors[floorIndex]->height; height++) {
        for (width = 0; width < dungeon->floors[floorIndex]->width; width++) {
            if (error_check_fwrite(&(dungeon->floors[floorIndex]->floorPlan[height][width]->hardness), sizeof(u_char), 1, file) != 0) {
                return 1;
            }
        }
    }
    // Write number of rooms
    u_short roomCount = htobe16(dungeon->floors[floorIndex]->roomCount);
    if (error_check_fwrite(&roomCount, sizeof(roomCount), 1, file) != 0) {
        return 1;
    }
    // Write rooms
    for (index = 0; index < dungeon->floors[floorIndex]->roomCount; index++) {
        // Write x position
        if (error_check_fwrite(&(dungeon->floors[floorIndex]->rooms[index]->startX), sizeof(dungeon->floors[floorIndex]->rooms[index]->startX), 1, file) != 0) {
            return 1;
        }
        // Write y position
        if (error_check_fwrite(&(dungeon->floors[floorIndex]->rooms[index]->startY), sizeof(dungeon->floors[floorIndex]->rooms[index]->startY), 1, file) != 0) {
            return 1;
        }
        // Write width
        if (error_check_fwrite(&(dungeon->floors[floorIndex]->rooms[index]->width), sizeof(dungeon->floors[floorIndex]->rooms[index]->width), 1, file) != 0) {
            return 1;
        }
        // Write height
        if (error_check_fwrite(&(dungeon->floors[floorIndex]->rooms[index]->height), sizeof(dungeon->floors[floorIndex]->rooms[index]->height), 1, file) != 0) {
            return 1;
        }
    }
    // Write number of upward staircases
    u_short stairUpCount = htobe16(dungeon->floors[floorIndex]->stairUpCount);
    if (error_check_fwrite(&stairUpCount, sizeof(stairUpCount), 1, file) != 0) {
        return 1;
    }
    // Write location of upward staircases
    for (index = 0; index < dungeon->floors[floorIndex]->stairUpCount; index++) {
        if (dungeon->floors[floorIndex]->stairUp[index] != NULL) {
            // Write x position
            if (error_check_fwrite(&(dungeon->floors[floorIndex]->stairUp[index]->x), sizeof(dungeon->floors[floorIndex]->stairUp[index]->x), 1, file) != 0) {
                return 1;
            }
            // Write y position
            if (error_check_fwrite(&(dungeon->floors[floorIndex]->stairUp[index]->y), sizeof(dungeon->floors[floorIndex]->stairUp[index]->y), 1, file) != 0) {
                return 1;
            }
        }
    }
    // Write number of downward staircases
    u_short stairDownCount = htobe16(dungeon->floors[floorIndex]->stairDownCount);
    if (error_check_fwrite(&stairDownCount, sizeof(stairDownCount), 1, file) != 0) {
        return 1;
    }
    // Write location of downward staircases
    for (index = 0; index < dungeon->floors[floorIndex]->stairDownCount; index++) {
        if (dungeon->floors[floorIndex]->stairDown[index] != NULL) {
            // Write x position
            if (error_check_fwrite(&(dungeon->floors[floorIndex]->stairDown[index]->x), sizeof(dungeon->floors[floorIndex]->stairDown[index]->x), 1, file) != 0) {
                return 1;
            }
            // Write y position
            if (error_check_fwrite(&(dungeon->floors[floorIndex]->stairDown[index]->y), sizeof(dungeon->floors[floorIndex]->stairDown[index]->y), 1, file) != 0) {
                return 1;
            }
        }
    }

    return 0;
}

void save_error(void* dungeonPointer) {
    Dungeon* dungeon = (Dungeon*) dungeonPointer;

    printf("Failed to save file\n");
    printf("Attempting to restore original file contents...\n");
    char* tempName = malloc((strlen(dungeon->settings->savePath) * sizeof(char)) + (strlen(".tmp") * sizeof(char)) + 1);
    sprintf(tempName, "%s%s", dungeon->settings->savePath, ".tmp");
    rename(tempName, dungeon->settings->savePath);
    free(tempName);
    printf("File contents should have been restored\n");
}

int load_from_file(void* dungeonPointer) {
    Dungeon* dungeon = (Dungeon*) dungeonPointer;

    if (dungeon->settings->doLoad) {
        FILE* file = fopen(dungeon->settings->loadPath, "rb");

        if (file == NULL) {
            printf("Failed to open file at path %s\n", dungeon->settings->savePath);
            return 1;
        }

        // Read the file heading. Validate that it is the proper heading
        char fileHeading[13];
        if (error_check_fread(fileHeading, sizeof(fileHeading) - 1, 1, file) != 0) {
            return 1;
        }
        fileHeading[12] = '\0';
        if (strncmp(fileHeading, FILE_HEADING, 12)) {
            printf("File presented is not a RLG327 save file.\n");
            return 1;
        }
        // Read the file version. Validate it against the current version marking
        u_int fileVersion;
        if (error_check_fread(&fileVersion, sizeof(fileVersion), 1, file) != 0) {
            return 1;
        }
        fileVersion = be32toh(fileVersion);
        if (fileVersion != dungeon->settings->file_version) {
            printf("Invalid file version. Tried version %d against saved version %d Unable to read.\n", fileVersion, dungeon->settings->file_version);
            return 1;
        }
        // Read the file size.
        u_int fileSize;
        if (error_check_fread(&fileSize, sizeof(fileSize), 1, file) != 0) {
            return 1;
        }
        fileSize = be32toh(fileSize);
        // Seek to end of file
        fseek(file, 0, SEEK_END);
        u_int actualFileSize = ftell(file);
        fseek(file, (sizeof(char) * strlen(fileHeading)) + sizeof(fileVersion) + sizeof(fileSize), SEEK_SET);
        if (fileSize != actualFileSize) {
            printf("Invalid file sizes. Actual file size %u bits doesn't match header of %u bits\n", actualFileSize, fileSize);
            return 1;
        }

        // Load player location
        u_char playerX;
        u_char playerY;
        if (error_check_fread(&playerX, sizeof(playerX), 1, file) != 0) {
            return 1;
        }
        if (error_check_fread(&playerY, sizeof(playerY), 1, file) != 0) {
            return 1;
        }
        u_char playerFloor = 0;
        u_char floorCount = 1;
        // Load current floor of player
        // Load max floors
        if (fileVersion == 1) {
            if (error_check_fread(&playerFloor, sizeof(playerFloor), 1, file) != 0) {
                return 1;
            }
            if (error_check_fread(&floorCount, sizeof(floorCount), 1, file) != 0) {
                return 1;
            }
        }

        dungeon->floorCount = floorCount;
        dungeon->currentFloor = playerFloor;
        dungeon->floors = malloc(floorCount * sizeof(Floor));
        dungeon->player = player_initialize(playerX, playerY, dungeon->currentFloor);

        u_char index;
        for (index = 0; index < floorCount; index++) {
            // Load each floor
            if (load_floor(file, dungeon, index) != 0) {
                return 1;
            }
        }
        dungeon->floors[dungeon->player->floor]->floorPlan[dungeon->player->y][dungeon->player->x]->hardness = PLAYER_HARDNESS;
        dungeon->floors[dungeon->player->floor]->floorPlan[dungeon->player->y][dungeon->player->x]->type = type_player;
        dungeon->floors[dungeon->player->floor]->floorPlan[dungeon->player->y][dungeon->player->x]->character = PLAYER_CHARACTER;

        fclose(file);
        file = NULL;
    }

    return 0;
}

int load_floor(FILE* file, void* dungeonPointer, u_char floorIndex) {
    Dungeon* dungeon = (Dungeon*) dungeonPointer;

    FloorLoadStructure* floorLoadStructure = malloc(sizeof(FloorLoadStructure));
    floorLoadStructure->width = FLOOR_WIDTH;
    floorLoadStructure->height = FLOOR_HEIGHT;

    u_int index;
    u_char floorPlanHardness[FLOOR_HEIGHT][FLOOR_WIDTH];
    u_char floorPlanCharacters[FLOOR_HEIGHT][FLOOR_WIDTH];
    u_char height;
    u_char width;
    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            if (error_check_fread(&(floorPlanHardness[height][width]), sizeof(floorPlanHardness[height][width]), 1, file) != 0) {
                return 1;
            }
            switch (floorPlanHardness[height][width]) {
                case BORDER_HARDNESS:
                    floorPlanCharacters[height][width] = CORNER_WALL_CHARACTER;
                    break;
                case ROCK_HARDNESS:
                    floorPlanCharacters[height][width] = ROCK_CHARACTER;
                    break;
                case CORRIDOR_HARDNESS:
                    floorPlanCharacters[height][width] = CORRIDOR_CHARACTER;
                    break;
                default:
                    floorPlanCharacters[height][width] = ROCK_CHARACTER;
                    break;
            }
        }
    }

    // Load number of rooms
    u_short numberOfRooms;
    if (error_check_fread(&numberOfRooms, sizeof(numberOfRooms), 1, file) != 0) {
        return 1;
    }
    numberOfRooms = be16toh(numberOfRooms);
    floorLoadStructure->roomCount = numberOfRooms;
    // Load room locations
    u_char roomsX[numberOfRooms];
    u_char roomsY[numberOfRooms];
    u_char roomsWidth[numberOfRooms];
    u_char roomsHeight[numberOfRooms];
    for (index = 0; index < numberOfRooms; index++) {
        // Read x position
        if (error_check_fread(&(roomsX[index]), sizeof(roomsX[index]), 1, file) != 0) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(roomsY[index]), sizeof(roomsY[index]), 1, file) != 0) {
            return 1;
        }
        // Read width
        if (error_check_fread(&(roomsWidth[index]), sizeof(roomsWidth[index]), 1, file) != 0) {
            return 1;
        }
        // Read height
        if (error_check_fread(&(roomsHeight[index]), sizeof(roomsHeight[index]), 1, file) != 0) {
            return 1;
        }

        for (height = roomsY[index]; height < roomsY[index] + roomsHeight[index]; height++) {
            for (width = roomsX[index]; width < roomsX[index] + roomsWidth[index]; width++) {
                floorPlanCharacters[height][width] = ROOM_CHARACTER;
            }
        }
    }
    floorLoadStructure->roomsX = roomsX;
    floorLoadStructure->roomsY = roomsY;
    floorLoadStructure->roomsWidth = roomsWidth;
    floorLoadStructure->roomsHeight = roomsHeight;

    // Read number of upward staircases
    u_short stairUpCount;
    if (error_check_fread(&stairUpCount, sizeof(stairUpCount), 1, file) != 0) {
        return 1;
    }
    stairUpCount = be16toh(stairUpCount);
    floorLoadStructure->stairUpCount = stairUpCount;
    // Read location of upward staircases
    u_char stairUpX[stairUpCount];
    u_char stairUpY[stairUpCount];
    for (index = 0; index < stairUpCount; index++) {
        // Read x position
        if (error_check_fread(&(stairUpX[index]), sizeof(stairUpX[index]), 1, file) != 0) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(stairUpY[index]), sizeof(stairUpY[index]), 1, file) != 0) {
            return 1;
        }

        floorPlanCharacters[stairUpY[index]][stairUpX[index]] = STAIRCASE_UP_CHARACTER;
    }
    floorLoadStructure->stairUpX = stairUpX;
    floorLoadStructure->stairUpY = stairUpY;

    // Read number of downward staircases
    u_short stairDownCount;
    if (error_check_fread(&stairDownCount, sizeof(stairDownCount), 1, file) != 0) {
        return 1;
    }
    stairDownCount = be16toh(stairDownCount);
    floorLoadStructure->stairDownCount = stairDownCount;
    // Read location of downward staircases
    u_char stairDownX[stairDownCount];
    u_char stairDownY[stairDownCount];
    for (index = 0; index < stairDownCount; index++) {
        // Read x position
        if (error_check_fread(&(stairDownX[index]), sizeof(stairDownX[index]), 1, file) != 0) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(stairDownY[index]), sizeof(stairDownY[index]), 1, file) != 0) {
            return 1;
        }

        floorPlanCharacters[stairDownY[index]][stairDownX[index]] = STAIRCASE_DOWN_CHARACTER;
    }
    floorLoadStructure->stairDownX = stairDownX;
    floorLoadStructure->stairDownY = stairDownY;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            floorLoadStructure->floorPlanCharacters[height][width] = floorPlanCharacters[height][width];
            floorLoadStructure->floorPlanHardness[height][width] = floorPlanHardness[height][width];
        }
    }

    dungeon->floors[floorIndex] = floor_load_initialize(floorLoadStructure);

    free(floorLoadStructure);

    return 0;
}

void load_error() {
    printf("Failed to load the file. Invalid data presented\n");
}