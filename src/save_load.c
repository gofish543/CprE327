#include "save_load.h"

bool dungeon_save_to_file(void* dungeonPointer) {
    Dungeon* dungeon = (Dungeon*) dungeonPointer;
    if (dungeon->settings->doSave) {
        FILE* file = fopen(dungeon->settings->savePath, "wb+");

        if (file == NULL) {
            printf("Failed to open file at path %s\n", dungeon->settings->savePath);
            return false;
        }

        char fileHeading[] = FILE_HEADING;
        u_int fileVersion = dungeon->settings->file_version;
        u_int fileVersion_be32 = htobe32(fileVersion);
        u_int fileSize = 0;

        // Write file heading
        fwrite(fileHeading, sizeof(char), strlen(fileHeading), file);
        // Write file version
        fwrite(&(fileVersion_be32), sizeof(fileVersion), 1, file);
        // Write file size, this will be over written later
        fwrite(&fileSize, sizeof(fileSize), 1, file);
        // Write player location
        fwrite(&(dungeon->player->x), sizeof(dungeon->player->x), 1, file);
        fwrite(&(dungeon->player->y), sizeof(dungeon->player->y), 1, file);
        // Write current floor of player
        // Write max floors
        if (fileVersion == 1) {
            fwrite(&(dungeon->player->floor), sizeof(dungeon->player->floor), 1, file);
            fwrite(&(dungeon->floorCount), sizeof(dungeon->floorCount), 1, file);
        }

        if (fileVersion == 1) {
            u_char index;
            for (index = 0; index < dungeon->floorCount; index++) {
                // Save each floor
                save_floor(file, dungeon, index);
            }
        } else {
            // Only save 1 floor and be done
            save_floor(file, dungeon, dungeon->currentFloor);
        }

        fflush(file);
        // Seek to end of file
        fseek(file, 0, SEEK_END);
        // Get current file pointer
        fileSize = htobe32(ftell(file));
        // Seek back to file size data index
        fseek(file, (sizeof(char) * strlen(fileHeading)) + sizeof(fileVersion), SEEK_SET);
        fwrite(&fileSize, sizeof(fileSize), 1, file);

        fclose(file);
        file = NULL;

        return true;
    }

    return false;
}

void save_floor(FILE* file, void* dungeonPointer, u_char floorIndex) {
    Dungeon* dungeon = (Dungeon*) dungeonPointer;

    u_char index;

    // Write dungeon floor plan
    u_char height;
    u_char width;
    for (height = 0; height < dungeon->floors[floorIndex]->height; height++) {
        for (width = 0; width < dungeon->floors[floorIndex]->width; width++) {
            fwrite(&(dungeon->floors[floorIndex]->floorPlan[height][width]->hardness), sizeof(u_char), 1, file);
        }
    }
    // Write number of rooms
    fwrite(&(dungeon->floors[floorIndex]->roomCount), sizeof(dungeon->floors[floorIndex]->roomCount), 1, file);
    // Write rooms
    for (index = 0; index < dungeon->floors[floorIndex]->roomCount; index++) {
        // Write x position
        fwrite(&(dungeon->floors[floorIndex]->rooms[index]->startX), sizeof(dungeon->floors[floorIndex]->rooms[index]->startX), 1, file);
        // Write y position
        fwrite(&(dungeon->floors[floorIndex]->rooms[index]->startY), sizeof(dungeon->floors[floorIndex]->rooms[index]->startY), 1, file);
        // Write width
        fwrite(&(dungeon->floors[floorIndex]->rooms[index]->width), sizeof(dungeon->floors[floorIndex]->rooms[index]->width), 1, file);
        // Write height
        fwrite(&(dungeon->floors[floorIndex]->rooms[index]->height), sizeof(dungeon->floors[floorIndex]->rooms[index]->height), 1, file);
    }
    // Write number of upward staircases
    fwrite(&(dungeon->floors[floorIndex]->stairUpCount), sizeof(dungeon->floors[floorIndex]->stairUpCount), 1, file);
    // Write location of upward staircases
    for (index = 0; index < dungeon->floors[floorIndex]->stairUpCount; index++) {
        if (dungeon->floors[floorIndex]->stairUp[index] != NULL) {
            // Write x position
            fwrite(&(dungeon->floors[floorIndex]->stairUp[index]->x), sizeof(dungeon->floors[floorIndex]->stairUp[index]->x), 1, file);
            // Write y position
            fwrite(&(dungeon->floors[floorIndex]->stairUp[index]->y), sizeof(dungeon->floors[floorIndex]->stairUp[index]->y), 1, file);
        }
    }
    // Write number of downward staircases
    fwrite(&(dungeon->floors[floorIndex]->stairDownCount), sizeof(dungeon->floors[floorIndex]->stairDownCount), 1, file);
    // Write location of downward staircases
    for (index = 0; index < dungeon->floors[floorIndex]->stairDownCount; index++) {
        if (dungeon->floors[floorIndex]->stairDown[index] != NULL) {
            // Write x position
            fwrite(&(dungeon->floors[floorIndex]->stairDown[index]->x), sizeof(dungeon->floors[floorIndex]->stairDown[index]->x), 1, file);
            // Write y position
            fwrite(&(dungeon->floors[floorIndex]->stairDown[index]->y), sizeof(dungeon->floors[floorIndex]->stairDown[index]->y), 1, file);
        }
    }
}

