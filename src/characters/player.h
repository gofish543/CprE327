#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_DEFAULT_LIGHT_RADIUS 3

#define PLAYER_SPEED 10

#define PLAYER_MAX_INVENTORY_SIZE 10

#include <algorithm>
#include <map>

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
        int handleEventKeyInventoryMenu();
        int handleEventKeyEquipmentMenu();
        int handleEventKeyWearItem();
        int handleEventKeyTakeOffItem();
        int handleEventKeyDropItem();
        int handleEventKeyDestroyItem();
        int handleEventKeyToggleFog();
        int handleEventKeyTeleport();
        int handleEventKeyStaircase(int command);
        int handleEventKeyMovement(int command);

        Object* wearItem(u_char index);
        Object* unEquipItem(u_char index);

        int moveTo(u_char toX, u_char toY);
        void battleMonster(Monster* monster);

        Player* addLevel(int amount);
        Player* removeLevel(int amount);

        Player* incrementMonstersSlain();
        Player* incrementDaysSurvived();

        u_int getColor();
        u_char getSpeed();
        Player* updateVisibility();

        /** GETTERS **/
        Staircase* getTakingStaircase();
        bool getRequestTerminate();
        u_int getLevel();
        u_int getMonstersSlain();
        u_int getDaysSurvived();
        u_char getInventoryCount();
        Object* getInventoryAt(u_char index);
        std::map<u_int, Object*>* getEquipment();
        /** GETTERS **/

        /** SETTERS **/
        Player* setFloor(Floor* floor);
        Player* setTakingStaircase(Staircase* takingStaircase);
        Player* setRequestTerminate(bool requestTerminate);
        Player* setLevel(u_int level);
        Player* setMonstersSlain(u_int monstersSlain);
        Player* setDaysSurvived(u_int daysSurvived);
        Player* removeFromInventory(u_char index);
        /** SETTERS **/

        Terrain* visibility[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
    protected:

    private:
        Staircase* takingStaircase;

        bool requestTerminate;

        u_int level;
        u_int monstersSlain;
        u_int daysSurvived;

        std::map<u_int, Object*> equipment;
        std::vector<Object*> inventory;
    };
}

#include "../floor.h"
#include "../input.h"
#include "../output.h"
#include "../objects/object.h"

#endif
