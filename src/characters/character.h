#ifndef CHARACTER_H
#define CHARACTER_H

#include "../../include/forward_declarations.h"
#include "../character_listings.h"
#include "../resource.h"

namespace App {
    class Character {
    public:
        Character(Floor* floor, u_char x, u_char y, u_char character, u_char speed);
        ~Character();

        Character* killCharacter();

        /** GETTERS **/
        Floor* getFloor();
        u_char getX();
        u_char getY();
        u_char getCharacter();
        u_char getSpeed();
        bool getIsAlive();
        /** GETTERS **/

        /** SETTERS **/
        Character* setFloor(Floor* floor);
        Character* setX(u_char x);
        Character* setY(u_char y);
        Character* setCharacter(u_char character);
        Character* setSpeed(u_char speed);
        Character* setIsAlive(bool isAlive);
        /** SETTERS **/
    protected:
        Floor* floor;

        u_char x;
        u_char y;
        u_char character;

        u_char speed;
        bool isAlive;

    private:
    };
}

#endif
