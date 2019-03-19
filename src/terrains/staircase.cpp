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
    Dungeon* dungeon = this->getFloor()->getDungeon();
    Player* player = dungeon->getPlayer();
    Floor* currentFloor = dungeon->getCurrentFloor();
    // Find the target staircase index
    u_short staircaseIndex;
    if (this->isUp()) {
        for (staircaseIndex = 0; staircaseIndex < currentFloor->getStairUpCount(); staircaseIndex++) {
            if (currentFloor->getUpStairs().at(staircaseIndex) == this) {
                break;
            }
        }
    } else {
        for (staircaseIndex = 0; staircaseIndex < currentFloor->getStairDownCount(); staircaseIndex++) {
            if (currentFloor->getDownStairs().at(staircaseIndex) == this) {
                break;
            }
        }
    }

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
    if (this->isUp() && staircaseIndex < currentFloor->getStairDownCount()) {
        player->setX(currentFloor->getDownStairs().at(staircaseIndex)->getX());
        player->setY(currentFloor->getDownStairs().at(staircaseIndex)->getY());
    }
        // If down staircase and there is an up staircase to step on
    else if (this->isDown() && staircaseIndex < currentFloor->getStairUpCount()) {
        player->setX(currentFloor->getUpStairs().at(staircaseIndex)->getX());
        player->setY(currentFloor->getUpStairs().at(staircaseIndex)->getY());
    }
        // Else random location
    else {
        u_char playerX;
        u_char playerY;
        auto room = u_short(random_number_between(0, currentFloor->getRoomCount() - 1));
        do {
            playerX = u_char(random_number_between(currentFloor->getRooms().at(room)->getStartingX(), currentFloor->getRooms().at(room)->getStartingX() + currentFloor->getRooms().at(room)->getWidth() - 1));
            playerY = u_char(random_number_between(currentFloor->getRooms().at(room)->getStartingY(), currentFloor->getRooms().at(room)->getStartingY() + currentFloor->getRooms().at(room)->getHeight() - 1));
        } while (currentFloor->getTerrainAt(playerX, playerY)->getCharacter() != STAIRCASE_UP_CHARACTER && currentFloor->getTerrainAt(playerX, playerY)->getCharacter() != STAIRCASE_DOWN_CHARACTER);

        if (currentFloor->getCharacterAt(playerX, playerY) != null) {
            currentFloor->getCharacterAt(playerX, playerY)->killCharacter();
        }

        player->setX(playerX);
        player->setY(playerY);
    }

    currentFloor->setCharacterAt(dungeon->getPlayer(), player->getX(), player->getY());

    // Move monster's on that floor to random locations not in the same room as the user
    Room* playerRoom = (Room*) currentFloor->getTerrainAt(player->getX(), player->getY());

    u_char height = 0;
    u_char width = 0;

    for (height = playerRoom->getStartingY(); height < playerRoom->getStartingY() + playerRoom->getHeight(); height++) {
        for (width = playerRoom->getStartingX(); width < playerRoom->getStartingX() + playerRoom->getWidth(); width++) {
            if (currentFloor->getCharacterAt(width, height) != null && currentFloor->getCharacterAt(width, height)->getIsMonster()) {
                // Move that monster somewhere else or kill it
                Room* randomRoomPlacement;
                bool placed = false;
                u_char monsterHeight;
                u_char monsterWidth;
                do {
                    randomRoomPlacement = currentFloor->getRooms().at(u_char(random_number_between(0, currentFloor->getRoomCount() - 1)));
                } while (randomRoomPlacement == playerRoom);

                for (monsterHeight = randomRoomPlacement->getStartingY(); monsterHeight < randomRoomPlacement->getStartingY() + randomRoomPlacement->getWidth() && !placed; monsterHeight++) {
                    for (monsterWidth = randomRoomPlacement->getStartingX(); monsterWidth < randomRoomPlacement->getStartingX() + randomRoomPlacement->getHeight() && !placed; monsterWidth++) {
                        if (currentFloor->getCharacterAt(monsterWidth, monsterHeight) == null) {
                            currentFloor->setCharacterAt(currentFloor->getCharacterAt(width, height), monsterWidth, monsterHeight)
                                    ->getCharacterAt(monsterWidth, monsterHeight)
                                    ->setY(monsterHeight)->setX(monsterWidth);

                            placed = true;
                        }
                    }
                }

                // If not placed, just kill the monster
                if (!placed) {
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
        if (currentFloor->getMonsters().at(index)->getIsAlive()) {
            dungeon->getEventManager()->addToQueue(new Event(1 + index, event_type_monster, currentFloor->getMonsters().at(index), Monster::HandleEvent, Monster::NextEventTick));
        }
    }

    return 0;
}

Floor* Staircase::getTargetFloor() {
    return this->floor->getDungeon()->getFloors().at(
            this->floor->getDungeon()->getCurrentFloor()->getFloorNumber() + this->type
    );
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