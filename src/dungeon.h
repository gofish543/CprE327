#ifndef DUNGEON_H
#define DUNGEON_H

#define DUNGEON_TEXT_LINES 3

#define DUNGEON_FLOORS_MIN 3
#define DUNGEON_FLOORS_MAX 5

#define DUNGEON_FLOOR_WIDTH 80
#define DUNGEON_FLOOR_HEIGHT 21

#include "../include/forward_declarations.h"
#include "floor.h"
#include "resource.h"
#include "settings.h"
#include <string>
#include <vector>
#include <ncurses.h>

namespace App {
    class Dungeon {

    public:
        explicit Dungeon(int argc, char* argv[]);
        ~Dungeon();

        std::string* prependText(std::string message, ...);
        std::string* appendText(std::string message, ...);

        WINDOW* getWindow();
        Floor* getCurrentFloor();
        std::vector<Floor*> getFloors();
        Settings* getSettings();
        u_char getFloorCount();
        std::string* getTextLines();
        std::string getText(u_char index);

        Dungeon* setCurrentFloor(Floor* floor);

    protected:

    private:
        Settings* settings;
        WINDOW* window;
        Floor* floor;
        std::vector<Floor*> floors;
        u_char floorCount;
        std::string textLines[DUNGEON_TEXT_LINES];
    };
}

using App::Dungeon;
using App::Settings;
using App::Floor;

#endif
