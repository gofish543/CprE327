#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <stdbool.h>
#include "character_listings.h"
#include "dungeon.h"
#include "floor.h"

bool dungeon_save_to_file(void* dungeonPointer);
bool dungeon_load_from_file(void* dungeonPointer);
void save_floor(FILE* file, void* dungeonPointer, u_char floorIndex);
void load_floor(FILE* file, void* dungeonPointer, u_char floorIndex);

#endif