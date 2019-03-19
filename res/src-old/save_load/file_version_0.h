#ifndef SAVE_LOAD_V_0_H
#define SAVE_LOAD_V_0_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../save_load.h"
#include "../character_listings.h"
#include "../dungeon.h"
#include "../floor.h"
#include "../resource.h"

int file_save_0(Dungeon* dungeon);
int file_load_0(Dungeon* dungeon);
int file_save_floor_0(FILE* file, Dungeon* dungeon, Floor* floor);
int file_load_floor_0(FILE* file, Dungeon* dungeon, Floor* floor);

#endif