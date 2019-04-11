#ifndef CHARACTER_H
#define CHARACTER_H

#define CHARACTER_SPEED_NUMERATOR 1000

#include <forward_declarations.h>
#include <global.h>
#include <colors.h>

#include "../Resource.h"

namespace App {
    class Character {
    public:
        Character(Floor* floor, u_char x, u_char y, u_char character, u_char speed, bool isPlayer, bool isMonster);
        virtual ~Character();

        Character* killCharacter();
        bool hasLineOfSightTo(u_char width, u_char height);
        virtual u_int getColor();

        /** GETTERS **/
        Floor* getFloor();
        u_char getX();
        u_char getY();
        u_char getCharacter();
        u_char getSpeed();
        bool isAlive();
        bool isPlayer();
        bool isMonster();
        /** GETTERS **/

        /** SETTERS **/
        Character* setX(u_char x);
        Character* setY(u_char y);
        Character* setCharacter(u_char character);
        Character* setSpeed(u_char speed);
        /** SETTERS **/

    protected:
        Floor* floor;

        u_char x;
        u_char y;
        u_char character;

        u_char speed;
        bool alive;

        bool player;
        bool monster;

    private:

    };
}

#endif
