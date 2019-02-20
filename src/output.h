#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dungeon.h"
#include "resource.h"
#include "floor.h"

void output_print_current_floor(Dungeon* dungeon);
void output_print_current_floor_hardness(Dungeon* dungeon);
void output_print_current_floor_tunneler_view(Dungeon* dungeon);
void output_print_current_floor_non_tunneler_view(Dungeon* dungeon);

void output_print_floor(Dungeon* dungeon, u_char floor);
void output_print_floor_hardness(Dungeon* dungeon, u_char floor);
void output_print_floor_tunneler_view(Dungeon* dungeon, u_char floor);
void output_print_floor_non_tunneler_view(Dungeon* dungeon, u_char floor);

void output_print_all_floors(Dungeon* dungeon);

void output_print_endgame(Dungeon* dungeon);

#endif
