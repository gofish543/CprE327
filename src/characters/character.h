#ifndef CHARACTER_H
#define CHARACTER_H

struct Character;
typedef struct Character Character;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../floor.h"
#include "../resource.h"
#include "monster.h"
#include "player.h"

struct Character {
    Floor* floor;

    Monster* monster;
    Player* player;

    u_char x;
    u_char y;

    u_char character;
};

Character* character_initialize(Floor* floor, Monster* monster, Player* player, u_char x, u_char y);
Character* character_terminate(Character* character);

#endif
