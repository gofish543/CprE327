#ifndef TERRAIN_H
#define TERRAIN_H

#include "../../include/forward_declarations.h"
#include "../character_listings.h"
#include "../resource.h"

namespace App {
    class Floor;

    class Terrain {
    public:
        explicit Terrain(Floor* floor, u_short id, u_char x, u_char y);

        Terrain* setCharacter(u_char character);
        Terrain* setHardness(u_char hardness);

        Floor* getFloor();
        u_short getId();
        u_char getX();
        u_char getY();
        u_char getCharacter();
        u_char getHardness();

        bool isWalkable;
        bool isImmutable;
        bool isRock;
    protected:
        Floor* floor;

        u_short id;

        u_char x;
        u_char y;

        u_char character;
        u_char hardness;
    private:

    };
}

using App::Floor;
using App::Terrain;

#endif
