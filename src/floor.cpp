#include <exception.h>
#include "floor.h"

Floor::Floor(Dungeon* dungeon)
        : tunnelerView{0}, nonTunnelerView{0}, cheapestPathToPlayer{0}, terrains{null}, characters{null}, objectsMap{null} {
    this->dungeon = dungeon;

    this->floorNumber = 0;

    this->initializeToNull()
            ->generateBorders();
}

Floor::Floor(Dungeon* dungeon, u_char floorNumber, u_short roomCount, u_short stairUpCount, u_short stairDownCount, u_short monsterCount, u_short objectCount)
        : tunnelerView{0}, nonTunnelerView{0}, cheapestPathToPlayer{0}, terrains{null}, characters{null}, objectsMap{null} {
    this->dungeon = dungeon;
    this->floorNumber = floorNumber;

    if (this->floorNumber == 0) {
        stairDownCount = 0;
    }
    if (this->floorNumber == this->dungeon->getFloorCount() - 1) {
        stairUpCount = 0;
    }

    if (this->dungeon->getSettings()->doNumberOfMonsters()) {
        monsterCount = this->dungeon->getSettings()->getNumberOfMonsters();
    }

    this->initializeToNull()
            ->generateBorders()
            ->generateRooms(roomCount, stairUpCount, stairDownCount)
            ->generateCorridors()
            ->generateRock()
            ->generatePlayer()
            ->generateMonsters(monsterCount)
            ->generateObjects(objectCount);
}

Floor::~Floor() {
    u_short index;
    u_char height;
    u_char width;

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            // Rooms need to be deleted special due to pointer management
            if (this->terrains[height][width]->isRoom()) {
                continue;
            }

            delete (this->terrains[height][width]);
        }
    }

    for (index = 0; index < this->getRoomCount(); index++) {
        delete (this->rooms.at(index));
    }

    for (index = 0; index < this->getStairUpCount(); index++) {
        delete (this->upStairs.at(index));
    }

    for (index = 0; index < this->getStairDownCount(); index++) {
        delete (this->downStairs.at(index));
    }

    for (index = 0; index < this->getMonsterCount(); index++) {
        delete (this->monsters.at(index));
    }

    for (index = 0; index < this->getObjectCount(); index++) {
        delete (this->objects.at(index));
    }
}

Floor* Floor::resetTunnelerView() {
    u_char height;
    u_char width;

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            this->tunnelerView[height][width] = U_CHAR_MIN;
        }
    }

    return this;
}

Floor* Floor::resetNonTunnelerView() {
    u_char height;
    u_char width;

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            this->nonTunnelerView[height][width] = U_CHAR_MIN;
        }
    }

    return this;
}

Floor* Floor::resetCheapestPathToPlayer() {
    u_char height;
    u_char width;

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            this->cheapestPathToPlayer[height][width] = U_CHAR_MIN;
        }
    }

    return this;
}

u_char Floor::getOutputCharacterAt(u_char x, u_char y) {
    if (this->dungeon->getSettings()->doFogOfWar()) {
        if (this->dungeon->getPlayer()->visibility[y][x] == null) {
            return ROCK_CHARACTER;
        } else if (this->characters[y][x] != null && this->dungeon->getPlayer()->hasLineOfSightTo(x, y)) {
            return this->characters[y][x]->getCharacter();
        } else if (this->objectsMap[y][x] != null && this->dungeon->getPlayer()->hasLineOfSightTo(x, y)) {
            return this->objectsMap[y][x]->getCharacter();
        } else {
            return this->dungeon->getPlayer()->visibility[y][x]->getCharacter();
        }
    } else if (this->characters[y][x] != null) {
        return this->characters[y][x]->getCharacter();
    } else if (this->objectsMap[y][x] != null) {
        return this->objectsMap[y][x]->getCharacter();
    } else {
        return this->terrains[y][x]->getCharacter();
    }
}

