#ifndef MONSTER_MOVEMENT_H
#define MONSTER_MOVEMENT_H

#include <algorithm>

#include <forward_declarations.h>
#include <global.h>

#include "../monster.h"
#include "../../Resource.h"

#define TELEPORT_CHANCE                 5
#define RANDOM_MOVE_CHANCE              (TELEPORT_CHANCE + 45)
#define OTHER_CHARACTERISTIC_CHANCE     (RANDOM_MOVE_CHANCE + 50)

#endif
