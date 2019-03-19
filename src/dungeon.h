#ifndef DUNGEON_H
#define DUNGEON_H

#define DUNGEON_TEXT_LINES 3

#define DUNGEON_FLOORS_MIN 3
#define DUNGEON_FLOORS_MAX 5

#define DUNGEON_FLOOR_WIDTH 80
#define DUNGEON_FLOOR_HEIGHT 21

#include "../include/forward_declarations.h"
#include "events.h"
#include "floor.h"
#include "resource.h"
#include "settings.h"
#include <string>
#include <vector>
#include <ncurses.h>

namespace App {
    class Dungeon {

    public:
        Dungeon(int argc, char* argv[]);
        ~Dungeon();

        std::string* prependText(std::string message, ...);
        std::string* appendText(std::string message, ...);

        /** GETTERS **/
        std::vector<Floor*> getFloors();
        Floor* getCurrentFloor();
        Settings* getSettings();
        EventManager* getEventManager();
        Player* getPlayer();
        WINDOW* getWindow();
        u_char getFloorCount();
        std::string* getTextLines();
        std::string getText(u_char index);
        /** GETTERS **/

        /** SETTERS **/
        Dungeon* setFloors(std::vector<Floor*> &floors);
        Dungeon* setCurrentFloor(Floor* floor);
        Dungeon* setSettings(Settings* settings);
        Dungeon* setEventManager(EventManager* eventManager);
        Dungeon* setPlayer(Player* player);
        Dungeon* setWindow(WINDOW* window);
        Dungeon* setFloorCount(u_char floorCount);
        /** SETTERS **/
    protected:

    private:
        std::vector<Floor*> floors;
        Floor* floor;
        Settings* settings;
        EventManager* eventManager;
        Player* player;

        WINDOW* window;

        u_char floorCount;
        std::string textLines[DUNGEON_TEXT_LINES];
    };
}

#endif
