#ifndef MONSTER_H
#define MONSTER_H

#define MONSTER_HARDNESS_PER_TURN 85

#define MONSTER_INTELLIGENT     0b000000001u
#define MONSTER_TELEPATHIC      0b000000010u
#define MONSTER_TUNNELER        0b000000100u
#define MONSTER_ERRATIC         0b000001000u
#define MONSTER_PASS            0b000010000u
#define MONSTER_PICKUP          0b000100000u
#define MONSTER_DESTROY         0b001000000u
#define MONSTER_UNIQ            0b010000000u
#define MONSTER_BOSS            0b100000000u

#define MONSTER_MOVEMENT_MASK               0b1111u
#define MONSTER_NON_ERRATIC_MOVEMENT_MASK   0b0111u

#define MONSTER_DIJKSTRA_TYPE_TUNNELER 1
#define MONSTER_DIJKSTRA_TYPE_CHEAPEST_PATH 0
#define MONSTER_DIJKSTRA_TYPE_NON_TUNNELER -1

#include <algorithm>
#include <string>

#include <forward_declarations.h>
#include <exception.h>
#include <global.h>

#include "../vendor/heap.h"
#include "../resource.h"
#include "character.h"

namespace App {
    class Monster : public Character {
    public:
        explicit Monster(Floor* floor, u_char x, u_char y, std::string* name, std::string* description, u_short color, u_char speed, u_short abilities, u_int hitPoints, Dice* attackDamage, u_char symbol, u_char rarity);
        ~Monster();

        static u_int AliveCount(Dungeon* dungeon);
        static int DijkstraCompare(const void* A, const void* B);
        static int RunDijkstraOnFloor(Floor* floor);
        static int RunDijkstra(Floor* floor, char type, u_char costChart[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH]);

        static int NextEventTick(Event* event);
        static int HandleEvent(Event* event);

        int moveTo(u_char toX, u_char toY);
        void battleMonster(Monster* otherMonster);
        void battlePlayer(Player* player);
        char* locationString(char location[19]);

        bool isIntelligent();
        bool isTelepathic();
        bool isTunneler();
        bool isErratic();
        bool isPass();
        bool isPickUp();
        bool isDestroy();
        bool isUnique();
        bool isBoss();

        u_char getMovementValue();
        u_char getNonErraticMovementValue();

        /** GETTERS **/
        std::string getName();
        std::string getDescription();
        u_int getColor();
        u_int getHitPoints();
        Dice* getAttackDamage();
        u_int getAbility();
        u_char getRarity();
        u_char getPlayerLastSpottedX();
        u_char getPlayerLastSpottedY();
        Dice* getDamageDice();
        /** GETTERS **/

        /** SETTERS **/
        Monster* setName(std::string* name);
        Monster* setDescription(std::string* description);
        Monster* setColor(u_short color);
        Monster* setHitPoints(u_int hitPoints);
        Monster* setAttackDamage(Dice* attackDamage);
        Monster* setAbilities(u_char abilities);
        Monster* setRarity(u_char rarity);
        Monster* setPlayerLastSpottedX(u_char playerLastSpottedX);
        Monster* setPlayerLastSpottedY(u_char playerLastSpottedY);
        /** SETTERS **/

        /** MOVEMENT PATTERNS **/
        static void Move0(Monster* monster, u_char* x, u_char* y);
        static void Move1(Monster* monster, u_char* x, u_char* y);
        static void Move2(Monster* monster, u_char* x, u_char* y);
        static void Move3(Monster* monster, u_char* x, u_char* y);
        static void Move4(Monster* monster, u_char* x, u_char* y);
        static void Move5(Monster* monster, u_char* x, u_char* y);
        static void Move6(Monster* monster, u_char* x, u_char* y);
        static void Move7(Monster* monster, u_char* x, u_char* y);
        static void Move8(Monster* monster, u_char* x, u_char* y);
        /** MOVEMENT PATTERNS **/
    protected:

    private:
        u_char playerLastSpottedX;
        u_char playerLastSpottedY;

        std::string name;
        std::string description;
        u_short color;

        u_int hitPoints;
        Dice* attackDamage;

        u_int abilities;
        u_char rarity;
    };

    class MonsterCost {
    public:
        MonsterCost(Floor* floor, u_char x, u_char y, u_char cost);
        ~MonsterCost();

        MonsterCost* addCost(u_char cost);
        MonsterCost* removeCost(u_char cost);

        /** GETTERS **/
        Floor* getFloor();
        u_char getX();
        u_char getY();
        u_char getCost();
        /** GETTERS **/

        /** SETTERS **/
        MonsterCost* setX(u_char x);
        MonsterCost* setY(u_char y);
        MonsterCost* setCost(u_char cost);
        /** SETTERS **/
    protected:

    private:
        Floor* floor;

        u_char x;
        u_char y;
        u_char cost;
    };
}

#include "../floor.h"
#include "../dice.h"

#endif
