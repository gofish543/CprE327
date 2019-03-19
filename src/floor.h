#ifndef FLOOR_H
#define FLOOR_H

#define FLOOR_ROOMS_MIN 8
#define FLOOR_ROOMS_MAX 8

#define FLOOR_STAIRS_MIN 2
#define FLOOR_STAIRS_MAX 4

#define FLOOR_MONSTERS_MIN 5
#define FLOOR_MONSTERS_MAX 10

#include "../include/forward_declarations.h"
#include "dungeon.h"
#include "resource.h"
#include "character_listings.h"
#include "terrains/terrain.h"
#include "terrains/border.h"
#include "terrains/corridor.h"
#include "terrains/rock.h"
#include "terrains/staircase.h"
#include "terrains/room.h"
#include "characters/character.h"
#include "characters/monster.h"
#include "characters/player.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>

namespace App {
    class Floor {

    public:
        Floor(Dungeon* dungeon, u_char floorNumber, u_short roomCount, u_short stairUpCount, u_short stairDownCount, u_short numberOfMonsters);
        ~Floor();

        Floor* resetTunnelerView();
        Floor* resetNonTunnelerView();
        Floor* resetCheapestPathToPlayer();

        u_char getPrintCharacterAt(u_char width, u_char height);

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
        std::vector<Monster*> getMonsters();
        std::vector<Staircase*> getUpStairs();
        std::vector<Staircase*> getDownStairs();
        std::vector<Room*> getRooms();
        /** GETTERS **/

        /** SETTERS **/
        Floor* setDungeon(Dungeon* dungeon);
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
        Floor* setMonsters(std::vector<Monster*> &monsters);
        Floor* setUpStairs(std::vector<Staircase*> &upStairs);
        Floor* setDownStairs(std::vector<Staircase*> &downStairs);
        Floor* setRooms(std::vector<Room*> &rooms);
        /** SETTERS **/

    protected:

    private:
        Dungeon* dungeon;
        u_char floorNumber;

        u_short roomCount;
        u_short stairUpCount;
        u_short stairDownCount;
        u_short monsterCount;

        Terrain* terrains[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
        Character* characters[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];

        u_char tunnelerView[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
        u_char nonTunnelerView[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
        u_char cheapestPathToPlayer[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];

        std::vector<Monster*> monsters;
        std::vector<Staircase*> upStairs;
        std::vector<Staircase*> downStairs;
        std::vector<Room*> rooms;

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
    };
}

#endif
