#ifndef INPUT_H
#define INPUT_H

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ncurses.h>

#include "dungeon.h"
#include "resource.h"
#include "floor.h"
#include "output.h"

int getChar(WINDOW* window, const bool ncurses);

#endif
