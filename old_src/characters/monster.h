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
#include "../vendor/heap.h"
#include "../character_listings.h"
#include "../floor.h"
#include "../dungeon.h"
#include "../resource.h"
#include "../events.h"
#include "../output.h"
#include "character.h"

#define MONSTER_MIN_SPEED 5
#define MONSTER_MAX_SPEED 20

#define MONSTER_INTELLIGENT_VALUE   0b0001
#define MONSTER_TELEPATHIC_VALUE    0b0010
#define MONSTER_TUNNELER_VALUE      0b0100
#define MONSTER_ERRATIC_VALUE       0b1000

#define monster_is_intelligent(classification)  (classification >> 1)
#define monster_is_telepathic(classification)   (classification >> 2)
#define monster_is_tunneler(classification)     (classification >> 3)
#define monster_is_erratic(classification)      (classification >> 4)

#define monster_next_tick(monster)   (monster->character->floor->dungeon->eventManager->currentTick + ( 1000 / monster->speed))

struct Monster {
    Character* character;

    u_char classification;
    u_char speed;
    bool isAlive;

    u_char** visionTable;
};

struct MonsterCost {
    Floor* floor;

    u_char x;
    u_char y;
    u_char cost;
};

Monster* monster_initialize(Character* character, u_char type, u_char speed);
Monster* monster_terminate(Monster* monster);

int monster_event(Event* event);

int monster_slain(Monster* monster);
int monster_count(Dungeon* dungeon);

int32_t monster_dijkstra_compare(const void* A, const void* B);
int monster_run_dijkstra_on_floor(Floor* floor);
void monster_run_dijkstra(Floor* floor, u_char** costChart);

int monsters_move(Floor* floor);
int monster_move(Monster* monster);
int monster_move_to(Monster* monster, u_char toX, u_char toY);

#endif
