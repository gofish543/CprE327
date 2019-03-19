#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "save_load/file_version_0.h"
#include "save_load/file_version_1.h"
#include "character_listings.h"
#include "dungeon.h"
#include "floor.h"
#include "resource.h"
#include "output.h"

int save_to_file(Dungeon* dungeon);
int load_from_file(Dungeon* dungeon);
int load_from_program(Dungeon* dungeon);

void save_error(Dungeon* dungeon);

#endif