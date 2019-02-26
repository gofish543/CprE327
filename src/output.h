#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ncurses.h>
#include "dungeon.h"
#include "resource.h"
#include "floor.h"

void output(Dungeon* dungeon, void (*targetOutputFunction)(Dungeon*));

void output_print_all_floors(Dungeon* dungeon);
void output_print_current_floor(Dungeon* dungeon);
void output_print_current_floor_hardness(Dungeon* dungeon);
void output_print_current_floor_tunneler(Dungeon* dungeon);
void output_print_current_floor_non_tunneler(Dungeon* dungeon);
void output_print_current_floor_shortest_path(Dungeon* dungeon);

void output_print_floor(Floor* floor);
void output_print_floor_hardness(Floor* floor);
void output_print_floor_tunneler_view(Floor* floor);
void output_print_floor_non_tunneler_view(Floor* floor);
void output_print_floor_shortest_path(Floor* floor);

void output_print_endgame(Dungeon* dungeon);

void print_error(WINDOW* window, const bool ncurses, const char* format, ...);
void print(WINDOW* window, const bool ncurses, const char* format, ...);

#endif
