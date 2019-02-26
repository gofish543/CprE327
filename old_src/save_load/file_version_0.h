#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character_listings.h"
#include "dungeon.h"
#include "floor.h"
#include "resource.h"

int save_to_file(Dungeon* dungeon);
int load_from_file(Dungeon* dungeon);
int load_from_program(Dungeon* dungeon);
int save_floor(FILE* file, Dungeon* dungeon, u_char floorIndex);
int load_floor(FILE* file, Dungeon* dungeon, u_char floorIndex);

void save_error(Dungeon* dungeon);

#endif