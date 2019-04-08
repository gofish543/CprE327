#include "character.h"

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
