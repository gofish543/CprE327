#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#define FILE_HEADING "RLG327-S2019"

#include "../include/forward_declarations.h"
#include "save_load/file_version_0.h"
#include "save_load/file_version_1.h"
#include "dungeon.h"
#include "floor.h"
#include "character_listings.h"
#include "resource.h"
#include "output.h"
#include <cstdio>
#include <cstdlib>

int save_to_file(Dungeon* dungeon);
int load_from_file(Dungeon* dungeon);

#endif