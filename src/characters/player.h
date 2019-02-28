#ifndef PLAYER_H
#define PLAYER_H

struct Player;
typedef struct Player Player;

#define PLAYER_SPEED 10

#define player_next_action(player)   (player->character->floor->dungeon->eventManager->tick + ( 1000 / player->speed))

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character.h"
#include "player/action.h"
#include "../character_listings.h"
#include "../dungeon.h"
#include "../floor.h"
#include "../events.h"
#include "../actions.h"
#include "../input.h"

struct Player {
    Character* character;

    u_char speed;
    bool isAlive;
    bool requestTerminate;

    Staircase* takingStaircase;

    // Statistical data
    u_int level;
    u_int monstersSlain;
    u_int daysSurvived;
};

Player* player_initialize(Floor* floor, u_char x, u_char y);
Player* player_terminate(Player* player);

int player_next_tick(Event* event);
int player_event(Event* event);

int player_move_to(Player* player, u_char toX, u_char toY);

bool player_battle(Player* player, Monster* monster);
void player_slain(Player* player);

#endif
