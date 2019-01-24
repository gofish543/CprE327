#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include <ncurses.h>

#include "generate_floor.h"
#include "structs.h"

void print_screen(GAME *game);

void print_floor(GAME *game);

void print_text(GAME *game);

#endif
