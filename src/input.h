#ifndef INPUT_H
#define INPUT_H

#include "dungeon.h"
#include "resource.h"
#include "floor.h"
#include "output.h"
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ncurses.h>

int getChar(WINDOW* window, const bool ncurses);

#endif
