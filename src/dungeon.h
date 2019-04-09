#ifndef DUNGEON_H
#define DUNGEON_H

#define DUNGEON_TEXT_LINES 3

#define DUNGEON_FLOORS_MIN 3
#define DUNGEON_FLOORS_MAX 5

#include <string>
#include <vector>
#include <ncurses.h>

#include <forward_declarations.h>
#include <exception.h>
#include <global.h>

#include "save_load.h"

namespace App {
    class Dungeon {

    public:
        Dungeon(int argc, char* argv[]);
        ~Dungeon();

        std::string* prependText(const std::string& text);
        std::string* appendText(const std::string& text);
        std::string* prependText(const std::string* format, ...);
        std::string* appendText(const std::string* format, ...);

        MonsterTemplate* randomMonsterTemplate();
        ObjectTemplate* randomObjectTemplate();

        /** GETTERS **/
        Floor* getFloor(u_char index);
        Floor* getCurrentFloor();
        u_char getFloorCount();
        std::vector<MonsterTemplate*> getMonsterTemplates();
        std::vector<ObjectTemplate*> getObjectTemplates();
        Settings* getSettings();
        EventManager* getEventManager();
        Player* getPlayer();
        WINDOW* getWindow();
        Output* getOutput();
        std::string getText(u_char index);
        /** GETTERS **/

        /** SETTERS **/
        Dungeon* addFloor(Floor* floor, u_char index = U_CHAR_MAX);
        Dungeon* setCurrentFloor(Floor* floor);
        Dungeon* setSettings(Settings* settings);
        Dungeon* setEventManager(EventManager* eventManager);
        Dungeon* setPlayer(Player* player);
        Dungeon* setWindow(WINDOW* window);
        /** SETTERS **/
    protected:

    private:
        std::vector<Floor*> floors;
        std::vector<MonsterTemplate*> monsterTemplates;
        std::vector<ObjectTemplate*> objectTemplates;

        Floor* floor;
        Settings* settings;
        EventManager* eventManager;
        Player* player;
        Output* output;

        WINDOW* window;

        std::string* textLines[DUNGEON_TEXT_LINES];
    };
}

#include "floor.h"
#include "dice.h"
#include "events.h"
#include "settings.h"
#include "output.h"
#include "templates/MonsterTemplate.h"
#include "templates/ObjectTemplate.h"
#include "characters/player.h"

#endif
