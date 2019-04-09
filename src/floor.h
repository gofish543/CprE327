#ifndef FLOOR_H
#define FLOOR_H

#define FLOOR_ROOMS_MIN 4
#define FLOOR_ROOMS_MAX 8

#define FLOOR_STAIRS_MIN 2
#define FLOOR_STAIRS_MAX 4

#define FLOOR_MONSTERS_MIN 5
#define FLOOR_MONSTERS_MAX 10

#include <vector>
#include <string>

#include <forward_declarations.h>
#include <character_listings.h>
#include <exception.h>
#include <global.h>

namespace App {
    class Floor {

    public:
        explicit Floor(Dungeon* dungeon);
        Floor(Dungeon* dungeon, u_char floorNumber, u_short roomCount, u_short stairUpCount, u_short stairDownCount, u_short numberOfMonsters);
        ~Floor();

        Floor* resetTunnelerView();
        Floor* resetNonTunnelerView();
        Floor* resetCheapestPathToPlayer();

        u_char getOutputCharacterAt(u_char x, u_char y);

        u_char tunnelerView[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
        u_char nonTunnelerView[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
        u_char cheapestPathToPlayer[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];

        Terrain* terrains[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
        Character* characters[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];

        /** GETTERS **/
        Dungeon* getDungeon();
        u_char getFloorNumber();
        u_short getRoomCount();
        u_short getStairUpCount();
        u_short getStairDownCount();
        u_short getMonsterCount();
        Terrain* getTerrainAt(u_char width, u_char height);
        Character* getCharacterAt(u_char width, u_char height);
        u_char getTunnelerViewAt(u_char width, u_char height);
        u_char getNonTunnelerViewAt(u_char width, u_char height);
        u_char getCheapestPathToPlayerAt(u_char width, u_char height);
        Monster* getMonster(u_short index);
        Staircase* getUpStair(u_short index);
        Staircase* getDownStair(u_short index);
        Room* getRoom(u_short index);
        /** GETTERS **/

        /** SETTERS **/
        Floor* setFloorNumber(u_char floorNumber);
        Floor* setRoomCount(u_short roomCount);
        Floor* setStairUpCount(u_short stairUpCount);
        Floor* setStairDownCount(u_short stairDownCount);
        Floor* setMonsterCount(u_short monsterCount);
        Floor* setTerrainAt(Terrain* terrain, u_char width, u_char height);
        Floor* setCharacterAt(Character* character, u_char width, u_char height);
        Floor* setTunnelerViewAt(u_char value, u_char width, u_char height);
        Floor* setNonTunnelerViewAt(u_char value, u_char width, u_char height);
        Floor* setCheapestPathToPlayer(u_char value, u_char width, u_char height);
        Floor* setMonster(Monster* monster, u_short index = U_SHORT_MAX);
        Floor* setUpStair(Staircase* staircase, u_short index = U_SHORT_MAX);
        Floor* setDownStair(Staircase* staircase, u_short index = U_SHORT_MAX);
        Floor* setRoom(Room* room, u_short index = U_SHORT_MAX);
        /** SETTERS **/

        // Initialize the class to empty values
        Floor* initializeToNull();

        // Generate class of functions
        Floor* generateBorders();
        Floor* generateRock();
        Floor* generateRooms();
        Floor* generateCorridors();
        Floor* generatePlayer();
        Floor* generateMonsters();

        // Load class of functions
        Floor* loadBorders();
        Floor* loadRock();
        Floor* loadRooms();
        Floor* loadCorridors();
        Floor* loadPlayer();
        Floor* loadMonsters();
    protected:

    private:
        Dungeon* dungeon;
        u_char floorNumber;

        u_short roomCount;
        u_short stairUpCount;
        u_short stairDownCount;
        u_short monsterCount;

        std::vector<Monster*> monsters;
        std::vector<Staircase*> upStairs;
        std::vector<Staircase*> downStairs;
        std::vector<Room*> rooms;
    };
}

#include "dice.h"
#include "terrains/terrain.h"
#include "terrains/border.h"
#include "terrains/corridor.h"
#include "terrains/rock.h"
#include "terrains/staircase.h"
#include "terrains/room.h"
#include "characters/character.h"
#include "characters/monster.h"
#include "characters/player.h"
#include "templates/MonsterTemplate.h"

#endif
