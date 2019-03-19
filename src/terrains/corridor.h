#ifndef CORRIDOR_H
#define CORRIDOR_H

#include "../../include/forward_declarations.h"
#include "../character_listings.h"
#include "../floor.h"
#include "terrain.h"

namespace App {
    class Corridor : public Terrain {
    public:
        Corridor(Floor* floor, u_short id, u_char x, u_char y);

    protected:

    private:

    };
}

using App::Floor;
using App::Terrain;
using App::Corridor;

#endif