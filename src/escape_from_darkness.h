#ifndef ESCAPE_FROM_DARKNESS_H
#define ESCAPE_FROM_DARKNESS_H

#include "../include/forward_declarations.h"
#include "dungeon.h"
#include "output.h"
#include "resource.h"
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <unistd.h>
#include <ncurses.h>

int game_tick(Dungeon* dungeon);

#endif
