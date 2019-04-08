#include "staircase.h"

Staircase::Staircase(Floor* floor, u_short id, u_char x, u_char y, char direction) : Terrain(floor, id, x, y) {
    this->hardness = STAIRCASE_HARDNESS;
    this->direction = direction;
    this->type = TERRAIN_STAIRCASE;

    if (this->direction == STAIRCASE_DIRECTION_DOWN) {
        this->character = STAIRCASE_DOWN_CHARACTER;
    } else if (this->direction == STAIRCASE_DIRECTION_UP) {
        this->character = STAIRCASE_UP_CHARACTER;
    } else {
        this->character = UNKNOWN_CHARACTER;
    }
}

Staircase::~Staircase() = default;

int Staircase::take() {
    u_char height;
    u_char width;
    u_short index;
    Dungeon* dungeon = this->floor->getDungeon();
    Player* player = dungeon->getPlayer();
    Floor* currentFloor = dungeon->getCurrentFloor();

    // Wipe the current queue of events and create a new one
    delete (dungeon->getEventManager());
    dungeon->setEventManager(new EventManager(dungeon));

    // Remove the current player from the character map
    currentFloor->setCharacterAt(null, player->getX(), player->getY());

    // Set dungeon floor and player floor pointers
    dungeon->setCurrentFloor(this->getTargetFloor());
    player->setFloor(this->getTargetFloor());

    // Update current floor
    currentFloor = dungeon->getCurrentFloor();

    // Place the character on the upper floor's staircase
    // If up staircase and there is a down staircase to step on
    if (this->isUp() && this->getId() < currentFloor->getStairDownCount()) {
        player->setX(currentFloor->getDownStair(this->getId())->getX())->setY(currentFloor->getDownStair(this->getId())->getY());
    }
        // If down staircase and there is an up staircase to step on
    else if (this->isDown() && this->getId() < currentFloor->getStairUpCount()) {
        player->setX(currentFloor->getUpStair(this->getId())->getX())->setY(currentFloor->getUpStair(this->getId())->getY());
    }
        // Else random location
    else {
        u_char playerX;
        u_char playerY;
        auto room = u_short(Dice::RandomNumberBetween(0, currentFloor->getRoomCount() - 1));

        playerX = currentFloor->getRoom(room)->randomXInside();
        playerY = currentFloor->getRoom(room)->randomYInside();

        if (currentFloor->getCharacterAt(playerX, playerY) != null) {
            currentFloor->getCharacterAt(playerX, playerY)->killCharacter();
        }

        player->setX(playerX)->setY(playerY);
    }

    currentFloor->setCharacterAt(dungeon->getPlayer(), player->getX(), player->getY());

    // Move monster's on that floor to random locations not in the same room as the user
    // We said that staircases cannot be at the edge of rooms, this is why
    Room* playerRoom = (Room*) currentFloor->getTerrainAt(player->getX() - u_char(1), player->getY());

    for (height = playerRoom->getStartingY(); height < playerRoom->getStartingY() + playerRoom->getHeight(); height++) {
        for (width = playerRoom->getStartingX(); width < playerRoom->getStartingX() + playerRoom->getWidth(); width++) {
            if (currentFloor->getCharacterAt(width, height) != null && currentFloor->getCharacterAt(width, height)->isMonster()) {
                u_char placementAttempts = 0;
                Room* monsterRoom;
                u_char monsterX;
                u_char monsterY;
                u_short roomIndex;
                bool placed = false;
                // Select random spots until they are only surrounded by room space
                do {
                    do {
                        monsterRoom = currentFloor->getRoom(u_char(Dice::RandomNumberBetween(0, currentFloor->getRoomCount() - 1)));
                    } while (monsterRoom->getId() == playerRoom->getId());

                    // Select random spot inside the room
                    monsterX = monsterRoom->randomXInside();
                    monsterY = monsterRoom->randomYInside();

                    placementAttempts++;
                } while (currentFloor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

                // If failed to find, just man handle it through
                if (placementAttempts >= 25) {
                    for (roomIndex = 0; roomIndex < currentFloor->getRoomCount(); roomIndex++) {
                        if (currentFloor->getRoom(roomIndex)->getId() == playerRoom->getId()) {
                            continue;
                        }
                        // Start looping and find the next open spot
                        for (height = currentFloor->getRoom(roomIndex)->getStartY(); height < currentFloor->getRoom(roomIndex)->getEndY(); height++) {
                            for (width = currentFloor->getRoom(roomIndex)->getStartX(); width < currentFloor->getRoom(roomIndex)->getEndX(); width++) {
                                if (currentFloor->getCharacterAt(width, height) == null) {
                                    monsterX = width;
                                    monsterY = height;
                                }
                            }
                        }
                    }
                } else {
                    placed = true;
                }

                if (placed) {
                    // Place monster
                    currentFloor->getCharacterAt(width, height)->setX(monsterX)->setY(monsterY);
                    currentFloor->setCharacterAt(currentFloor->getCharacterAt(width, height), monsterX, monsterY);
                } else {
                    // Kill it
                    currentFloor->getCharacterAt(width, height)->killCharacter();
                }

                currentFloor->setCharacterAt(null, width, height);
            }
        }
    }

    // Update monster mappings for that floor
    Monster::RunDijkstraOnFloor(currentFloor);

    // Add player to the new queue
    dungeon->getEventManager()->addToQueue(new Event(0, event_type_player, player, Player::HandleEvent, Player::NextEventTick));

    // Add monsters to the new queue
    for (index = 0; index < currentFloor->getMonsterCount(); index++) {
        if (currentFloor->getMonster(index)->isAlive()) {
            dungeon->getEventManager()->addToQueue(new Event(1 + index, event_type_monster, currentFloor->getMonster(index), Monster::HandleEvent, Monster::NextEventTick));
        }
    }

    // Reset visibility
    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            player->visibility[height][width] = null;
        }
    }

    player->updateVisibility();

    return 0;
}

Floor* Staircase::getTargetFloor() {
    return this->floor->getDungeon()->getFloor(this->floor->getDungeon()->getCurrentFloor()->getFloorNumber() + this->direction);
}

bool Staircase::isUp() {
    return this->direction == STAIRCASE_DIRECTION_UP;
}

bool Staircase::isDown() {
    return this->direction == STAIRCASE_DIRECTION_DOWN;
}

/** GETTERS **/
char Staircase::getDirection() {
    return this->direction;
}
/** GETTERS **/

/** SETTERS **/
/** SETTERS **/