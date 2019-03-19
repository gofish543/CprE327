#include "player.h"

Player::Player(Floor* floor, u_char x, u_char y) : Character(floor, x, y, PLAYER_CHARACTER, PLAYER_SPEED) {
    this->takingStaircase = null;

    this->requestTerminate = false;

    this->level = 0;
    this->monstersSlain = 0;
    this->daysSurvived = 0;
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

int Player::nextActionTick(Event* event) {
    if (this->isAlive) {
        return this->floor->getDungeon()->getEventManager()->getTick() + (1000 / this->speed);
    } else {
        return -1;
    }
}

int Player::handleEventKeyMonsterMenu() {

}

int Player::handleEventKeyToggleFog() {

}

int Player::handleEventKeyTeleport() {

}

int Player::handleEventKeyStaircase() {

}

int Player::handleEventKeyMovement() {

}

int Player::handleEvent(Event* event) {
    int move;
    Dungeon* dungeon = this->getFloor()->getDungeon();

    move = getChar(dungeon->getWindow(), dungeon->getSettings()->doNCursesPrint());

    switch (move) {
        case 'm':
            this->handleEventKeyMonsterMenu();
            return this->handleEvent(event);
        case 'f':
            this->handleEventKeyToggleFog();
            return this->handleEvent(event);
        case 't':
            this->handleEventKeyTeleport();

            return 0;
        case '<':
        case '>':
            this->handleEventKeyStaircase();
            return this->handleEvent(event);
        default:
            if (this->handleEventKeyMovement()) {
                return this->handleEvent(event);
            }
            return 0;
    }
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

        if (!this->isAlive) {
            // Player died, don't move to spot
            return 0;
        }
    }

    // Move the character to the requested spot
    floor->setCharacterAt(null, this->x, this->y);

    this->x = toX;
    this->y = toY;

    floor->setCharacterAt(this, this->x, this->y);

    Monster::RunRijkstraOnFloor(floor);

    return 0;
}

bool Player::battleMonster(App::Monster* monster) {
    if (Monster::AliveCount(this->floor->getDungeon()) == 1) {
        return false;
    } else {
        return true;
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
Player* Player::setTakingStaircase(Staircase*& takingStaircase) {
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
