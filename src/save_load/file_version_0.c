#include "file_version_0.h"

int file_save_0(Dungeon* dungeon) {
    if (dungeon->settings->doSave) {
        // Move current file to a tmp file in case save fails
        char* tempName = malloc((strlen(dungeon->settings->savePath) * sizeof(char)) + (strlen(".tmp") * sizeof(char)) + 1);
        sprintf(tempName, "%s%s", dungeon->settings->savePath, ".tmp");
        rename(dungeon->settings->savePath, tempName);
        free(tempName);

        // Open the file
        FILE* file = fopen(dungeon->settings->savePath, "wb+");

        if (file == null) {
            printf("Failed to open file at path %s\n", dungeon->settings->savePath);
            return 1;
        }

        char fileHeading[] = FILE_HEADING;
        u_int fileVersion = dungeon->settings->file_version;
        u_int fileVersion_be32 = htobe32(fileVersion);
        u_int fileSize = 0;

        // Write file heading
        if (error_check_fwrite(fileHeading, sizeof(char), strlen(fileHeading), file)) {
            return 1;
        }
        // Write file version
        if (error_check_fwrite(&(fileVersion_be32), sizeof(fileVersion), 1, file)) {
            return 1;
        }
        // Write file size, this will be over written later
        if (error_check_fwrite(&fileSize, sizeof(fileSize), 1, file)) {
            return 1;
        }
        // Write player location
        if (error_check_fwrite(&(dungeon->player->character->x), sizeof(dungeon->player->character->x), 1, file)) {
            return 1;
        }
        if (error_check_fwrite(&(dungeon->player->character->y), sizeof(dungeon->player->character->y), 1, file)) {
            return 1;
        }
        // Only save 1 floor and be done
        if (file_save_floor_0(file, dungeon, dungeon->floor)) {
            return 1;
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
        file = null;

        tempName = malloc((strlen(dungeon->settings->savePath) * sizeof(char)) + (strlen(".tmp") * sizeof(char)) + 1);
        sprintf(tempName, "%s%s", dungeon->settings->savePath, ".tmp");
        remove(tempName);
        free(tempName);
    }

    return 0;
}

int file_save_floor_0(FILE* file, Dungeon* dungeon, Floor* floor) {
    u_int index;

    // Write dungeon floor plan
    u_char height;
    u_char width;
    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            if (error_check_fwrite(&(floor->terrains[height][width]->hardness), sizeof(u_char), 1, file)) {
                return 1;
            }
        }
    }
    // Write number of rooms
    u_short roomCount = htobe16(floor->roomCount);
    if (error_check_fwrite(&roomCount, sizeof(roomCount), 1, file)) {
        return 1;
    }
    // Write rooms
    for (index = 0; index < floor->roomCount; index++) {
        // Write x position
        if (error_check_fwrite(&(floor->rooms[index]->startX), sizeof(floor->rooms[index]->startX), 1, file)) {
            return 1;
        }
        // Write y position
        if (error_check_fwrite(&(floor->rooms[index]->startY), sizeof(floor->rooms[index]->startY), 1, file)) {
            return 1;
        }
        // Write width
        if (error_check_fwrite(&(floor->rooms[index]->width), sizeof(floor->rooms[index]->width), 1, file)) {
            return 1;
        }
        // Write height
        if (error_check_fwrite(&(floor->rooms[index]->height), sizeof(floor->rooms[index]->height), 1, file)) {
            return 1;
        }
    }
    // Write number of upward staircases
    u_short stairUpCount = htobe16(floor->stairUpCount);
    if (error_check_fwrite(&stairUpCount, sizeof(stairUpCount), 1, file)) {
        return 1;
    }
    // Write location of upward staircases
    for (index = 0; index < floor->stairUpCount; index++) {
        if (floor->upStairs[index] != null) {
            // Write x position
            if (error_check_fwrite(&(floor->upStairs[index]->x), sizeof(floor->upStairs[index]->x), 1, file)) {
                return 1;
            }
            // Write y position
            if (error_check_fwrite(&(floor->upStairs[index]->y), sizeof(floor->upStairs[index]->y), 1, file)) {
                return 1;
            }
        }
    }
    // Write number of downward staircases
    u_short stairDownCount = htobe16(floor->stairDownCount);
    if (error_check_fwrite(&stairDownCount, sizeof(stairDownCount), 1, file)) {
        return 1;
    }
    // Write location of downward staircases
    for (index = 0; index < floor->stairDownCount; index++) {
        if (floor->downStairs[index] != null) {
            // Write x position
            if (error_check_fwrite(&(floor->downStairs[index]->x), sizeof(floor->downStairs[index]->x), 1, file)) {
                return 1;
            }
            // Write y position
            if (error_check_fwrite(&(floor->downStairs[index]->y), sizeof(floor->downStairs[index]->y), 1, file)) {
                return 1;
            }
        }
    }

    return 0;
}