bool dungeon_load_from_file(void* dungeonPointer) {
    Dungeon* dungeon = (Dungeon*) dungeonPointer;

    if (dungeon->settings->doLoad) {
        FILE* file = fopen(dungeon->settings->loadPath, "rb");

        if (file == NULL) {
            printf("Failed to open file at path %s\n", dungeon->settings->savePath);
            return false;
        }

        // Read the file heading. Validate that it is the proper heading
        char fileHeading[13];
        fread(fileHeading, sizeof(fileHeading) - 1, 1, file);
        fileHeading[12] = '\0';
        if (strncmp(fileHeading, FILE_HEADING, 12)) {
            printf("File presented is not a RLG327 save file.\n");
            return false;
        }
        // Read the file version. Validate it against the current version marking
        u_int fileVersion;
        fread(&fileVersion, sizeof(fileVersion), 1, file);
        fileVersion = be32toh(fileVersion);
        if (fileVersion != dungeon->settings->file_version) {
            printf("Invalid file version. Tried version %d against saved version %d Unable to read.\n", fileVersion, dungeon->settings->file_version);
            return false;
        }
        // Read the file size.
        u_int fileSize;
        fread(&fileSize, sizeof(fileSize), 1, file);
        fileSize = be32toh(fileSize);
        // Seek to end of file
        fseek(file, 0, SEEK_END);
        u_int actualFileSize = ftell(file);
        fseek(file, (sizeof(char) * strlen(fileHeading)) + sizeof(fileVersion) + sizeof(fileSize), SEEK_SET);
        if (fileSize != actualFileSize) {
            printf("Invalid file sizes. Actual file size %u bits doesn't match header of %u bits\n", actualFileSize, fileSize);
            return false;
        }

        // Load player location
        u_char playerX;
        u_char playerY;
        fread(&playerX, sizeof(playerX), 1, file);
        fread(&playerY, sizeof(playerY), 1, file);
        u_char playerFloor = 0;
        u_char floorCount = 1;
        // Load current floor of player
        // Load max floors
        if (fileVersion == 1) {
            fread(&playerFloor, sizeof(playerFloor), 1, file);
            fread(&floorCount, sizeof(floorCount), 1, file);
        }

        dungeon->floorCount = floorCount;
        dungeon->currentFloor = playerFloor;
        dungeon->floors = malloc(floorCount * sizeof(Floor));
        dungeon->player = player_initialize(playerX, playerY, dungeon->currentFloor);

        u_char index;
        for (index = 0; index < floorCount; index++) {
            // Load each floor
            load_floor(file, dungeon, index);
        }

        fclose(file);
        file = NULL;

        return true;
    }

    return false;
}

