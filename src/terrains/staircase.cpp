#include "staircase.h"

Staircase::Staircase(Floor* floor, u_short id, u_char x, u_char y, char type) : Terrain(floor, id, x, y) {
    this->walkable = true;
    this->rock = false;
    this->immutable = false;

    this->hardness = STAIRCASE_HARDNESS;
    this->type = type;

    if (this->type == STAIRCASE_TYPE_DOWN) {
        this->character = STAIRCASE_DOWN_CHARACTER;
    } else if (this->type == STAIRCASE_TYPE_UP) {
        this->character = STAIRCASE_UP_CHARACTER;
    } else {
        this->character = UNKNOWN_CHARACTER;
    }
}

Staircase::~Staircase() = default;

int Staircase::take() {
    u_char height;
    u_char width;
    Dungeon* dungeon = this->getFloor()->getDungeon();
    Player* player = dungeon->getPlayer();
    Floor* currentFloor = dungeon->getCurrentFloor();
    // Wipe the current queue of events
    delete (dungeon->getEventManager());

    // Create a new event manager for the floor upcoming floor
    dungeon->setEventManager(new EventManager(dungeon));

    // Update character map
    currentFloor->setCharacterAt(null, player->getX(), player->getY());

    // Set dungeon floor and player floor pointers
    dungeon->setCurrentFloor(this->getTargetFloor());
    player->setFloor(dungeon->getCurrentFloor());
    currentFloor = dungeon->getCurrentFloor();

    // Place the character on the upper floor's staircase
    // If up staircase and there is a down staircase to step on
    if (this->isUp() && this->getId() < currentFloor->getStairDownCount()) {
        player->setX(currentFloor->getDownStair(this->getId())->getX());
        player->setY(currentFloor->getDownStair(this->getId())->getY());
    }
        // If down staircase and there is an up staircase to step on
    else if (this->isDown() && this->getId() < currentFloor->getStairUpCount()) {
        player->setX(currentFloor->getUpStair(this->getId())->getX());
        player->setY(currentFloor->getUpStair(this->getId())->getY());
    }
        // Else random location
    else {
        u_char playerX;
        u_char playerY;
        auto room = u_short(Dice::RandomNumberBetween(0, currentFloor->getRoomCount() - 1));

        playerX = u_char(Dice::RandomNumberBetween(currentFloor->getRoom(room)->getStartingX(), currentFloor->getRoom(room)->getStartingX() + currentFloor->getRoom(room)->getWidth() - 1));
        playerY = u_char(Dice::RandomNumberBetween(currentFloor->getRoom(room)->getStartingY(), currentFloor->getRoom(room)->getStartingY() + currentFloor->getRoom(room)->getHeight() - 1));

        if (currentFloor->getCharacterAt(playerX, playerY) != null) {
            currentFloor->getCharacterAt(playerX, playerY)->killCharacter();
        }

        player->setX(playerX);
        player->setY(playerY);
    }

    currentFloor->setCharacterAt(dungeon->getPlayer(), player->getX(), player->getY());

    // Move monster's on that floor to random locations not in the same room as the user
    // We said that staircases cannot be at the edge of rooms, this is why
    Room* playerRoom = (Room*) currentFloor->getTerrainAt(player->getX() - u_char(1), player->getY());

    for (height = playerRoom->getStartingY(); height < playerRoom->getStartingY() + playerRoom->getHeight(); height++) {
        for (width = playerRoom->getStartingX(); width < playerRoom->getStartingX() + playerRoom->getWidth(); width++) {
            if (currentFloor->getCharacterAt(width, height) != null && currentFloor->getCharacterAt(width, height)->getIsMonster()) {
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
                    monsterX = u_char(Dice::RandomNumberBetween(monsterRoom->getStartingX(), monsterRoom->getStartingX() + monsterRoom->getWidth() - 1));
                    monsterY = u_char(Dice::RandomNumberBetween(monsterRoom->getStartingY(), monsterRoom->getStartingY() + monsterRoom->getHeight() - 1));

                    placementAttempts++;
                } while (currentFloor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

                // If failed to find, just man handle it through
                if (placementAttempts >= 25) {
                    for (roomIndex = 0; roomIndex < currentFloor->getRoomCount(); roomIndex++) {
                        if (currentFloor->getRoom(roomIndex)->getId() == playerRoom->getId()) {
                            continue;
                        }
                        // Start looping and find the next open spot
                        for (height = currentFloor->getRoom(roomIndex)->getStartingY(); height < currentFloor->getRoom(roomIndex)->getStartingY() + currentFloor->getRoom(roomIndex)->getHeight(); height++) {
                            for (width = currentFloor->getRoom(roomIndex)->getStartingX(); width < currentFloor->getRoom(roomIndex)->getStartingX() + currentFloor->getRoom(roomIndex)->getWidth(); width++) {
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

    u_short index = 0;
    // Add monsters to the new queue
    for (index = 0; index < currentFloor->getMonsterCount(); index++) {
        if (currentFloor->getMonster(index)->getIsAlive()) {
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
    return this->floor->getDungeon()->getFloor(this->floor->getDungeon()->getCurrentFloor()->getFloorNumber() + this->type);
}

bool Staircase::isUp() {
    return this->type == STAIRCASE_TYPE_UP;
}

bool Staircase::isDown() {
    return this->type == STAIRCASE_TYPE_DOWN;
}

/** GETTERS **/
char Staircase::getType() {
    return this->type;
}
/** GETTERS **/

/** SETTERS **/
Staircase* Staircase::setType(char type) {
    this->type = type;

    return this;
}
/** SETTERS **/