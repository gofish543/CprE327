#ifndef SAVE_LOAD_V_0_H
#define SAVE_LOAD_V_0_H

#include "../../include/forward_declarations.h"
#include "../character_listings.h"
#include "../dungeon.h"
#include "../floor.h"
#include "../resource.h"
#include <cstdio>
#include <cstdlib>

int file_save_0(Dungeon* dungeon);
int file_load_0(Dungeon* dungeon);
int file_save_floor_0(FILE* file, Floor* floor);
int file_load_floor_0(FILE* file, Floor* floor);

#endif