#ifndef STAIRCASE_H
#define STAIRCASE_H

#define STAIRCASE_DIRECTION_UP      1
#define STAIRCASE_DIRECTION_DOWN    -1

#include <vector>

#include <forward_declarations.h>
#include <global.h>
#include <character_listings.h>

#include "terrain.h"

namespace App {
    class Staircase : public Terrain {
    public:
        Staircase(Floor* floor, u_char x, u_char y, u_char index, char direction);
        ~Staircase();

        Floor* getTargetFloor();
        bool isUp();
        bool isDown();
        int take();

        /** GETTERS **/
        char getDirection();
        /** GETTERS **/

        /** SETTERS **/
        /** SETTERS **/
    protected:

    private:
        u_char index;

        char direction;
    };
}

#include "../dungeon.h"

#endif
