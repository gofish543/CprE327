#include "file_version_0.h"

int file_save_0(Dungeon* dungeon) {
    // Move current file to a tmp file in case save fails
    std::string tempName = dungeon->getSettings()->getSavePath() + ".tmp";
    std::rename(dungeon->getSettings()->getSavePath().c_str(), tempName.c_str());

    // Open the file
    FILE* file = std::fopen(dungeon->getSettings()->getSavePath().c_str(), "wb+");

    if (file == null) {
        printf("Failed to open file at path %s\n", dungeon->getSettings()->getSavePath().c_str());
        return 1;
    }

    char fileHeading[] = FILE_HEADING;
    u_int fileVersion = dungeon->getSettings()->getFileVersion();
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
    u_char x = dungeon->getPlayer()->getX();
    u_char y = dungeon->getPlayer()->getY();
    if (error_check_fwrite(&(x), sizeof(x), 1, file)) {
        return 1;
    }
    if (error_check_fwrite(&(y), sizeof(y), 1, file)) {
        return 1;
    }
    // Only save 1 floor and be done
    if (file_save_floor_0(file, dungeon->getCurrentFloor())) {
        return 1;
    }

    fflush(file);
    // Seek to end of file
    fseek(file, 0, SEEK_END);
    // Get current file pointer
    fileSize = htobe32(uint32_t(ftell(file)));
    // Seek back to file size data index
    fseek(file, (sizeof(char) * strlen(fileHeading)) + sizeof(fileVersion), SEEK_SET);
    if (error_check_fwrite(&fileSize, sizeof(fileSize), 1, file)) {
        return 1;
    }

    fclose(file);

    tempName = dungeon->getSettings()->getSavePath() + ".tmp";
    std::remove(tempName.c_str());

    return 0;
}

int file_save_floor_0(FILE* file, Floor* floor) {
    u_int index;

    // Write dungeon floor plan
    u_char height;
    u_char width;
    u_char hardness;
    u_char x;
    u_char y;
    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            hardness = floor->getTerrainAt(width, height)->getHardness();
            if (error_check_fwrite(&(hardness), sizeof(hardness), 1, file)) {
                return 1;
            }
        }
    }
    // Write number of rooms
    u_short roomCount = htobe16(floor->getRoomCount());
    if (error_check_fwrite(&roomCount, sizeof(roomCount), 1, file)) {
        return 1;
    }
    // Write rooms
    for (index = 0; index < floor->getRoomCount(); index++) {
        // Write x position
        x = floor->getRoom(u_short(index))->getStartingX();
        y = floor->getRoom(u_short(index))->getStartingY();
        height = floor->getRoom(u_short(index))->getHeight();
        width = floor->getRoom(u_short(index))->getWidth();

        if (error_check_fwrite(&(x), sizeof(x), 1, file)) {
            return 1;
        }
        // Write y position
        if (error_check_fwrite(&(y), sizeof(y), 1, file)) {
            return 1;
        }
        // Write width
        if (error_check_fwrite(&(width), sizeof(width), 1, file)) {
            return 1;
        }
        // Write height
        if (error_check_fwrite(&(height), sizeof(height), 1, file)) {
            return 1;
        }
    }
    // File version zero has to write zero stairs to anywhere, its just a floor
    // Write number of upward staircases
    u_short stairUpCount = htobe16(0);
    if (error_check_fwrite(&stairUpCount, sizeof(stairUpCount), 1, file)) {
        return 1;
    }
    // Write location of upward staircases
    for (index = 0; index < 0; index++) {
        x = floor->getUpStair(u_short(index))->getX();
        y = floor->getUpStair(u_short(index))->getY();
        // Write x position
        if (error_check_fwrite(&(x), sizeof(x), 1, file)) {
            return 1;
        }
        // Write y position
        if (error_check_fwrite(&(y), sizeof(y), 1, file)) {
            return 1;
        }
    }
    // Write number of downward staircases
    u_short stairDownCount = htobe16(0);
    if (error_check_fwrite(&stairDownCount, sizeof(stairDownCount), 1, file)) {
        return 1;
    }
    // Write location of downward staircases
    for (index = 0; index < 0; index++) {
        x = floor->getDownStair(u_short(index))->getX();
        y = floor->getDownStair(u_short(index))->getY();
        // Write x position
        if (error_check_fwrite(&(x), sizeof(x), 1, file)) {
            return 1;
        }
        // Write y position
        if (error_check_fwrite(&(y), sizeof(y), 1, file)) {
            return 1;
        }
    }

    return 0;
}

