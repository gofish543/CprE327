#include "player.h"

Player::Player(Floor* floor, u_char x, u_char y) : Character(floor, x, y, PLAYER_CHARACTER, PLAYER_SPEED, true, false) {
    u_char height;
    u_char width;

    this->takingStaircase = null;

    this->requestTerminate = false;

    this->level = 0;
    this->monstersSlain = 0;
    this->daysSurvived = 0;

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            this->visibility[height][width] = null;
        }
    }
}

Player::Player(Floor* floor, u_char x, u_char y, u_int level, u_int monstersSlain, u_int daysSurvived)
        : Player(floor, x, y) {
    this->takingStaircase = null;

    this->requestTerminate = false;

    this->level = level;
    this->monstersSlain = monstersSlain;
    this->daysSurvived = daysSurvived;
}

Player::~Player() = default;

int Player::NextEventTick(Event* event) {
    auto player = (Player*) event->character;

    if (player->isAlive()) {
        return player->getFloor()->getDungeon()->getEventManager()->getTick() + (CHARACTER_SPEED_NUMERATOR / player->getSpeed());
    } else {
        return -1;
    }
}

int Player::HandleEvent(Event* event) {
    int move;
    auto player = (Player*) event->character;
    Dungeon* dungeon = player->getFloor()->getDungeon();

    dungeon->getOutput()->print();

    move = getChar(dungeon->getSettings()->doNCursesPrint());

    switch (move) {
        case 'm':
            player->handleEventKeyMonsterMenu();

            return Player::HandleEvent(event);
        case 'f':
            player->handleEventKeyToggleFog();

            return Player::HandleEvent(event);
        case 'g':
            player->handleEventKeyTeleport();

            return 0;
        case '<':
        case '>':
            if (player->handleEventKeyStaircase(move)) {
                return Player::HandleEvent(event);
            }
            return 0;
        default:
            if (player->handleEventKeyMovement(move)) {
                return Player::HandleEvent(event);
            }
            return 0;
    }
}

int Player::handleEventKeyMonsterMenu() {
    Dungeon* dungeon = this->getFloor()->getDungeon();
    u_short startIndex = 0;
    int character = 0;

    while (character != 27) {
        dungeon->getOutput()->printMonsterMenu(startIndex);
        dungeon->getOutput()->print("Esc: Close\nArrowUp: Scroll Down\nArrowDown: Scroll Up\n");

        character = getChar(dungeon->getSettings()->doNCursesPrint());

        switch (character) {
            case KEY_DOWN:
                if (startIndex < dungeon->getCurrentFloor()->getMonsterCount()) {
                    startIndex++;
                }
                break;
            case KEY_UP:
                if (startIndex > 0) {
                    startIndex--;
                }
                break;
            default:
                break;
        }
    }

    dungeon->getOutput()->print();

    return 0;
}

int Player::handleEventKeyToggleFog() {
    this->getFloor()->getDungeon()->getSettings()->setFogOfWar(
            !this->getFloor()->getDungeon()->getSettings()->doFogOfWar()
    );

    this->getFloor()->getDungeon()->getOutput()->print();

    return 0;
}

