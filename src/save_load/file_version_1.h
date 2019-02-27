#ifndef SAVE_LOAD_V_1_H
#define SAVE_LOAD_V_1_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../save_load.h"
#include "../character_listings.h"
#include "../dungeon.h"
#include "../floor.h"
#include "../resource.h"

int file_save_1(Dungeon* dungeon);
int file_load_1(Dungeon* dungeon);
int file_save_floor_1(FILE* file, Dungeon* dungeon, Floor* floor);
int file_load_floor_1(FILE* file, Dungeon* dungeon, Floor* floor);

#endif