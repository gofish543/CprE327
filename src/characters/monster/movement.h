#ifndef MONSTER_MOVEMENT_H
#define MONSTER_MOVEMENT_H

#include "../../../include/forward_declarations.h"
#include "../../vendor/heap.h"
#include "../../character_listings.h"
#include "../../resource.h"
#include "../monster.h"
#include <cmath>
#include <climits>

#define EVOLVE_CHANCE 2
#define TELEPORT_CHANCE (EVOLVE_CHANCE + 3)
#define RANDOM_MOVE_CHANCE (TELEPORT_CHANCE + 45)
#define OTHER_CHARACTERISTIC_CHANCE (RANDOM_MOVE_CHANCE + 50)

#endif
