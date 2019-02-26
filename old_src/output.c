#include "output.h"

void output(Dungeon* dungeon, void (* targetOutputFunction)(Dungeon*)) {
    if (dungeon->settings->doNCursesPrint) {
        // Clear the current window
        clear();
    }
    // Run the output function
    targetOutputFunction(dungeon);

    print(dungeon->settings->doNCursesPrint, "%s\n", dungeon->textLine1);
    print(dungeon->settings->doNCursesPrint, "%s\n", dungeon->textLine2);
    print(dungeon->settings->doNCursesPrint, "%s\n", dungeon->textLine3);

    if (dungeon->settings->doNCursesPrint) {
        // Refresh the window
        refresh();
    }
}

void output_print_all_floors(Dungeon* dungeon) {
    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        output_print_floor(dungeon, index);
    }
}

void output_print_current_floor(Dungeon* dungeon) {
    output_print_floor(dungeon->floor);
}

void output_print_current_floor_hardness(Dungeon* dungeon) {
    output_print_floor_hardness(dungeon->floor);
}

void output_print_current_floor_tunneler(Dungeon* dungeon) {
    output_print_floor_tunneler_view(dungeon->floor);
}

void output_print_current_floor_non_tunneler(Dungeon* dungeon) {
    output_print_floor_non_tunneler_view(dungeon->floor);
}

void output_print_floor(Floor* floor) {
    u_char width;
    u_char height;
    bool ncurses = floor->dungeon->settings->doNCursesPrint;
    bool expanded = floor->dungeon->settings->expandedPrint;

    if (expanded) {
        print(ncurses, "  ");
        for (width = 0; width < floor->width; width++) {
            print(ncurses, "%3d", width);
        }
        print(ncurses, "\n");
    }

    for (height = 0; height < floor->height; height++) {
        if (expanded) {
            print(ncurses, "%2d ", height);
        }

        for (width = 0; width < floor->width; width++) {
            if (expanded) {
                print(ncurses, " %c ", floor->floorPlan[height][width]->character);
            } else {
                print(ncurses, "%c", floor->floorPlan[height][width]->character);
            }
        }

        print(ncurses, "\n");
    }
}

void output_print_floor_hardness(Floor* floor) {
    u_char width;
    u_char height;
    bool ncurses = floor->dungeon->settings->doNCursesPrint;
    bool expanded = floor->dungeon->settings->expandedPrint;

    if (expanded) {
        print(ncurses, "  ");
        for (width = 0; width < floor->width; width++) {
            print(ncurses, "%3d", width);
        }
        print(ncurses, "\n");
    }

    for (height = 0; height < floor->height; height++) {
        if (expanded) {
            print(ncurses, "%2d ", height);
        }

        for (width = 0; width < floor->width; width++) {
            if (expanded) {
                if (floor->floorPlan[height][width]->type == type_border) {
                    print(ncurses, " %c ", floor->floorPlan[height][width]->character);
                } else {
                    print(ncurses, "%3d", floor->floorPlan[height][width]->hardness);
                }
            } else {
                if (floor->floorPlan[height][width]->type == type_border) {
                    print(ncurses, "%c", floor->floorPlan[height][width]->character);
                } else {
                    print(ncurses, "%d", floor->floorPlan[height][width]->hardness % 10);
                }
            }
        }

        print(ncurses, "\n");
    }
}

void output_print_floor_tunneler_view(Floor* floor) {
    u_char width;
    u_char height;
    bool ncurses = floor->dungeon->settings->doNCursesPrint;
    bool expanded = floor->dungeon->settings->expandedPrint;

    if (expanded) {
        print(ncurses, "  ");
        for (width = 0; width < floor->width; width++) {
            print(ncurses, "%3d", width);
        }
        print(ncurses, "\n");
    }

    for (height = 0; height < floor->height; height++) {
        if (expanded) {
            print(ncurses, "%2d ", height);
        }

        for (width = 0; width < floor->width; width++) {
            if (expanded) {
                if (floor->floorPlan[height][width]->type == type_border) {
                    u_char character = floor->floorPlan[height][width]->character;
                    print(ncurses, " %c ",character);
                } else {
                    print(ncurses, "%3d", floor->tunnelerCost[height][width]);
                }
            } else {
                if (floor->floorPlan[height][width]->type == type_border) {
                    print(ncurses, "%c", floor->floorPlan[height][width]->character);
                } else {
                    print(ncurses, "%d", floor->tunnelerCost[height][width] % 10);
                }
            }
        }

        print(ncurses, "\n");
    }
}

void output_print_floor_non_tunneler_view(Floor* floor) {
    u_char width;
    u_char height;
    bool ncurses = floor->dungeon->settings->doNCursesPrint;
    bool expanded = floor->dungeon->settings->expandedPrint;

    if (expanded) {
        print(ncurses, "  ");
        for (width = 0; width < floor->width; width++) {
            print(ncurses, "%3d", width);
        }
        print(ncurses, "\n");
    }

    for (height = 0; height < floor->height; height++) {
        if (expanded) {
            print(ncurses, "%2d ", height);
        }

        for (width = 0; width < floor->width; width++) {
            if (expanded) {
                if (floor->floorPlan[height][width]->type == type_border) {
                    u_char character = floor->floorPlan[height][width]->character;
                    print(ncurses, " %c ",character);
                } else {
                    print(ncurses, "%3d", floor->nonTunnelerCost[height][width]);
                }
            } else {
                if (floor->floorPlan[height][width]->type == type_border) {
                    print(ncurses, "%c", floor->floorPlan[height][width]->character);
                } else {
                    print(ncurses, "%d", floor->nonTunnelerCost[height][width] % 10);
                }
            }
        }

        print(ncurses, "\n");
    }
}

void output_print_endgame(Dungeon* dungeon) {
    bool ncurses = dungeon->settings->doNCursesPrint;

    if (!dungeon->player->isAlive && monster_count(dungeon) > 0) {
        print(ncurses, "Queue completely empty, terminating the program safely\n");
    } else {
        u_char height;
        for (height = 0; height < FLOOR_HEIGHT; height++) {
            printw("\n");
        }
        print(ncurses, "+----------------+-------+--- PLAYER  STATISTICS -----------------------------+\n");
        print(ncurses, "| Player Level   | %5d |                                                    |\n", dungeon->player->level);
        print(ncurses, "| Days Survived  | %5d |                                                    |\n", dungeon->player->daysSurvived);
        print(ncurses, "| Monsters Slain | %5d |                                                    |\n", dungeon->player->monstersSlain);
        print(ncurses, "| Tiles Explored | %5d |                                                    |\n", dungeon->player->tilesExplored);
        print(ncurses, "| Alive          | %5d |                                                    |\n", dungeon->player->isAlive);
        print(ncurses, "+----------------+-------+--- PLAYER  STATISTICS -----------------------------+\n");
    }
}

void print_error(const bool ncurses, const char* format, ...) {
    if(ncurses) {
        // If ncurses, clear the screen
        clear();
    }

    va_list args;
    va_start(args, fmt);

    if (ncurses) {
        printw(format, args);
        printf("Press any key to continue...\n");

        refresh();

        getch();
    } else {
        printf(format, args);
    }

    va_end(args);
}

void print(const bool ncurses, const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (ncurses) {
        printw(format, args);
    } else {
        printf(format, args);
    }

    va_end(args);
}