void Floor::getColorAt(u_char x, u_char y, u_int* foreground, u_int* background) {
    *foreground = EFD_COLOR_WHITE;
    *background = EFD_COLOR_GREY_DARK;

    Character* character = this->characters[y][x];
    Object* object = this->objectsMap[y][x];
    Terrain* visibility = this->dungeon->getPlayer()->visibility[y][x];

    // Set the background color
    if (this->dungeon->getSettings()->doFogOfWar() && this->dungeon->getPlayer()->hasLineOfSightTo(x, y) && this->terrains[y][x]->isWalkable()) {
        *background = EFD_COLOR_GREY_LIGHT;
    } else {
        *background = EFD_COLOR_GREY_DARK;
    }

    if (this->dungeon->getPlayer()->getX() == x && this->dungeon->getPlayer()->getY() == y) {
        *foreground = EFD_COLOR_WHITE;
    } else if (this->dungeon->getSettings()->doFogOfWar()) {
        if (visibility == null) {
            *foreground = EFD_COLOR_WHITE;
        } else if (character != null && this->dungeon->getPlayer()->hasLineOfSightTo(x, y)) {
            *foreground = character->getColor();
        } else if (object != null && this->dungeon->getPlayer()->hasLineOfSightTo(x, y)) {
            *foreground = object->getColor();
        } else {
            *foreground = EFD_COLOR_WHITE;
        }
    } else if (character != null && character->isMonster()) {
        *foreground = character->getColor();
    } else if (object != null) {
        *foreground = object->getColor();
    } else {
        *foreground = EFD_COLOR_WHITE;
    }
}

/** GETTERS **/
Dungeon* Floor::getDungeon() {
    return this->dungeon;
}

u_char Floor::getFloorNumber() {
    return this->floorNumber;
}

u_short Floor::getRoomCount() {
    return this->rooms.size();
}

u_short Floor::getStairUpCount() {
    return this->upStairs.size();
}

u_short Floor::getStairDownCount() {
    return this->downStairs.size();
}

u_short Floor::getMonsterCount() {
    return this->monsters.size();
}

u_short Floor::getObjectCount() {
    return this->objects.size();
}

Terrain* Floor::getTerrainAt(u_char width, u_char height) {
    return this->terrains[height][width];
}

Character* Floor::getCharacterAt(u_char width, u_char height) {
    return this->characters[height][width];
}

Object* Floor::getObjectAt(u_char width, u_char height) {
    return this->objectsMap[height][width];
}

u_char Floor::getTunnelerViewAt(u_char width, u_char height) {
    return this->tunnelerView[height][width];
}

u_char Floor::getNonTunnelerViewAt(u_char width, u_char height) {
    return this->nonTunnelerView[height][width];
}

u_char Floor::getCheapestPathToPlayerAt(u_char width, u_char height) {
    return this->cheapestPathToPlayer[height][width];
}

Monster* Floor::getMonster(u_short index) {
    if (index < this->getMonsterCount()) {
        return this->monsters[index];
    } else {
        throw Exception::MonsterOutOfBounds();
    }
}

Staircase* Floor::getUpStair(u_short index) {
    if (index < this->getStairUpCount()) {
        return this->upStairs[index];
    } else {
        throw Exception::StaircaseOutOfBounds();
    }
}

Staircase* Floor::getDownStair(u_short index) {
    if (index < this->getStairDownCount()) {
        return this->downStairs[index];
    } else {
        throw Exception::StaircaseOutOfBounds();
    }
}

Room* Floor::getRoom(u_short index) {
    if (index < this->getRoomCount()) {
        return this->rooms[index];
    } else {
        throw Exception::RoomOutOfBounds();
    }
}
/** GETTERS **/

/** SETTERS **/
Floor* Floor::setFloorNumber(u_char floorNumber) {
    this->floorNumber = floorNumber;

    return this;
}

Floor* Floor::setTerrainAt(Terrain* terrain, u_char width, u_char height) {
    this->terrains[height][width] = terrain;

    return this;
}

Floor* Floor::setCharacterAt(Character* character, u_char width, u_char height) {
    this->characters[height][width] = character;

    return this;
}

Floor* Floor::setTunnelerViewAt(u_char value, u_char width, u_char height) {
    this->tunnelerView[height][width] = value;

    return this;
}

Floor* Floor::setNonTunnelerViewAt(u_char value, u_char width, u_char height) {
    this->nonTunnelerView[height][width] = value;

    return this;
}

Floor* Floor::setCheapestPathToPlayer(u_char value, u_char width, u_char height) {
    this->cheapestPathToPlayer[height][width] = value;

    return this;
}

Floor* Floor::setMonster(Monster* monster, u_short index) {
    if (index == U_SHORT_MAX) {
        this->monsters.push_back(monster);
    } else if (index < this->getRoomCount()) {
        this->monsters[index] = monster;
    } else {
        throw Exception::MonsterOutOfBounds();
    }

    return this;
}

