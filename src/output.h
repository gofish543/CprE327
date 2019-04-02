#ifndef OUTPUT_H
#define OUTPUT_H

#include "../include/forward_declarations.h"
#include "characters/monster.h"
#include "dungeon.h"
#include "floor.h"
#include "resource.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstdarg>
#include <ncurses.h>

void output(Dungeon* dungeon, void (* targetOutputFunction)(Dungeon*));

void output_print_all_floors(Dungeon* dungeon);
void output_print_current_floor(Dungeon* dungeon);
void output_print_current_floor_hardness(Dungeon* dungeon);
void output_print_current_floor_tunneler(Dungeon* dungeon);
void output_print_current_floor_non_tunneler(Dungeon* dungeon);
void output_print_current_floor_shortest_path(Dungeon* dungeon);
void output_print_current_floor_monster_menu(Dungeon* dungeon, u_short startIndex);

void output_print_floor(Floor* floor);
void output_print_floor_hardness(Floor* floor);
void output_print_floor_tunneler_view(Floor* floor);
void output_print_floor_non_tunneler_view(Floor* floor);
void output_print_floor_shortest_path(Floor* floor);
void output_print_floor_monster_menu(Floor* floor, u_short startIndex);

void output_print_monster_templates(Dungeon* dungeon);

void output_print_endgame(Dungeon* dungeon);

void print_error(WINDOW* window, bool ncurses, const char* format, ...);
void print(WINDOW* window, bool ncurses, const char* format, ...);

#endif
