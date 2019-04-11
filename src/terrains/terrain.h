#ifndef TERRAIN_H
#define TERRAIN_H

#define TERRAIN_BORDER      0b00000001u
#define TERRAIN_CORRIDOR    0b00000010u
#define TERRAIN_ROCK        0b00000100u
#define TERRAIN_ROOM        0b00001000u
#define TERRAIN_STAIRCASE   0b00010000u

#include <forward_declarations.h>
#include <character_listings.h>
#include <global.h>

namespace App {
    class Terrain {
    public:
        Terrain(Floor* floor, u_char x, u_char y);
        ~Terrain();

        bool isBorder();
        bool isCorridor();
        bool isRock();
        bool isRoom();
        bool isStaircase();

        bool isImmutable();
        bool isWalkable();

        /** GETTERS **/
        Floor* getFloor();
        u_char getX();
        u_char getY();
        u_char getCharacter();
        u_char getHardness();
        u_char getType();
        /** GETTERS **/

        /** SETTERS **/
        Terrain* setCharacter(u_char character);
        Terrain* setHardness(u_char hardness);
        /** SETTERS **/
    protected:
        Floor* floor;

        u_char x;
        u_char y;

        u_char character;
        u_char hardness;

        u_char type;
    private:

    };
}

#endif