Floor* Floor::setUpStair(Staircase* staircase, u_short index) {
    if (index == U_SHORT_MAX) {
        this->upStairs.push_back(staircase);
    } else if (index < this->getStairUpCount()) {
        this->upStairs[index] = staircase;
    } else {
        throw Exception::StaircaseOutOfBounds();
    }

    return this;
}

Floor* Floor::setDownStair(Staircase* staircase, u_short index) {
    if (index == U_SHORT_MAX) {
        this->downStairs.push_back(staircase);
    } else if (index < this->getStairDownCount()) {
        this->downStairs[index] = staircase;
    } else {
        throw Exception::StaircaseOutOfBounds();
    }

    return this;
}

Floor* Floor::setRoom(Room* room, u_short index) {
    if (index == U_SHORT_MAX) {
        this->rooms.push_back(room);
    } else if (index < this->getRoomCount()) {
        this->rooms[index] = room;
    } else {
        throw Exception::RoomOutOfBounds();
    }

    return this;
}
/** SETTERS **/

Floor* Floor::initializeToNull() {
    u_char y;
    u_char x;

    for (y = 0; y < DUNGEON_FLOOR_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            this->characters[y][x] = null;
            this->terrains[y][x] = null;
            this->objectsMap[y][x] = null;
            this->tunnelerView[y][x] = U_CHAR_MIN;
            this->nonTunnelerView[y][x] = U_CHAR_MIN;
            this->cheapestPathToPlayer[y][x] = U_CHAR_MIN;
        }
    }

    return this;
}

Floor* Floor::generateBorders() {
    u_char y;
    u_char x;

    // Set NORTH and SOUTH walls, ignoring corners
    for (x = 1; x < DUNGEON_FLOOR_WIDTH - 1; x++) {
        // Create a new border
        this->terrains[0][x] = new Border(this, 0, x, NORTH_SOUTH_WALL_CHARACTER);
        this->terrains[DUNGEON_FLOOR_HEIGHT - 1][x] = new Border(this, x, DUNGEON_FLOOR_HEIGHT - 1, NORTH_SOUTH_WALL_CHARACTER);
    }

    // Set EAST and WEST walls, ignoring corners
    for (y = 1; y < DUNGEON_FLOOR_HEIGHT - 1; y++) {
        // Create a new border
        this->terrains[y][0] = new Border(this, 0, y, EAST_WEST_WALL_CHARACTER);
        this->terrains[y][DUNGEON_FLOOR_WIDTH - 1] = new Border(this, DUNGEON_FLOOR_WIDTH - 1, y, EAST_WEST_WALL_CHARACTER);
    }

    // Set top left corner
    this->terrains[0][0] = new Border(this, 0, 0, CORNER_WALL_CHARACTER);

    // Set top right corner
    this->terrains[0][DUNGEON_FLOOR_WIDTH - 1] = new Border(this, DUNGEON_FLOOR_WIDTH - 1, 0, CORNER_WALL_CHARACTER);

    // Set bottom left corner
    this->terrains[DUNGEON_FLOOR_HEIGHT - 1][0] = new Border(this, 0, DUNGEON_FLOOR_HEIGHT - 1, CORNER_WALL_CHARACTER);

    // Set bottom right corner
    this->terrains[DUNGEON_FLOOR_HEIGHT - 1][DUNGEON_FLOOR_WIDTH - 1] = new Border(this, DUNGEON_FLOOR_WIDTH - 1, DUNGEON_FLOOR_HEIGHT - 1, CORNER_WALL_CHARACTER);

    return this;
}

Floor* Floor::generateRock() {
    u_char height;
    u_char width;
    u_char index;
    u_char x;
    u_char y;
    u_int s;
    u_int t;
    u_int p;
    u_int q;

    int gaussian[5][5] = {
            {1, 4,  7,  4,  1},
            {4, 16, 26, 16, 4},
            {7, 26, 41, 26, 7},
            {4, 16, 26, 16, 4},
            {1, 4,  7,  4,  1}
    };

    // Generate hardness
    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            if (this->terrains[height][width] == null) {
                this->terrains[height][width] = new Rock(this, width, height, u_char(Dice::RandomNumberBetween(ROCK_HARDNESS_MIN, ROCK_HARDNESS_MAX)));
            }
        }
    }

    // Smooth out hardness so there is no sharp changes
    for (index = 0; index < 1; index++) {
        for (y = 0; y < DUNGEON_FLOOR_HEIGHT; y++) {
            for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
                for (s = t = p = 0; p < 5; p++) {
                    for (q = 0; q < 5; q++) {
                        if (y + (p - 2) >= 0 && y + (p - 2) < DUNGEON_FLOOR_HEIGHT &&
                            x + (q - 2) >= 0 && x + (q - 2) < DUNGEON_FLOOR_WIDTH) {
                            s += gaussian[p][q];
                            t += this->terrains[u_char(y + (p - 2))][u_char(x + (q - 2))]->getHardness() * gaussian[p][q];
                        }
                    }
                }
                if (this->terrains[y][x] != null && this->terrains[y][x]->isRock()) {
                    this->terrains[y][x]->setHardness(u_char(t / s));
                }
            }
        }
    }

    return this;
}

