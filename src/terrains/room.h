#ifndef ROOM_H
#define ROOM_H

#define ROOM_MIN_WIDTH 4
#define ROOM_MAX_WIDTH 8

#define ROOM_MIN_HEIGHT 4
#define ROOM_MAX_HEIGHT 8

#include <forward_declarations.h>
#include <global.h>
#include <character_listings.h>

#include "terrain.h"

namespace App {
    class Room : public Terrain {
    public:
        Room(Floor* floor, u_char x, u_char y, u_char startingX, u_char startingY, u_char width, u_char height);

        u_char randomXInside();
        u_char randomYInside();

        u_char getStartX();
        u_char getStartY();
        u_char getEndX();
        u_char getEndY();

        u_char getArea();

        /** GETTERS **/
        u_char getWidth();
        u_char getHeight();
        u_char getStartingX();
        u_char getStartingY();
        /** GETTERS **/

        /** SETTERS **/
        /** SETTERS */

        /** OPERATORS **/
        /** OPERATORS **/
    protected:

    private:
        u_char width;
        u_char height;
        u_char startingX;
        u_char startingY;
    };
}

#include "../Dice.h"

#endif
