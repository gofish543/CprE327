#ifndef CHARACTER_ACTION_H
#define CHARACTER_ACTION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../character_listings.h"
#include "../../output.h"

int player_action_monster_menu(Dungeon* dungeon);
int player_action_movement(Dungeon* dungeon, int command);
int player_action_staircase(Dungeon* dungeon, int command);

#endif
