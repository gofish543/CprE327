#ifndef DUNGEON_H
#define DUNGEON_H

#define DUNGEON_TEXT_LINES 3

#define DUNGEON_FLOORS_MIN 3
#define DUNGEON_FLOORS_MAX 5

#define DUNGEON_FLOOR_WIDTH 80
#define DUNGEON_FLOOR_HEIGHT 21

#include "../include/forward_declarations.h"
#include "./templates/MonsterTemplate.h"
#include "events.h"
#include "floor.h"
#include "resource.h"
#include "save_load.h"
#include "settings.h"
#include <climits>
#include <string>
#include <vector>

namespace App {
    class Dungeon {

    public:
        Dungeon(int argc, char* argv[]);
        ~Dungeon();

        std::string* prependText(std::string message, ...);
        std::string* appendText(std::string message, ...);

        /** GETTERS **/
        Floor* getFloor(u_char index);
        std::vector<MonsterTemplate*> getMonsterTemplates();
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
        Dungeon* setCurrentFloor(Floor* floor);
        Dungeon* setSettings(Settings* settings);
        Dungeon* setEventManager(EventManager* eventManager);
        Dungeon* setPlayer(Player* player);
        Dungeon* setWindow(WINDOW* window);
        Dungeon* setFloorCount(u_char floorCount);
        Dungeon* addFloor(Floor* floor, u_char index = UCHAR_MAX);
        /** SETTERS **/
    protected:

    private:
        std::vector<Floor*> floors;
        std::vector<MonsterTemplate*> monsterTemplates;
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
