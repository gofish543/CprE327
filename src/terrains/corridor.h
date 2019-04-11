#ifndef CORRIDOR_H
#define CORRIDOR_H

#include <forward_declarations.h>
#include <global.h>
#include <character_listings.h>

#include "terrain.h"

namespace App {
    class Corridor : public Terrain {
    public:
        Corridor(Floor* floor, u_char x, u_char y);
        ~Corridor();

        /** GETTERS **/
        /** GETTERS **/

        /** SETTERS **/
        /** SETTERS **/
    protected:

    private:

    };
}

#endif
