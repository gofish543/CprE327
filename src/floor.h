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
#include "vendor/queue.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>

namespace App {
    class Floor {

    public:
        explicit Floor(Dungeon* dungeon, u_char floorNumber, u_short roomCount, u_short stairUpCount, u_short stairDownCount, u_short numberOfMonsters);
        ~Floor();

        Dungeon* getDungeon();
        u_char getFloorNumber();
        u_short getRoomCount();
        u_short getStairUpCount();
        u_short getStairDownCount();
        u_short getMonsterCount();
        const std::vector<Staircase*> getUpStairs();
        const std::vector<Staircase*> getDownStairs();
        const std::vector<Room*> getRooms();
        Terrain* getTerrainAt(u_char width, u_char height);
        u_char getCharacterAt(u_char width, u_char height);
        u_char getHardnessAt(u_char width, u_char height);

        u_char getTunnelerViewAt(u_char width, u_char height);
        u_char getNonTunnelerViewAt(u_char width, u_char height);
        u_char getCheapestPathToPlayerAt(u_char width, u_char height);

        Floor* setHardnessAt(u_char hardness, u_char width, u_char height);

    protected:
    private:
        Dungeon* dungeon;
        u_char floorNumber;

        u_short roomCount;
        u_short stairUpCount;
        u_short stairDownCount;
        u_short monsterCount;

        Terrain* terrains[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
//        Character* characters[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];

        u_char tunnelerView[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
        u_char nonTunnelerView[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
        u_char cheapestPathToPlayer[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];

//        Monster** monsters;
        std::vector<Staircase*> upStairs;
        std::vector<Staircase*> downStairs;
        std::vector<Room*> rooms;

        Floor* initializeToNull();

        // Generate class of functions
        Floor* generateBorders();
        Floor* generateRock();
        Floor* generateRooms();
        Floor* generateCorridors();
        Floor* generateMonsters();

        // Load class of functions

    };
}

using App::Dungeon;
using App::Floor;
using App::Terrain;

#endif