int Player::handleEventKeyTeleport() {
    Dungeon* dungeon = this->getFloor()->getDungeon();

    // Store away original settings
    bool originalFogOfWar = dungeon->getSettings()->doFogOfWar();
    u_char originalX = this->getX();
    u_char originalY = this->getY();
    int character = 0;
    Character* save = null;

    // Change user character, print, and pull from map
    this->setCharacter(PLAYER_TELEPORT_CHARACTER);
    dungeon->getSettings()->setFogOfWar(false);

    this->getFloor()->getDungeon()->getOutput()->print();

    this->getFloor()->setCharacterAt(null, this->x, this->y);

    while (character != 27 && character != 'g' && character != 'r') {
        character = getChar(dungeon->getSettings()->doNCursesPrint());
        this->getFloor()->setCharacterAt(null, this->getX(), this->getY());

        switch (character) {
            case KEY_UP:
                this->y = u_char(std::max(1, this->y - 1));
                break;
            case KEY_DOWN:
                this->y = u_char(std::min(DUNGEON_FLOOR_HEIGHT - 2, this->y + 1));
                break;
            case KEY_RIGHT:
                this->x = u_char(std::min(DUNGEON_FLOOR_WIDTH - 2, this->x + 1));
                break;
            case KEY_LEFT:
                this->x = u_char(std::max(1, this->x - 1));
                break;
            default:
                break;
        }

        // Restore previous spot
        if (save != null) {
            this->getFloor()->setCharacterAt(save, save->getX(), save->getY());
        }

        // Store away target spot
        save = this->getFloor()->getCharacterAt(this->x, this->y);

        // Move our "teleport" character there
        this->getFloor()->setCharacterAt(this, this->x, this->y);

        // Print
        this->getFloor()->getDungeon()->getOutput()->print();
    }

    switch (character) {
        default:
        case 27: // Esc
            // Revert to original characters
            this->getFloor()->setCharacterAt(null, this->x, this->y);
            this->setX(originalX)->setY(originalY);
            break;
        case 'g':
            break;
        case 'r':
            this->getFloor()->setCharacterAt(null, this->x, this->y);

            Room* randomRoom = this->getFloor()->getRoom(u_short(Dice::RandomNumberBetween(0, this->getFloor()->getRoomCount() - 1)));
            this->setX(u_char(Dice::RandomNumberBetween(randomRoom->getStartingX(), randomRoom->getStartingX() + randomRoom->getWidth() - 1)));
            this->setY(u_char(Dice::RandomNumberBetween(randomRoom->getStartingY(), randomRoom->getStartingY() + randomRoom->getHeight() - 1)));

            // Restore previous location
            if (save != null) {
                this->getFloor()->setCharacterAt(save, save->getX(), save->getY());
            }

            save = this->getFloor()->getCharacterAt(this->x, this->y);
            break;
    }

    if (save != null && save->isMonster()) {
        // Fight monster
        this->battleMonster((Monster*) save);

        // If didn't survive, place back character
        if (!this->isAlive()) {
            this->getFloor()->setCharacterAt(save, save->getX(), save->getY());
        }
    }

    // Restore character
    this->setCharacter(PLAYER_CHARACTER);

    // Place back and restore original fog of war
    this->getFloor()->setCharacterAt(this, this->getX(), this->getY());
    dungeon->getSettings()->setFogOfWar(originalFogOfWar);

    // Update visibility
    this->updateVisibility();

    this->getFloor()->getDungeon()->getOutput()->print();

    return 0;
}

int Player::handleEventKeyStaircase(int command) {
    Dungeon* dungeon = this->getFloor()->getDungeon();
    Floor* floor = this->getFloor();
    Terrain* onTerrain = floor->getTerrainAt(this->x, this->y);
    std::string movingFormat = "Moving %s a staircase";

    // We are standing on a staircase
    if (onTerrain->getCharacter() == STAIRCASE_DOWN_CHARACTER || onTerrain->getCharacter() == STAIRCASE_UP_CHARACTER) {
        if ((command == STAIRCASE_DOWN_CHARACTER && onTerrain->getCharacter() == STAIRCASE_DOWN_CHARACTER) ||
            (command == STAIRCASE_UP_CHARACTER && onTerrain->getCharacter() == STAIRCASE_UP_CHARACTER)) {
            // Trying to take a down staircase, and standing on a down staircase
            // Trying to take an up staircase and standing on an up staircase
            this->takingStaircase = (Staircase*) onTerrain;

            dungeon->prependText(&movingFormat, (this->takingStaircase->isUp() ? "Up" : "Down"));
            return 0;
        }
    }

    dungeon->prependText("Not standing on a valid staircase");
    dungeon->getOutput()->print();
    // Not on staircase, cant take a staircase
    // Or not on a staircase facing the right direction
    return 1;
}

int Player::handleEventKeyMovement(int command) {
    Dungeon* dungeon = this->getFloor()->getDungeon();

    u_char x = dungeon->getPlayer()->getX();
    u_char y = dungeon->getPlayer()->getY();

    std::string movementString;

    switch (command) {
        // Upper Left
        case '7':
        case 'y':
            movementString = "Moving upper left";
            y--;
            x--;
            break;
            // Up
        case '8':
        case 'k':
            movementString = "Moving up";
            y--;
            break;
            // Upper right
        case '9':
        case 'u':
            movementString = "Moving upper right";
            y--;
            x++;
            break;
            // Left
        case '4':
        case 'h':
            movementString = "Moving left";
            x--;
            break;
            // Right
        case '6':
        case 'l':
            movementString = "Moving right";
            x++;
            break;
            // Down left
        case '1':
        case 'b':
            movementString = "Moving down left";
            y++;
            x--;
            break;
            // Down
        case '2':
        case 'j':
            movementString = "Moving down";
            y++;
            break;
            // Down right
        case '3':
        case 'n':
            movementString = "Moving down right";
            y++;
            x++;
            break;
        case 'Q':
            dungeon->getPlayer()->setRequestTerminate(true);
            // Rest for a turn
        case '5':
        case '.':
            movementString = "A day of rest...";
            break;
        default:
            dungeon->prependText("Invalid key entered");
            dungeon->getOutput()->print();
            return 1;
    }

    if (!dungeon->getCurrentFloor()->getTerrainAt(x, y)->isWalkable()) {
        dungeon->prependText("Terrain is not walkable, invalid key entered");
        dungeon->getOutput()->print();
        return 1;
    } else {
        dungeon->prependText(movementString);
    }

    this->moveTo(x, y);

    return 0;
}

