#ifndef TERRAIN_H
#define TERRAIN_H

#include "../../include/forward_declarations.h"
#include "character_listings.h"
#include "../resource.h"

namespace App {
    class Terrain {
    public:
        Terrain(Floor* floor, u_short id, u_char x, u_char y);
        ~Terrain();
        /** GETTERS **/
        Floor* getFloor();
        u_short getId();
        u_char getX();
        u_char getY();
        u_char getCharacter();
        u_char getHardness();
        bool isWalkable();
        bool isImmutable();
        bool isRock();
        /** GETTERS **/

        /** SETTERS **/
        Terrain* setFloor(Floor* floor);
        Terrain* setId(u_short id);
        Terrain* setX(u_char x);
        Terrain* setY(u_char y);
        Terrain* setCharacter(u_char character);
        Terrain* setHardness(u_char hardness);
        Terrain* setWalkable(bool walkable);
        Terrain* setImmutable(bool immutable);
        Terrain* setRock(bool rock);
        /** SETTERS **/
    protected:
        Floor* floor;

        u_short id;

        u_char x;
        u_char y;

        u_char character;
        u_char hardness;

        bool walkable;
        bool immutable;
        bool rock;

    private:

    };
}

#endif
