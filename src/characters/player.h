#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_SPEED 10

#include "../../include/forward_declarations.h"
#include "../terrains/staircase.h"
#include "../character_listings.h"
#include "../resource.h"
#include "character.h"

namespace App {
    class Player : public Character {
    public:
        Player(Floor* floor, u_char x, u_char y);
        Player(Floor* floor, u_char x, u_char y, u_int level, u_int monstersSlain, u_int daysSurvived);
        ~Player();

        int handleEventKeyMonsterMenu();
        int handleEventKeyToggleFog();
        int handleEventKeyTeleport();
        int handleEventKeyStaircase();
        int handleEventKeyMovement();

        int nextActionTick(Event* event);
        int handleEvent(Event* event);
        int moveTo(u_char toX, u_char toY);
        bool battleMonster(Monster* monster);

        Player* addLevel(int amount);
        Player* removeLevel(int amount);

        Player* incrementMonstersSlain();
        Player* incrementDaysSurvived();

        /** GETTERS **/
        Staircase* getTakingStaircase();
        bool getRequestTerminate();
        u_int getLevel();
        u_int getMonstersSlain();
        u_int getDaysSurvived();
        /** GETTERS **/

        /** SETTERS **/
        Player* setTakingStaircase(Staircase* &takingStaircase);
        Player* setRequestTerminate(bool requestTerminate);
        Player* setLevel(u_int level);
        Player* setMonstersSlain(u_int monstersSlain);
        Player* setDaysSurvived(u_int daysSurvived);
        /** SETTERS **/
    protected:
    private:
        Staircase* takingStaircase;

        bool requestTerminate;

        u_int level;
        u_int monstersSlain;
        u_int daysSurvived;
    };
}

#endif
