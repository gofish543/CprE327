#ifndef SAVE_LOAD_V_1_H
#define SAVE_LOAD_V_1_H

#include <cstdio>
#include <cstdlib>

#include <forward_declarations.h>
#include <character_listings.h>

int file_save_1(Dungeon* dungeon);
int file_load_1(Dungeon* dungeon);
int file_save_floor_1(FILE* file, Floor* floor);
int file_load_floor_1(FILE* file, Floor* floor);

#include "../dungeon.h"
#include "../floor.h"
#include "../resource.h"

#endif