#ifndef PLAYER_H
#define PLAYER_H

struct Player;
typedef struct Player Player;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character.h"
#include "../character_listings.h"
#include "../floor.h"
#include "../dungeon.h"
#include "../events.h"
#include "../actions.h"

#define PLAYER_SPEED 10

struct Player {
    Character* character

    u_char speed;
    bool isAlive;

    // Statistical data
    u_int level;
    u_int monstersSlain;
    u_int daysSurvived;
};

Player* player_initialize(Character* character);
Player* player_terminate(Player* player);

int player_event(Event* event);

int player_move_to(Player* player, u_char toFloor, u_char toX, u_char toY);

bool player_battle(Player* player, Monster* monster);

#endif
