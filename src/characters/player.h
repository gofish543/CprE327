#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_DEFAULT_LIGHT_RADIUS 3

#define PLAYER_SPEED 10

#include <algorithm>

#include <forward_declarations.h>
#include <character_listings.h>
#include <global.h>

#include "../resource.h"
#include "character.h"

namespace App {
    class Player : public Character {
    public:
        Player(Floor* floor, u_char x, u_char y);
        Player(Floor* floor, u_char x, u_char y, u_int level, u_int monstersSlain, u_int daysSurvived);
        ~Player();

        static int NextEventTick(Event* event);
        static int HandleEvent(Event* event);

        int handleEventKeyMonsterMenu();
        int handleEventKeyToggleFog();
        int handleEventKeyTeleport();
        int handleEventKeyStaircase(int command);
        int handleEventKeyMovement(int command);

        int moveTo(u_char toX, u_char toY);
        void battleMonster(Monster* monster);

        Player* addLevel(int amount);
        Player* removeLevel(int amount);

        Player* incrementMonstersSlain();
        Player* incrementDaysSurvived();

        u_int getColor();
        Player* updateVisibility();

        /** GETTERS **/
        Staircase* getTakingStaircase();
        bool getRequestTerminate();
        u_int getLevel();
        u_int getMonstersSlain();
        u_int getDaysSurvived();
        /** GETTERS **/

        /** SETTERS **/
        Player* setFloor(Floor* floor);
        Player* setTakingStaircase(Staircase* takingStaircase);
        Player* setRequestTerminate(bool requestTerminate);
        Player* setLevel(u_int level);
        Player* setMonstersSlain(u_int monstersSlain);
        Player* setDaysSurvived(u_int daysSurvived);
        /** SETTERS **/

        Terrain* visibility[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
    protected:

    private:
        Staircase* takingStaircase;

        bool requestTerminate;

        u_int level;
        u_int monstersSlain;
        u_int daysSurvived;
    };
}

#include "character.h"
#include "../floor.h"
#include "../events.h"
#include "../input.h"
#include "../output.h"

#endif
