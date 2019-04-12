#include "character.h"
#include "../floor.h"

Character::Character(Floor* floor, u_char x, u_char y, u_char character, u_char speed, bool isPlayer, bool isMonster) {
    this->floor = floor;
    this->x = x;
    this->y = y;
    this->character = character;
    this->speed = speed;
    this->alive = true;

    this->player = isPlayer;
    this->monster = isMonster;
}

Character::~Character() = default;

Character* Character::killCharacter() {
    this->alive = false;

    return this;
}

bool Character::hasLineOfSightTo(u_char width, u_char height) {
    Floor* floor = this->getFloor();
    double slope;
    double error = 0.0;

    u_char x;
    u_char y;

    u_char x0 = this->getX();
    u_char x1 = width;

    u_char y0 = this->getY();
    u_char y1 = height;

    char deltaX = x1 - x0;
    char deltaY = y1 - y0;

    if (this->isPlayer()) {
        // Player can only see so far
//        Player* player = (Player*) this;
        if (std::sqrt(deltaX * deltaX + deltaY * deltaY) > PLAYER_DEFAULT_LIGHT_RADIUS) {
            return false;
        }
    }

    if (deltaX == 0 && deltaY == 0) {
        // Standing on the point
        return true;
    }

    if (deltaX == 0) {
        // If horizontal line
        x = x0;
        for (y = y0; y != y1; y += get_sign(deltaY)) {
            if (!floor->getTerrainAt(x, y)->isWalkable()) {
                return false;
            }
        }

        return true;
    } else if (deltaY == 0) {
        // If vertical line
        y = y0;
        for (x = x0; x != x1; x += get_sign(deltaX)) {
            if (!floor->getTerrainAt(x, y)->isWalkable()) {
                return false;
            }
        }

        return true;
    } else {
        slope = double(deltaY) / double(deltaX);

        if (slope < 0) {
            // DOWN TO LEFT || UP TO RIGHT //
            if (deltaX > 0) {
                // UP TO RIGHT //
                y = y0;
                for (x = x0; x < x1; x++) {
                    if (!floor->getTerrainAt(x, y)->isWalkable()) {
                        return false;
                    }

                    error += slope;
                    if (error <= -.50) {
                        y--;
                        error += 1.0;
                    }
                }

                // Burn through remaining Y until you reach it
                for (; y > y1; y--) {
                    if (!floor->getTerrainAt(x, y)->isWalkable()) {
                        return false;
                    }
                }

                return true;
            } else {
                // DOWN TO LEFT //
                y = y0;
                for (x = x0; x > x1; x--) {
                    if (!floor->getTerrainAt(x, y)->isWalkable()) {
                        return false;
                    }

                    error += slope;
                    if (error <= -.50) {
                        y++;
                        error += 1.0;
                    }
                }

                // Burn through remaining Y until you reach it
                for (; y < y1; y++) {
                    if (!floor->getTerrainAt(x, y)->isWalkable()) {
                        return false;
                    }
                }

                return true;
            }
        } else {
            // DOWN TO RIGHT || UP TO LEFT //
            if (deltaX > 0) {
                // DOWN TO RIGHT //
                y = y0;
                for (x = x0; x < x1; x++) {
                    if (!floor->getTerrainAt(x, y)->isWalkable()) {
                        return false;
                    }

                    error += slope;
                    if (error >= .50) {
                        y++;
                        error -= 1.0;
                    }
                }

                // Burn through remaining Y until you reach it
                for (; y < y1; y++) {
                    if (!floor->getTerrainAt(x, y)->isWalkable()) {
                        return false;
                    }
                }

                return true;
            } else {
                // UP TO LEFT //
                y = y0;
                for (x = x0; x > x1; x--) {
                    if (!floor->getTerrainAt(x, y)->isWalkable()) {
                        return false;
                    }

                    error += slope;
                    if (error >= .50) {
                        y--;
                        error -= 1.0;
                    }
                }

                // Burn through remaining Y until you reach it
                for (; y > y1; y--) {
                    if (!floor->getTerrainAt(x, y)->isWalkable()) {
                        return false;
                    }
                }

                return true;
            }
        }
    }
}

u_int Character::getColor() {
    return EFD_COLOR_WHITE;
}

/** GETTERS **/
Floor* Character::getFloor() {
    return this->floor;
}

u_char Character::getX() {
    return this->x;
}

u_char Character::getY() {
    return this->y;
}

u_char Character::getCharacter() {
    return this->character;
}

u_char Character::getSpeed() {
    return this->speed;
}

bool Character::isAlive() {
    return this->alive;
}

bool Character::isPlayer() {
    return this->player;
}

bool Character::isMonster() {
    return this->monster;
}
/** GETTERS **/

/** SETTERS **/
Character* Character::setX(u_char x) {
    this->x = x;

    return this;
}

Character* Character::setY(u_char y) {
    this->y = y;

    return this;
}

Character* Character::setCharacter(u_char character) {
    this->character = character;

    return this;
}

Character* Character::setSpeed(u_char speed) {
    this->speed = speed;

    return this;
}
/** SETTERS **/
