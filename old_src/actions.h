#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dungeon.h"
#include "resource.h"
#include "characters/monster.h"
#include "characters/player.h"
#include "output.h"

void actions_battle(Player* player, Monster* monster);

#endif
