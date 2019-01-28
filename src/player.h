#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character_listings.h"

typedef struct {
    u_char x;
    u_char y;
    u_char floor;
} Player;

Player* player_initialize(u_char x, u_char y, u_char floor);
Player* player_terminate(Player* player);

#endif
