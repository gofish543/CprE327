#ifndef MONSTER_H
#define MONSTER_H

struct Monster;
typedef struct Monster Monster;

struct MonsterCost;
typedef struct MonsterCost MonsterCost;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "vendor/heap.h"
#include "character_listings.h"
#include "floor.h"
#include "dungeon.h"
#include "resource.h"
#include "events.h"

#define MONSTER_MIN_SPEED 5
#define MONSTER_MAX_SPEED 20

#define MONSTER_INTELLIGENT_VALUE 1
#define MONSTER_TELEPATHIC_VALUE 2
#define MONSTER_TUNNELER_VALUE 4
#define MONSTER_ERRATIC_VALUE 8

struct Monster {
    u_char floor;
    FloorDot* dot;
    FloorDot* standingOn;

    Dungeon* dungeon;

    bool intelligent;
    bool telepathic;
    bool tunneler;
    bool erratic;

    u_char classification;

    u_char speed;

    bool isAlive;
};

struct MonsterCost {
    u_char x;
    u_char y;
    u_char cost;
    Floor* floor;
    bool isTunnel;
};

Monster* monster_initialize(Dungeon* dungeon, u_char x, u_char y, u_char floor, u_char speed, bool intelligent, bool telepathic, bool tunneler, bool erratic);
Monster* monster_terminate(Monster* monster);

int monster_event(Event* event);

Monster* monster_get_at(Floor* floor, u_char x, u_char y);
int monster_slain(Monster* monster);
int monster_count(Dungeon* dungeon);

int monster_run_dijkstra_on_all_floors(Dungeon* dungeon);
int monster_run_dijkstra_on_floor(Floor* floor);

int32_t monster_dijkstra_compare(const void* A, const void* B);
void monster_dijkstra_tunneler(Floor* floor);
void monster_dijkstra_non_tunneler(Floor* floor);

int monster_free_dungeon(Dungeon* dungeon);
int monster_free_floor(Floor* floor);
int monster_free(Monster* monster);

int monster_move_to(Monster* monster, u_char toX, u_char toY);
int monsters_move(Floor* floor);
int monster_move(Floor* floor, Monster* monster);

#endif
