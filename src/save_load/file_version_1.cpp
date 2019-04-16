#include "file_version_1.h"

int file_save_1(Dungeon* dungeon) {
    // Move current file to a tmp file in case save fails
    std::string tempName = *dungeon->getSettings()->getSavePath() + ".tmp";
    std::rename(dungeon->getSettings()->getSavePath()->c_str(), tempName.c_str());

    // Open the file
    FILE* file = std::fopen(dungeon->getSettings()->getSavePath()->c_str(), "wb+");

    if (file == null) {
        printf("Failed to open file at path %s\n", dungeon->getSettings()->getSavePath()->c_str());
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
    // Write player level
    u_int playerLevel_be32 = htobe32(dungeon->getPlayer()->getLevel());
    if (error_check_fwrite(&(playerLevel_be32), sizeof(playerLevel_be32), 1, file)) {
        return 1;
    }
    // Write player monstersSlain
    u_int playerMonstersSlain_be32 = htobe32(dungeon->getPlayer()->getMonstersSlain());
    if (error_check_fwrite(&(playerMonstersSlain_be32), sizeof(playerMonstersSlain_be32), 1, file)) {
        return 1;
    }
    // Write player daysSurvived
    u_int playerDaysSurvived_be32 = htobe32(dungeon->getPlayer()->getDaysSurvived());
    if (error_check_fwrite(&(playerDaysSurvived_be32), sizeof(playerDaysSurvived_be32), 1, file)) {
        return 1;
    }
    // Write player health
    u_int playerHealth_be32 = htobe32(dungeon->getPlayer()->getHealth());
    if (error_check_fwrite(&(playerHealth_be32), sizeof(playerHealth_be32), 1, file)) {
        return 1;
    }
    // Write current floor of player
    u_char currentFloorNumber = dungeon->getCurrentFloor()->getFloorNumber();
    if (error_check_fwrite(&(currentFloorNumber), sizeof(currentFloorNumber), 1, file)) {
        return 1;
    }
    // Write max floors
    u_char maxFloors = dungeon->getFloorCount();
    if (error_check_fwrite(&(maxFloors), sizeof(maxFloors), 1, file)) {
        return 1;
    }

    u_char index;
    for (index = 0; index < dungeon->getFloorCount(); index++) {
        // Save each floor
        if (file_save_floor_1(file, dungeon->getFloor(index))) {
            return 1;
        }
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

    tempName = *dungeon->getSettings()->getSavePath() + ".tmp";
    std::remove(tempName.c_str());

    return 0;
}

int file_save_floor_1(FILE* file, Floor* floor) {
    u_int index;

    // Write dungeon floor plan
    u_char height;
    u_char width;
    u_char hardness;
    u_char x;
    u_char y;
    u_char lastSpottedX;
    u_char lastSpottedY;
    u_char speed;
    u_int abilities;
    std::string
    u_short color;

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
        x = floor->getRoom(index)->getStartingX();
        y = floor->getRoom(index)->getStartingY();
        height = floor->getRoom(index)->getHeight();
        width = floor->getRoom(index)->getWidth();

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
    // Write number of upward staircases
    u_short stairUpCount = htobe16(floor->getStairUpCount());
    if (error_check_fwrite(&stairUpCount, sizeof(stairUpCount), 1, file)) {
        return 1;
    }
    // Write location of upward staircases
    for (index = 0; index < floor->getStairUpCount(); index++) {
        x = floor->getUpStair(index)->getX();
        y = floor->getUpStair(index)->getY();
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
    u_short stairDownCount = htobe16(floor->getStairDownCount());
    if (error_check_fwrite(&stairDownCount, sizeof(stairDownCount), 1, file)) {
        return 1;
    }
    // Write location of downward staircases
    for (index = 0; index < floor->getStairDownCount(); index++) {
        x = floor->getDownStair(index)->getX();
        y = floor->getDownStair(index)->getY();
        // Write x position
        if (error_check_fwrite(&(x), sizeof(x), 1, file)) {
            return 1;
        }
        // Write y position
        if (error_check_fwrite(&(y), sizeof(y), 1, file)) {
            return 1;
        }
    }

    // Write number of monsters
    u_short monsterCount = htobe16(floor->getMonsterCount());
    if (error_check_fwrite(&monsterCount, sizeof(monsterCount), 1, file)) {
        return 1;
    }
    // Write location of monsters
    for (index = 0; index < floor->getMonsterCount(); index++) {
        x = floor->getMonster(index)->getX();
        y = floor->getMonster(index)->getY();
        classification = floor->getMonster(index)->getAbility();
        speed = floor->getMonster(index)->getSpeed();
        lastSpottedX = floor->getMonster(index)->getPlayerLastSpottedX();
        lastSpottedY = floor->getMonster(index)->getPlayerLastSpottedY();
        // Write x Position
        if (error_check_fwrite(&(x), sizeof(x), 1, file)) {
            return 1;
        }
        // Write y Position
        if (error_check_fwrite(&(y), sizeof(y), 1, file)) {
            return 1;
        }
        // Write classification value
        if (error_check_fwrite(&(classification), sizeof(classification), 1, file)) {
            return 1;
        }
        // Write speed
        if (error_check_fwrite(&(speed), sizeof(speed), 1, file)) {
            return 1;
        }
        // Write last spotted playerX
        if (error_check_fwrite(&(lastSpottedX), sizeof(lastSpottedX), 1, file)) {
            return 1;
        }
        // Write last spotted playerY
        if (error_check_fwrite(&(lastSpottedY), sizeof(lastSpottedY), 1, file)) {
            return 1;
        }
    }

    return 0;
}

int file_load_1(Dungeon* dungeon) {
    FILE* file = std::fopen(dungeon->getSettings()->getLoadPath()->c_str(), "rb");

    if (file == null) {
        printf("Failed to open file at path %s\n", dungeon->getSettings()->getLoadPath()->c_str());
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
    u_int level;
    u_int monstersSlain;
    u_int daysSurvived;
    if (error_check_fread(&level, sizeof(level), 1, file)) {
        return 1;
    }
    if (error_check_fread(&monstersSlain, sizeof(monstersSlain), 1, file)) {
        return 1;
    }
    if (error_check_fread(&daysSurvived, sizeof(daysSurvived), 1, file)) {
        return 1;
    }
    level = be32toh(level);
    monstersSlain = be32toh(monstersSlain);
    daysSurvived = be32toh(daysSurvived);

    u_char playerFloor;
    u_char floorCount;
    // Load current floor of player
    // Load max floors
    if (error_check_fread(&playerFloor, sizeof(playerFloor), 1, file)) {
        return 1;
    }
    if (error_check_fread(&floorCount, sizeof(floorCount), 1, file)) {
        return 1;
    }

    u_char index;
    for (index = 0; index < floorCount; index++) {
        auto floor = new Floor(dungeon);
        floor->setFloorNumber(index);

        if (file_load_floor_1(file, floor)) {
            return 1;
        }

        dungeon->addFloor(floor);
    }

    dungeon->setCurrentFloor(dungeon->getFloor(playerFloor));

    dungeon->setPlayer(new Player(dungeon->getCurrentFloor(), playerX, playerY, level, monstersSlain, daysSurvived, 100000));

    dungeon->getCurrentFloor()->setCharacterAt(dungeon->getPlayer(), playerX, playerY);

    fclose(file);

    return 0;
}

int file_load_floor_1(FILE* file, Floor* floor) {
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
                    floor->setTerrainAt(new Corridor(floor, width, height), width, height);
                } else {
                    floor->setTerrainAt(new Rock(floor, width, height, hardness), width, height);
                }
            }
        }
    }


    // Load number of rooms
    u_short numberOfRooms;
    if (error_check_fread(&numberOfRooms, sizeof(numberOfRooms), 1, file)) {
        return 1;
    }
    numberOfRooms = be16toh(numberOfRooms);
    // Load room locations
    u_char startX;
    u_char startY;
    u_char x;
    u_char y;
    Room* room;
    for (index = 0; index < numberOfRooms; index++) {
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

        room = new Room(floor, startX, startY, width, height);

        floor->setRoom(room);
        for (y = startY; y < startY + height; y++) {
            for (x = startX; x < startX + width; x++) {
                delete (floor->getTerrainAt(x, y));
                floor->setTerrainAt(room, x, y);
            }
        }
    }

    Staircase* staircase;
    // Read number of upward staircases
    u_short stairUpCount;
    if (error_check_fread(&stairUpCount, sizeof(stairUpCount), 1, file)) {
        return 1;
    }
    stairUpCount = be16toh(stairUpCount);
    // Read location of upward staircases
    for (index = 0; index < stairUpCount; index++) {
        // Read x position
        if (error_check_fread(&(x), sizeof(x), 1, file)) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(y), sizeof(y), 1, file)) {
            return 1;
        }

        staircase = new Staircase(floor, x, y, index, STAIRCASE_DIRECTION_UP);
        delete (floor->getTerrainAt(x, y));
        floor->setUpStair(staircase);
        floor->setTerrainAt(staircase, x, y);
    }

    // Read number of downward staircases
    u_short stairDownCount;
    if (error_check_fread(&stairDownCount, sizeof(stairDownCount), 1, file)) {
        return 1;
    }
    stairDownCount = be16toh(stairDownCount);
    // Read location of downward staircases
    for (index = 0; index < stairDownCount; index++) {
        // Read x position
        if (error_check_fread(&(x), sizeof(x), 1, file)) {
            return 1;
        }
        // Read y position
        if (error_check_fread(&(y), sizeof(y), 1, file)) {
            return 1;
        }

        staircase = new Staircase(floor, x, y, index, STAIRCASE_DIRECTION_DOWN);
        delete (floor->getTerrainAt(x, y));
        floor->setDownStair(staircase);
        floor->setTerrainAt(staircase, x, y);
    }

    // Read number of monsters
    u_short monsterCount;
    if (error_check_fread(&monsterCount, sizeof(monsterCount), 1, file)) {
        return 1;
    }
    monsterCount = be16toh(monsterCount);
    u_char tempX;
    u_char tempY;
    u_char tempType;
    u_char tempSpeed;
    u_char playerLastSpottedX;
    u_char playerLastSpottedY;
    Monster* monster;
    // Read location of monsters
    for (index = 0; index < monsterCount; index++) {
        // Read x Position
        if (error_check_fread(&(tempX), sizeof(tempX), 1, file)) {
            return 1;
        }
        // Read y Position
        if (error_check_fread(&(tempY), sizeof(tempY), 1, file)) {
            return 1;
        }
        // Read classification value
        if (error_check_fread(&(tempType), sizeof(tempType), 1, file)) {
            return 1;
        }
        // Read speed
        if (error_check_fread(&(tempSpeed), sizeof(tempSpeed), 1, file)) {
            return 1;
        }
        std::string str = "Hello World";
        monster = new Monster(floor, tempX, tempY, &str, &str, 0, 0, 0, 0, new Dice(0, 0, 0), '0', 0);

        // Read last spotted playerX
        if (error_check_fread(&(playerLastSpottedX), sizeof(playerLastSpottedX), 1, file)) {
            return 1;
        }
        // Read last spotted playerY
        if (error_check_fread(&(playerLastSpottedY), sizeof(playerLastSpottedY), 1, file)) {
            return 1;
        }
        floor->setMonster(monster);
        monster->setPlayerLastSpottedX(playerLastSpottedX)->setPlayerLastSpottedY(playerLastSpottedY);
        floor->setCharacterAt(monster, tempX, tempY);
    }

    return 0;
}