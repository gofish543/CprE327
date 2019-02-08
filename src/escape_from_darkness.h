#ifndef ESCAPE_FROM_DARKNESS_H
#define ESCAPE_FROM_DARKNESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "dungeon.h"
#include "settings.h"
#include "resource.h"
#include "monster.h"

int initialize(Dungeon** dungeon, int argc, char* argv[]);
int terminate(Dungeon** dungeon);

void game_tick(Dungeon* dungeon);

#endif
