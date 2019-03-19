#ifndef ROCK_H
#define ROCK_H

#include "../../include/forward_declarations.h"
#include "../character_listings.h"
#include "../floor.h"
#include "terrain.h"

namespace App {
    class Rock : public Terrain {
    public:
        Rock(Floor* floor, u_short id, u_char x, u_char y, u_char hardness);

    protected:

    private:
    };
}

using App::Floor;
using App::Terrain;
using App::Rock;

#endif
