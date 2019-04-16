#ifndef SAVE_LOAD_V_0_H
#define SAVE_LOAD_V_0_H

#include <cstdio>
#include <string>
#include <cstdlib>

#include <forward_declarations.h>
#include <character_listings.h>

int file_save_0(Dungeon* dungeon);
int file_load_0(Dungeon* dungeon);
int file_save_floor_0(FILE* file, Floor* floor);
int file_load_floor_0(FILE* file, Floor* floor);

#include "../dungeon.h"
#include "../floor.h"
#include "../resource.h"

#endif