Floor* Floor::generateRooms(u_short roomCount, u_short stairUpCount, u_short stairDownCount) {
    // Do some quick math to determine if the floor has room for all these rooms
    if (roomCount * (ROOM_MAX_WIDTH * ROOM_MAX_HEIGHT) > .6 * (DUNGEON_FLOOR_WIDTH * DUNGEON_FLOOR_HEIGHT)) {
        // If the max room characters is greater than 50% of the space available in the room, leave
        throw Exception::RoomEmptySpaceInvalid();
    }

    u_short index;
    u_short roomIndex;

    u_char roomStartX;
    u_char roomStartY;

    u_char roomWidth;
    u_char roomHeight;

    u_char width;
    u_char height;

    bool collision;

    // Place to store the random stair room ids
    u_char stairX;
    u_char stairY;
    Room* room;

    std::vector<u_short> upStairRooms;
    std::vector<u_short> downStairRooms;

    // Determine the rooms for each of the staircases
    for (roomIndex = 0; roomIndex < stairUpCount; roomIndex++) {
        upStairRooms.push_back(u_short(Dice::RandomNumberBetween(0, roomCount - 1)));
    }

    for (roomIndex = 0; roomIndex < stairDownCount; roomIndex++) {
        downStairRooms.push_back(u_short(Dice::RandomNumberBetween(0, roomCount - 1)));
    }

    // Place each room
    for (index = 0; index < roomCount; index++) {
        do {
            collision = false;

            // Find something inside the game play box
            roomStartX = Dice::RandomNumberBetween(1, DUNGEON_FLOOR_WIDTH - 2);
            roomStartY = Dice::RandomNumberBetween(1, DUNGEON_FLOOR_HEIGHT - 2);

            // Select a random height and width
            roomWidth = Dice::RandomNumberBetween(ROOM_MIN_WIDTH, ROOM_MAX_WIDTH);
            roomHeight = Dice::RandomNumberBetween(ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT);

            // Need to check boundaries one off to make sure they are open spaces
            for (height = roomStartY - 1; height < (roomStartY + roomHeight + 1); height++) {
                // Need to check boundaries one off to make sure they are open spaces
                for (width = roomStartX - 1; width < (roomStartX + roomWidth + 1); width++) {
                    if (
                            width >= DUNGEON_FLOOR_WIDTH - 1 || // Check if width in bounds
                            height >= DUNGEON_FLOOR_HEIGHT - 1 || // Check if height in bounds
                            this->terrains[height][width] != null // Check if another item already exists
                            ) {
                        collision = true;
                    }
                }
            }
        } while (collision);

        room = new Room(this, roomStartX, roomStartY, roomWidth, roomHeight);
        this->rooms.push_back(room);

        // Are we placing any staircases in this room?
        for (roomIndex = 0; roomIndex < stairUpCount; roomIndex++) {
            if (upStairRooms.at(roomIndex) == index) {
                // Place the up staircase
                do {
                    stairX = room->randomXInside();
                    stairY = room->randomYInside();
                } while (this->terrains[stairY][stairX] != null);

                this->terrains[stairY][stairX] = new Staircase(this, stairX, stairY, this->upStairs.size(), STAIRCASE_DIRECTION_UP);
                this->upStairs.push_back(new Staircase(this, stairX, stairY, this->upStairs.size(), STAIRCASE_DIRECTION_UP));
            }
        }

        for (roomIndex = 0; roomIndex < stairDownCount; roomIndex++) {
            if (downStairRooms.at(roomIndex) == index) {
                // Place the down staircase
                do {
                    stairX = room->randomXInside();
                    stairY = room->randomYInside();
                } while (this->terrains[stairY][stairX] != null);

                this->terrains[stairY][stairX] = new Staircase(this, stairX, stairY, this->downStairs.size(), STAIRCASE_DIRECTION_DOWN);
                this->downStairs.push_back(new Staircase(this, stairX, stairY, this->downStairs.size(), STAIRCASE_DIRECTION_DOWN));
            }
        }

        // Valid room finally found, place it
        for (height = roomStartY; height < roomStartY + roomHeight; height++) {
            for (width = roomStartX; width < roomStartX + roomWidth; width++) {
                if (this->terrains[height][width] == null) {
                    this->terrains[height][width] = room;
                }
            }
        }
    }

    return this;
}