int file_load_0(Dungeon* dungeon) {
    FILE* file = std::fopen(dungeon->getSettings()->getLoadPath().c_str(), "rb");

    if (file == null) {
        printf("Failed to open file at path %s\n", dungeon->getSettings()->getLoadPath().c_str());
        return 1;
    }

    // Read the file heading. Validate that it is the proper heading
    char fileHeading[13];
    if (error_check_fread(fileHeading, sizeof(fileHeading) - 1, 1, file)) {
        return 1;
    }
    fileHeading[12] = '\0';
    if (strncmp(fileHeading, FILE_HEADING, 12) != 0) {
        printf("File presented is not a RLG327 save file.\n");
        return 1;
    }
    // Read the file version. Validate it against the current version marking
    u_int fileVersion;
    if (error_check_fread(&fileVersion, sizeof(fileVersion), 1, file)) {
        return 1;
    }
    fileVersion = be32toh(fileVersion);
    if (fileVersion != dungeon->getSettings()->getFileVersion()) {
        printf("Invalid file version. Tried version %d against saved version %d Unable to read.\n", fileVersion, dungeon->getSettings()->getFileVersion());
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
    auto actualFileSize = u_int(ftell(file));
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
    auto floor = new Floor(dungeon);
    dungeon->setFloorCount(1)->addFloor(floor)->setCurrentFloor(floor)->setPlayer(new Player(floor, playerX, playerY));

    floor->setFloorNumber(0)->setCharacterAt(dungeon->getPlayer(), playerX, playerY);

    if (file_load_floor_0(file, floor)) {
        return 1;
    }

    fclose(file);

    debug_terminate();
    return 0;
}

int file_load_floor_0(FILE* file, Floor* floor) {
    u_char hardness;

    u_char height;
    u_char width;
    u_char index;

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            if (error_check_fread(&(hardness), sizeof(hardness), 1, file)) {
                return 1;
            }

            if (floor->getTerrainAt(width, height) == null) {
                if (hardness == CORRIDOR_HARDNESS) {
                    floor->setTerrainAt(new Corridor(floor, 0, width, height), width, height);
                } else {
                    floor->setTerrainAt(new Rock(floor, 0, width, height, hardness), width, height);
                }
            }
        }
    }

    // Load number of rooms
    u_short numberOfRooms;
    if (error_check_fread(&numberOfRooms, sizeof(numberOfRooms), 1, file)) {
        return 1;
    }
    floor->setRoomCount(be16toh(numberOfRooms));
    // Load room locations
    u_char startX;
    u_char startY;
    u_char x;
    u_char y;
    for (index = 0; index < floor->getRoomCount(); index++) {
        // Read x position
        if (error_check_fread(&(startX), sizeof(startX), 1, file)) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(startY), sizeof(startY), 1, file)) {
            return 1;
        }
        // Read width
        if (error_check_fread(&(width), sizeof(width), 1, file)) {
            return 1;
        }
        // Read height
        if (error_check_fread(&(height), sizeof(height), 1, file)) {
            return 1;
        }

        floor->setRoom(new Room(floor, index, startX, startY, startX, startY, width, height));

        for (y = startY; y < startY + height; y++) {
            for (x = startX; x < startX + width; x++) {
                delete (floor->getTerrainAt(x, y));
                floor->setTerrainAt(new Room(floor, index, x, y, startX, startY, width, height), x, y);
            }
        }
    }

    // Read number of upward staircases
    u_short stairUpCount;
    if (error_check_fread(&stairUpCount, sizeof(stairUpCount), 1, file)) {
        return 1;
    }
    floor->setStairUpCount(be16toh(stairUpCount));
    // Read location of upward staircases
    for (index = 0; index < floor->getStairUpCount(); index++) {
        // Read x position
        if (error_check_fread(&(x), sizeof(x), 1, file)) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(y), sizeof(y), 1, file)) {
            return 1;
        }

        floor->setUpStair(new Staircase(floor, index, x, y, STAIRCASE_TYPE_UP));
        delete (floor->getTerrainAt(x, y));
        floor->setTerrainAt(new Staircase(floor, index, x, y, STAIRCASE_TYPE_UP), x, y);
    }

    // Read number of downward staircases
    u_short stairDownCount;
    if (error_check_fread(&stairDownCount, sizeof(stairDownCount), 1, file)) {
        return 1;
    }
    floor->setStairDownCount(be16toh(stairDownCount));
    // Read location of downward staircases
    for (index = 0; index < floor->getStairDownCount(); index++) {
        // Read x position
        if (error_check_fread(&(x), sizeof(x), 1, file)) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(y), sizeof(y), 1, file)) {
            return 1;
        }

        floor->setDownStair(new Staircase(floor, index, x, y, STAIRCASE_TYPE_DOWN));
        delete (floor->getTerrainAt(x, y));
        floor->setTerrainAt(new Staircase(floor, index, x, y, STAIRCASE_TYPE_DOWN), x, y);
    }

    if (floor->getDungeon()->getSettings()->doNumberOfMonsters()) {
        floor->setMonsterCount(floor->getDungeon()->getSettings()->getNumberOfMonsters());
    } else {
        floor->setMonsterCount(u_short(Dice::RandomNumberBetween(FLOOR_MONSTERS_MIN, FLOOR_MONSTERS_MAX)));
    }

    if (floor->generateMonsters()) {
        return 1;
    }
    return 0;
}