#ifndef ESCAPE_FROM_DARKNESS_H
#define ESCAPE_FROM_DARKNESS_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "dungeon.h"

void initialize(Dungeon** dungeon);
void terminate(Dungeon** dungeon);

#endif
