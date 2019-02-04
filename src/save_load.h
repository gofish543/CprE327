#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character_listings.h"
#include "dungeon.h"
#include "floor.h"
#include "resource.h"

int save_to_file(void* dungeonPointer);
int load_from_file(void* dungeonPointer);
int save_floor(FILE* file, void* dungeonPointer, u_char floorIndex);
int load_floor(FILE* file, void* dungeonPointer, u_char floorIndex);

void save_error(void* dungeonPointer);
void load_error();

#endif