Floor* Floor::generateCorridors() {
    bool upValid;
    bool downValid;
    bool leftValid;
    bool rightValid;

    Room* firstRoom;
    u_char firstRoomX;
    u_char firstRoomY;

    Room* secondRoom;
    u_char secondRoomX;
    u_char secondRoomY;

    u_char tempX;
    u_char tempY;

    u_short index;
    for (index = 0; index < this->getRoomCount() - 1; index++) {
        firstRoom = this->rooms[index];
        secondRoom = this->rooms[index + 1];

        // First we want to select a random spot within the room, but it needs to be on the border
        do {
            firstRoomX = firstRoom->randomXInside();
            firstRoomY = firstRoom->randomYInside();

            upValid = this->terrains[firstRoomY - 1][firstRoomX] == null;
            downValid = this->terrains[firstRoomY + 1][firstRoomX] == null;
            leftValid = this->terrains[firstRoomY][firstRoomX - 1] == null;
            rightValid = this->terrains[firstRoomY][firstRoomX + 1] == null;
        } while (upValid || downValid || leftValid || rightValid);

        // Second we want to select a random spot within the next room, but it needs to be on the border
        do {
            secondRoomX = secondRoom->randomXInside();
            secondRoomY = secondRoom->randomYInside();

            upValid = this->terrains[secondRoomY - 1][secondRoomX] == null;
            downValid = this->terrains[secondRoomY + 1][secondRoomX] == null;
            leftValid = this->terrains[secondRoomY][secondRoomX - 1] == null;
            rightValid = this->terrains[secondRoomY][secondRoomX + 1] == null;
        } while (upValid || downValid || leftValid || rightValid);

        // Now connect them in the X direction
        tempX = firstRoomX;
        while (tempX != secondRoomX) {
            if (tempX > secondRoomX) {
                tempX--;
            } else if (tempX < secondRoomX) {
                tempX++;
            }

            if (this->terrains[secondRoomY][tempX] == null) {
                this->terrains[secondRoomY][tempX] = new Corridor(this, tempX, secondRoomY);
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

            if (this->terrains[tempY][firstRoomX] == null) {
                this->terrains[tempY][firstRoomX] = new Corridor(this, firstRoomX, tempY);
            }
        }

        // Handle the corner case
        if (this->terrains[secondRoomY][firstRoomX] == null) {
            this->terrains[secondRoomY][firstRoomX] = new Corridor(this, firstRoomX, secondRoomY);
        }
    }

    return this;
}

Floor* Floor::generatePlayer() {
    if (this->floorNumber == 0) {
        // Need to place player
        // Select random room and random coords and place character there
        u_char playerX;
        u_char playerY;
        auto room = u_short(Dice::RandomNumberBetween(0, this->getRoomCount() - 1));
        do {
            playerX = this->rooms.at(room)->randomXInside();
            playerY = this->rooms.at(room)->randomYInside();
        } while (this->terrains[playerY][playerX]->getCharacter() == STAIRCASE_UP_CHARACTER &&
                 this->terrains[playerY][playerX]->getCharacter() == STAIRCASE_DOWN_CHARACTER);

        this->characters[playerY][playerX] = dungeon->getPlayer()->setFloor(this)->setX(playerX)->setY(playerY);
    }

    return this;
}

Floor* Floor::generateMonsters(u_short monsterCount) {
    u_char width;
    u_char height;

    u_short index;

    u_char monsterX;
    u_char monsterY;

    Room* monsterRoom;
    Room* playerRoom = null;

    u_int placementAttempts = 0;
    u_short maxMonsters = 0;
    u_char roomIndex;

    // Find out the max number of monsters possible within the program
    for (roomIndex = 0; roomIndex < this->getRoomCount(); roomIndex++) {
        maxMonsters += this->rooms[roomIndex]->getArea();
    }

    if (this->dungeon->getPlayer()->getFloor() == this) {
        playerRoom = (Room*) this->terrains[this->dungeon->getPlayer()->getY()][this->dungeon->getPlayer()->getX()];
        maxMonsters -= playerRoom->getArea();
    }

    // If there are more monsters trying to be placed than monsters available, set the max number of monsters
    if (monsterCount > maxMonsters) {
        monsterCount = maxMonsters;
    }

    for (index = 0; index < monsterCount; index++) {
        placementAttempts = 0;

        do {
            // Select a room as long as it isn't the player's room
            do {
                monsterRoom = this->rooms[Dice::RandomNumberBetween(0, this->getRoomCount() - 1)];
            } while (monsterRoom == playerRoom);

            // Select random spot inside the room
            monsterX = monsterRoom->randomXInside();
            monsterY = monsterRoom->randomYInside();

            placementAttempts++;
        } while (this->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < this->getRoomCount(); roomIndex++) {
                monsterRoom = this->rooms[roomIndex];
                if (monsterRoom == playerRoom) {
                    continue;
                }

                // Start looping and find the next open spot
                for (height = monsterRoom->getStartY(); height < monsterRoom->getEndY(); height++) {
                    for (width = monsterRoom->getStartX(); width < monsterRoom->getEndX(); width++) {
                        if (this->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        // Select a random monster template
        MonsterTemplate* monsterTemplate = this->dungeon->randomMonsterTemplate();
        Monster* monster = monsterTemplate->generateMonster(this, monsterX, monsterY);
        // If the monster generated is a boss or is unique, remove from possible templates
        if (monster->isBoss() || monster->isUnique()) {
            if (monster->isBoss()) {
                this->dungeon->setBoss(monster);
            }
            this->dungeon->removeMonsterTemplate(monsterTemplate);
        }

        this->monsters.push_back(monster);
        this->characters[monsterY][monsterX] = this->monsters.at(index);
    }

    return this;
}

Floor* Floor::generateObjects(u_short objectCount) {
    u_short placementAttempts;

    Room* objectRoom;
    u_short index;
    u_short roomIndex;
    u_char height;
    u_char width;

    u_char objectX;
    u_char objectY;

    u_short maxObjects = 0;

    // Find out the max number of monsters possible within the program
    for (index = 0; index < this->getRoomCount(); index++) {
        maxObjects += this->rooms[index]->getArea();
    }

    // If there are more monsters trying to be placed than monsters available, set the max number of monsters
    if (objectCount > maxObjects) {
        objectCount = maxObjects;
    }

    for (index = 0; index < objectCount; index++) {
        placementAttempts = 0;

        do {
            objectRoom = this->rooms[Dice::RandomNumberBetween(0, this->getRoomCount() - 1)];

            // Select random spot inside the room
            objectX = objectRoom->randomXInside();
            objectY = objectRoom->randomYInside();

            placementAttempts++;
        } while (this->objectsMap[objectY][objectX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < this->getRoomCount(); roomIndex++) {
                objectRoom = this->rooms[roomIndex];

                // Start looping and find the next open spot
                for (height = objectRoom->getStartY(); height < objectRoom->getEndY(); height++) {
                    for (width = objectRoom->getStartX(); width < objectRoom->getEndX(); width++) {
                        if (this->objectsMap[height][width] == null) {
                            objectX = width;
                            objectY = height;
                        }
                    }
                }
            }
        }

        // Select a random template and make an object
        ObjectTemplate* objectTemplate = this->dungeon->randomObjectTemplate();
        Object* object = objectTemplate->generateObject(this, objectX, objectY);

        // If the monster generated is an artifact, remove from possible templates
        if (object->getIsArtifact()) {
            this->dungeon->removeObjectTemplate(objectTemplate);
        }

        this->objects.push_back(object);
        this->objectsMap[objectY][objectX] = this->objects.at(index);
    }

    return this;
}

Floor* Floor::loadBorders() {
    return this;
}

Floor* Floor::loadRock() {
    return this;
}

Floor* Floor::loadRooms() {
    return this;
}

Floor* Floor::loadCorridors() {
    return this;
}

Floor* Floor::loadPlayer() {
    return this;
}

Floor* Floor::loadMonsters() {
    return this;
}