void load_floor(FILE* file, void* dungeonPointer, u_char floorIndex) {
    Dungeon* dungeon = (Dungeon*) dungeonPointer;

    printf("Loaded floor %d\n", floorIndex);

    FloorLoadStructure* floorLoadStructure = malloc(sizeof(FloorLoadStructure));
    floorLoadStructure->width = FLOOR_WIDTH;
    floorLoadStructure->height = FLOOR_HEIGHT;

    u_char index;
    u_char floorPlan[FLOOR_HEIGHT][FLOOR_WIDTH];
    u_char height;
    u_char width;
    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            fread(&(floorPlan[height][width]), sizeof(floorPlan[height][width]), 1, file);
            switch (floorPlan[height][width]) {
                case BORDER_HARDNESS:
                    floorPlan[height][width] = CORNER_WALL_CHARACTER;
                    break;
                case ROCK_HARDNESS:
                    floorPlan[height][width] = ROCK_CHARACTER;
                    break;
                case CORRIDOR_HARDNESS:
                    floorPlan[height][width] = CORRIDOR_CHARACTER;
                    break;
                default:
                    break;
            }
        }
    }

    // Load number of rooms
    u_char numberOfRooms;
    fread(&numberOfRooms, sizeof(numberOfRooms), 1, file);
    floorLoadStructure->roomCount = numberOfRooms;
    // Load room locations
    u_char roomsX[numberOfRooms];
    u_char roomsY[numberOfRooms];
    u_char roomsWidth[numberOfRooms];
    u_char roomsHeight[numberOfRooms];
    for (index = 0; index < numberOfRooms; index++) {
        // Read x position
        fread(&(roomsX[index]), sizeof(roomsX[index]), 1, file);
        // Read y position
        fread(&(roomsY[index]), sizeof(roomsY[index]), 1, file);
        // Read width
        fread(&(roomsWidth[index]), sizeof(roomsWidth[index]), 1, file);
        // Read height
        fread(&(roomsHeight[index]), sizeof(roomsHeight[index]), 1, file);

        for (height = roomsY[index]; height < roomsY[index] + roomsHeight[index]; height++) {
            for (width = roomsX[index]; width < roomsX[index] + roomsWidth[index]; width++) {
                floorPlan[height][width] = ROOM_CHARACTER;
            }
        }
    }
    if (floorIndex == dungeon->currentFloor) {
        floorPlan[dungeon->player->y][dungeon->player->x] = PLAYER_CHARACTER;
    }
    floorLoadStructure->roomsX = roomsX;
    floorLoadStructure->roomsY = roomsY;
    floorLoadStructure->roomsWidth = roomsWidth;
    floorLoadStructure->roomsHeight = roomsHeight;

    // Read number of upward staircases
    u_char stairUpCount;
    fread(&(stairUpCount), sizeof(stairUpCount), 1, file);
    floorLoadStructure->stairUpCount = stairUpCount;
    // Read location of upward staircases
    u_char stairUpX[stairUpCount];
    u_char stairUpY[stairUpCount];
    for (index = 0; index < stairUpCount; index++) {
        // Read x position
        fread(&(stairUpX[index]), sizeof(stairUpX[index]), 1, file);
        // Read y position
        fread(&(stairUpY[index]), sizeof(stairUpY[index]), 1, file);

        floorPlan[stairUpY[index]][stairUpX[index]] = STAIRCASE_UP_CHARACTER;
    }
    floorLoadStructure->stairUpX = stairUpX;
    floorLoadStructure->stairUpY = stairUpY;

    // Read number of downward staircases
    u_char stairDownCount;
    fread(&(stairDownCount), sizeof(stairDownCount), 1, file);
    floorLoadStructure->stairDownCount = stairDownCount;
    // Read location of downward staircases
    u_char stairDownX[stairDownCount];
    u_char stairDownY[stairDownCount];
    for (index = 0; index < stairDownCount; index++) {
        // Read x position
        fread(&(stairDownX[index]), sizeof(stairDownX[index]), 1, file);
        // Read y position
        fread(&(stairDownY[index]), sizeof(stairDownY[index]), 1, file);

        floorPlan[stairDownY[index]][stairDownX[index]] = STAIRCASE_DOWN_CHARACTER;
    }
    floorLoadStructure->stairDownX = stairDownX;
    floorLoadStructure->stairDownY = stairDownY;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            floorLoadStructure->floorPlan[height][width] = floorPlan[height][width];
        }
    }

    dungeon->floors[floorIndex] = floor_load_initialize(floorLoadStructure);

    free(floorLoadStructure);
}