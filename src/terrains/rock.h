#ifndef ROCK_H
#define ROCK_H

#include <forward_declarations.h>
#include <global.h>
#include <character_listings.h>

#include "terrain.h"

namespace App {
    class Rock : public Terrain {
    public:
        Rock(Floor* floor, u_short id, u_char x, u_char y, u_char hardness);
        ~Rock();

        /** GETTERS **/
        /** GETTERS **/

        /** SETTERS **/
        /** SETTERS **/

    protected:

    private:

    };
}

#endif