int Player::moveTo(u_char toX, u_char toY) {
    Floor* floor = this->getFloor();

    // If moving to the same spot, just exit
    if (toX == this->getX() && toY == this->getY()) {
        return 0;
    }

    // Check out target location
    if (floor->getCharacterAt(toX, toY) != null) {
        this->battleMonster((Monster*) floor->getCharacterAt(toX, toY));

        if (!this->isAlive()) {
            // Player died, don't move to spot
            return 0;
        }
    }

    // Move the character to the requested spot
    floor->setCharacterAt(null, this->x, this->y);

    this->x = toX;
    this->y = toY;

    floor->setCharacterAt(this, this->x, this->y);

    Monster::RunDijkstraOnFloor(floor);

    this->updateVisibility();

    return 0;
}

void Player::battleMonster(Monster* monster) {
    if (Monster::AliveCount(this->floor->getDungeon()) == 1) {
        // Player died
        this->killCharacter();

        // Remove player corps
        this->floor->setCharacterAt(null, this->getX(), this->getY());
    } else {
        // Monster died
        monster->killCharacter();

        // Remove monster corps
        this->floor->setCharacterAt(null, monster->getX(), monster->getY());

        // Add level to the player
        this->addLevel(1);
        this->incrementMonstersSlain();
    }
}

Player* Player::addLevel(int amount) {
    this->level += amount;

    return this;
}

Player* Player::removeLevel(int amount) {
    if (this->level - amount < 0) {
        this->level = 0;
    } else {
        this->level -= amount;
    }

    return this;
}

u_int Player::getColor() {
    return EFD_COLOR_WHITE;
}

Player* Player::updateVisibility() {
    u_char height;
    u_char width;
    u_char minHeight = std::max(1, this->getY() - PLAYER_DEFAULT_LIGHT_RADIUS);
    u_char maxHeight = std::min(DUNGEON_FLOOR_HEIGHT - 1, this->getY() + PLAYER_DEFAULT_LIGHT_RADIUS);
    u_char minWidth = std::max(1, this->getX() - PLAYER_DEFAULT_LIGHT_RADIUS);
    u_char maxWidth = std::min(DUNGEON_FLOOR_WIDTH - 1, this->getX() + PLAYER_DEFAULT_LIGHT_RADIUS);

    for (height = minHeight; height < maxHeight; height++) {
        for (width = minWidth; width < maxWidth; width++) {
            if (this->hasLineOfSightTo(width, height)) {
                this->visibility[height][width] = this->floor->getTerrainAt(width, height);
            }
        }
    }

    return this;
}

Player* Player::incrementMonstersSlain() {
    this->monstersSlain++;

    return this;
}

Player* Player::incrementDaysSurvived() {
    this->daysSurvived++;

    return this;
}

/** GETTERS **/
Staircase* Player::getTakingStaircase() {
    return this->takingStaircase;
}

bool Player::getRequestTerminate() {
    return this->requestTerminate;
}

u_int Player::getLevel() {
    return this->level;
}

u_int Player::getMonstersSlain() {
    return this->monstersSlain;
}

u_int Player::getDaysSurvived() {
    return this->daysSurvived;
}
/** GETTERS **/

/** SETTERS **/
Player* Player::setFloor(Floor* floor) {
    this->floor = floor;

    return this;
}
Player* Player::setTakingStaircase(Staircase* takingStaircase) {
    this->takingStaircase = takingStaircase;

    return this;
}

Player* Player::setRequestTerminate(bool requestTerminate) {
    this->requestTerminate = requestTerminate;

    return this;
}

Player* Player::setLevel(u_int level) {
    this->level = level;

    return this;
}

Player* Player::setMonstersSlain(u_int monstersSlain) {
    this->monstersSlain = monstersSlain;

    return this;
}

Player* Player::setDaysSurvived(u_int daysSurvived) {
    this->daysSurvived = daysSurvived;

    return this;
}
/** SETTERS **/
