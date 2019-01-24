#ifndef ESCAPE_FROM_DARKNESS_H
#define ESCAPE_FROM_DARKNESS_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#include "map.h"

void initialize(struct Map** map);
void terminate(struct Map** map);

#endif
