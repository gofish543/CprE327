#ifndef STAIRCASE_H
#define STAIRCASE_H

#define STAIRCASE_TYPE_UP 1
#define STAIRCASE_TYPE_DOWN -1

#include "../../include/forward_declarations.h"
#include "../character_listings.h"
#include "../floor.h"
#include "terrain.h"
#include <vector>

namespace App {
    class Staircase : public Terrain {
    public:
        Staircase(Floor* floor, u_short id, u_char x, u_char y, char type);
        ~Staircase();

        Floor* getTargetFloor();
        bool isUp();
        bool isDown();
        int take();

        /** GETTERS **/
        char getType();
        /** GETTERS **/

        /** SETTERS **/
        Staircase* setType(char type);
        /** SETTERS **/
    protected:

    private:
        char type;
    };
}

#endif
