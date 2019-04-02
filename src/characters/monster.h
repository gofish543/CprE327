#ifndef MONSTER_H
#define MONSTER_H

#define MONSTER_MIN_SPEED 5
#define MONSTER_MAX_SPEED 20

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

#define MONSTER_INTELLIGENT_LEVEL 8
#define MONSTER_TELEPATHIC_LEVEL 4
#define MONSTER_TUNNELER_LEVEL 2
#define MONSTER_ERRATIC_LEVEL 1

#define MONSTER_DIJKSTRA_TYPE_TUNNELER 1
#define MONSTER_DIJKSTRA_TYPE_CHEAPEST_PATH 0
#define MONSTER_DIJKSTRA_TYPE_NON_TUNNELER -1

#include "../../include/forward_declarations.h"
#include "../vendor/heap.h"
#include "../floor.h"
#include "../resource.h"
#include "character.h"
#include <climits>

namespace App {
    class Monster : public Character {
    public:
        Monster(Floor* floor, u_char x, u_char y, u_char classification, u_char speed);
        explicit Monster(std::string monster);
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
        bool hasLineOfSightTo(Player* player);
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

        /** GETTERS **/
        std::string getName();
        std::string getDescription();
        std::string getColor();
        u_int getHitPoints();
        u_int getAttackDamage();
        u_char getClassification();
        u_char getRarity();
        u_char getPlayerLastSpottedX();
        u_char getPlayerLastSpottedY();
        /** GETTERS **/

        /** SETTERS **/
        Monster* setName(std::string* name);
        Monster* setDescription(std::string* description);
        Monster* setColor(std::string* color);
        Monster* setHitPoints(u_int hitPoints);
        Monster* setAttackDamage(u_int attackDamage);
        Monster* setClassification(u_char classification);
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
        static void Move9(Monster* monster, u_char* x, u_char* y);
        static void Move10(Monster* monster, u_char* x, u_char* y);
        static void Move11(Monster* monster, u_char* x, u_char* y);
        static void Move12(Monster* monster, u_char* x, u_char* y);
        static void Move13(Monster* monster, u_char* x, u_char* y);
        static void Move14(Monster* monster, u_char* x, u_char* y);
        static void Move15(Monster* monster, u_char* x, u_char* y);
        /** MOVEMENT PATTERNS **/
    protected:

    private:
        u_char playerLastSpottedX;
        u_char playerLastSpottedY;

        std::string name;
        std::string description;
        std::string color;

        u_int hitPoints;
        u_int attackDamage;

        u_char classification;
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
