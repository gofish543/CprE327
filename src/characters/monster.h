#ifndef MONSTER_H
#define MONSTER_H

#define MONSTER_MIN_SPEED 5
#define MONSTER_MAX_SPEED 20

#define MONSTER_HARDNESS_PER_TURN 85

#define MONSTER_INTELLIGENT_VALUE   0x0001
#define MONSTER_TELEPATHIC_VALUE    0x0002
#define MONSTER_TUNNELER_VALUE      0x0004
#define MONSTER_ERRATIC_VALUE       0x0008

#define MONSTER_INTELLIGENT_LEVEL 8
#define MONSTER_TELEPATHIC_LEVEL 4
#define MONSTER_TUNNELER_LEVEL 2
#define MONSTER_ERRATIC_LEVEL 1

//#define monster_is_intelligent(monster)  (monster->classification & 0x0001)
//#define monster_is_telepathic(monster)   (monster->classification & 0x0002)
//#define monster_is_tunneler(monster)     (monster->classification & 0x0004)
//#define monster_is_erratic(monster)      (monster->classification & 0x0008)
//
//#define monster_next_action(monster)   (monster->character->floor->dungeon->eventManager->tick + ( 1000 / monster->speed))

#include "../../include/forward_declarations.h"
#include "../vendor/heap.h"
#include "../terrains/staircase.h"
#include "../character_listings.h"
#include "../resource.h"

#include "character.h"
#include <climits>

namespace App {
    class Monster : public Character {
    public:
        Monster(Floor* floor, u_char x, u_char y);
        Monster(Floor* floor, u_char x, u_char y, u_char classification, u_char speed);
        ~Monster();

        static int MonstersAlive(Dungeon* dungeon);
        static int DijkstraCompare(const void* A, const void* B);
        static int RunDijkstraOnFloor(Floor* floor);
        static void RunDijkstra(Floor* floor, char type, u_char costChart[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH]);

        int nextActionTick(Event* event);
        int handleEvent(Event* event);
        int moveTo(u_char toX, u_char toY);
        int battleMonster(Monster* otherMonster);
        int battlePlayer(Player* player);
        bool hasLineOfSightTo(Player* player);
        std::string locationString();

        bool isIntelligent();
        bool isTelepathic();
        bool isTunneler();
        bool isErratic();

        /** GETTERS **/
        u_char getClassification();
        u_char getLevel();
        u_char getPlayerLastSpottedX();
        u_char getPlayerLastSpottedY();
        /** GETTERS **/

        /** SETTERS **/
        Monster* setClassification(u_char classification);
        Monster* setLevel(u_char level);
        Monster* setPlayerLastSpottedX(u_char playerLastSpottedX);
        Monster* setPlayerLastSpottedY(u_char playerLastSpottedY);
        /** SETTERS **/

        /** MOVEMENT PATTERNS **/
        void Move0();
        void Move1();
        void Move2();
        void Move3();
        void Move4();
        void Move5();
        void Move6();
        void Move7();
        void Move8();
        void Move9();
        void Move10();
        void Move11();
        void Move12();
        void Move13();
        void Move14();
        void Move15();
        /** MOVEMENT PATTERNS **/
    protected:

    private:
        u_char classification;
        u_char level;

        u_char playerLastSpottedX;
        u_char playerLastSpottedY;
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
        MonsterCost* setFloor(Floor* floor);
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

#endif
