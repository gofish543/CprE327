#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <cstdio>
#include <cstdlib>

#include <forward_declarations.h>

//#include "save_load/file_version_0.h"
//#include "save_load/file_version_1.h"

int save_to_file(Dungeon* dungeon);
int load_from_file(Dungeon* dungeon);
void save_error(Dungeon* dungeon);

#include "Floor.h"

#endif