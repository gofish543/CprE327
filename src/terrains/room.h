#ifndef ROOM_H
#define ROOM_H

#define ROOM_MIN_WIDTH 4
#define ROOM_MAX_WIDTH 8

#define ROOM_MIN_HEIGHT 4
#define ROOM_MAX_HEIGHT 8

#include "../../include/forward_declarations.h"
#include "character_listings.h"
#include "../floor.h"
#include "terrain.h"

namespace App {
    class Room : public Terrain {
    public:
        Room(Floor* floor, u_short id, u_char x, u_char y, u_char startingX, u_char startingY, u_char width, u_char height);

        /** GETTERS **/
        u_char getWidth();
        u_char getHeight();
        u_char getStartingX();
        u_char getStartingY();
        /** GETTERS **/

        /** SETTERS **/
        Room* setWidth(u_char width);
        Room* setHeight(u_char height);
        Room* setStartingX(u_char startingX);
        Room* setStartingY(u_char startingY);
        /** SETTERS */

    protected:

    private:
        u_char width;
        u_char height;
        u_char startingX;
        u_char startingY;
    };
}

#endif
