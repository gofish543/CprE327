#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ncurses.h>
#include "dungeon.h"
#include "resource.h"
#include "floor.h"
#include "output.h"

int getChar(WINDOW* window, const bool ncurses);

#endif