int file_load_0(Dungeon* dungeon) {
    if (dungeon->settings->doLoad) {
        FILE* file = fopen(dungeon->settings->loadPath, "rb");

        if (file == null) {
            printf("Failed to open file at path %s\n", dungeon->settings->savePath);
            return 1;
        }

        // Read the file heading. Validate that it is the proper heading
        char fileHeading[13];
        if (error_check_fread(fileHeading, sizeof(fileHeading) - 1, 1, file)) {
            return 1;
        }
        fileHeading[12] = '\0';
        if (strncmp(fileHeading, FILE_HEADING, 12)) {
            printf("File presented is not a RLG327 save file.\n");
            return 1;
        }
        // Read the file version. Validate it against the current version marking
        u_int fileVersion;
        if (error_check_fread(&fileVersion, sizeof(fileVersion), 1, file)) {
            return 1;
        }
        fileVersion = be32toh(fileVersion);
        if (fileVersion != dungeon->settings->file_version) {
            printf("Invalid file version. Tried version %d against saved version %d Unable to read.\n", fileVersion, dungeon->settings->file_version);
            return 1;
        }
        // Read the file size.
        u_int fileSize;
        if (error_check_fread(&fileSize, sizeof(fileSize), 1, file)) {
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
        if (error_check_fread(&playerX, sizeof(playerX), 1, file)) {
            return 1;
        }
        if (error_check_fread(&playerY, sizeof(playerY), 1, file)) {
            return 1;
        }
        dungeon->floorCount = 1;
        dungeon->floors = malloc(dungeon->floorCount * sizeof(Floor*));
        dungeon->floors[0] = malloc(sizeof(Floor));
        dungeon->floor = dungeon->floors[0];
        dungeon->floor->dungeon = dungeon;

        dungeon->floor->floorNumber = 0;
        floor_generate_empty_characters(dungeon->floor);
        floor_generate_empty_terrains(dungeon->floor);
        floor_generate_borders(dungeon->floor);
        dungeon->player = player_initialize(dungeon->floor, playerX, playerY);
        dungeon->floor->characters[playerY][playerX] = dungeon->player->character;

        if (file_load_floor_0(file, dungeon, dungeon->floor)) {
            return 1;
        }

        fclose(file);
        file = null;
    }

    return 0;
}

int file_load_floor_0(FILE* file, Dungeon* dungeon, Floor* floor) {
    u_char height;
    u_char width;
    u_char index;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            if (error_check_fread(&(floor->terrains[height][width]->hardness), sizeof(floor->terrains[height][width]->hardness), 1, file)) {
                return 1;
            }

            if(!floor->terrains[height][width]->isImmutable) {
                switch (floor->terrains[height][width]->hardness) {
                    case CORRIDOR_HARDNESS:
                        floor->terrains[height][width]->hardness = CORRIDOR_HARDNESS;
                        floor->terrains[height][width]->character = CORRIDOR_CHARACTER;
                        floor->terrains[height][width]->isImmutable = false;
                        floor->terrains[height][width]->isWalkable = true;
                        floor->terrains[height][width]->isRock = false;
                        break;
                }
            }
        }
    }

    // Load number of rooms
    u_short numberOfRooms;
    if (error_check_fread(&numberOfRooms, sizeof(numberOfRooms), 1, file)) {
        return 1;
    }
    floor->roomCount = be16toh(numberOfRooms);
    floor->rooms = malloc(floor->roomCount * sizeof(Room*));
    // Load room locations
    for (index = 0; index < floor->roomCount; index++) {
        floor->rooms[index] = malloc(sizeof(Room));
        // Read x position
        if (error_check_fread(&(floor->rooms[index]->startX), sizeof(floor->rooms[index]->startX), 1, file)) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(floor->rooms[index]->startY), sizeof(floor->rooms[index]->startY), 1, file)) {
            return 1;
        }
        // Read width
        if (error_check_fread(&(floor->rooms[index]->width), sizeof(floor->rooms[index]->width), 1, file)) {
            return 1;
        }
        // Read height
        if (error_check_fread(&(floor->rooms[index]->height), sizeof(floor->rooms[index]->height), 1, file)) {
            return 1;
        }

        for (height = floor->rooms[index]->startY; height < floor->rooms[index]->startY + floor->rooms[index]->height; height++) {
            for (width = floor->rooms[index]->startX; width < floor->rooms[index]->startX + floor->rooms[index]->width; width++) {
                floor->terrains[height][width]->room = floor->rooms[index];

                floor->terrains[height][width]->character = ROOM_CHARACTER;
                floor->terrains[height][width]->hardness = ROOM_HARDNESS;

                floor->terrains[height][width]->isWalkable = true;
                floor->terrains[height][width]->isRock = false;
                floor->terrains[height][width]->isImmutable = false;
            }
        }
    }

    // Read number of upward staircases
    u_short stairUpCount;
    if (error_check_fread(&stairUpCount, sizeof(stairUpCount), 1, file)) {
        return 1;
    }
    floor->stairUpCount = be16toh(stairUpCount);
    floor->upStairs = malloc(floor->stairUpCount * sizeof(Staircase*));
    // Read location of upward staircases
    for (index = 0; index < floor->stairUpCount; index++) {
        floor->upStairs[index] = malloc(sizeof(Staircase));
        // Read x position
        if (error_check_fread(&(floor->upStairs[index]->x), sizeof(floor->upStairs[index]->x), 1, file)) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(floor->upStairs[index]->y), sizeof(floor->upStairs[index]->y), 1, file)) {
            return 1;
        }

        floor->terrains[floor->upStairs[index]->y][floor->upStairs[index]->x]->staircase = floor->upStairs[index];
        floor->terrains[floor->upStairs[index]->y][floor->upStairs[index]->x]->hardness = STAIRCASE_HARDNESS;
        floor->terrains[floor->upStairs[index]->y][floor->upStairs[index]->x]->character = STAIRCASE_UP_CHARACTER;
    }

    // Read number of downward staircases
    u_short stairDownCount;
    if (error_check_fread(&stairDownCount, sizeof(stairDownCount), 1, file)) {
        return 1;
    }
    floor->stairDownCount = be16toh(stairDownCount);
    floor->downStairs = malloc(floor->stairDownCount * sizeof(Staircase*));
    // Read location of downward staircases
    for (index = 0; index < floor->stairDownCount; index++) {
        floor->downStairs[index] = malloc(sizeof(Staircase));
        // Read x position
        if (error_check_fread(&(floor->downStairs[index]->x), sizeof(floor->downStairs[index]->x), 1, file)) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(floor->downStairs[index]->y), sizeof(floor->downStairs[index]->y), 1, file)) {
            return 1;
        }
        floor->terrains[floor->downStairs[index]->y][floor->downStairs[index]->x]->staircase = floor->downStairs[index];
        floor->terrains[floor->downStairs[index]->y][floor->downStairs[index]->x]->hardness = STAIRCASE_HARDNESS;
        floor->terrains[floor->downStairs[index]->y][floor->downStairs[index]->x]->character = STAIRCASE_DOWN_CHARACTER;
    }

    if (floor->dungeon->settings->doNumberOfMonsters) {
        floor->monsterCount = floor->dungeon->settings->numberOfMonsters;
    } else {
        floor->monsterCount = random_number_between(FLOOR_MONSTERS_MIN, FLOOR_MONSTERS_MAX);
    }
    floor->monsters = malloc(floor->monsterCount * sizeof(Monster*));

    if (floor_generate_monsters(floor)) {
        return 1;
    }
    return 0;
}