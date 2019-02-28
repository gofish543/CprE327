#include "output.h"

void output(Dungeon* dungeon, void (* targetOutputFunction)(Dungeon*)) {
    if (dungeon->settings->doNCursesPrint) {
        // Clear the current window
        clear();
    }
    // Run the output function
    targetOutputFunction(dungeon);

    if (targetOutputFunction != output_print_endgame) {
        print(dungeon->window, dungeon->settings->doNCursesPrint, "%s\n", dungeon->textLine1);
        print(dungeon->window, dungeon->settings->doNCursesPrint, "%s\n", dungeon->textLine2);
        print(dungeon->window, dungeon->settings->doNCursesPrint, "%s\n", dungeon->textLine3);
    }

    if (dungeon->settings->doNCursesPrint) {
        // Refresh the window
        refresh();
    }
}

void output_print_all_floors(Dungeon* dungeon) {
    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        output_print_floor(dungeon->floors[index]);
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

void output_print_current_floor_shortest_path(Dungeon* dungeon) {
    output_print_floor_shortest_path(dungeon->floor);
}

void output_print_current_floor_monster_menu(Dungeon* dungeon, u_short startIndex) {
    output_print_floor_monster_menu(dungeon->floor, startIndex);
}

void output_print_floor(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->dungeon;
    bool ncurses = dungeon->settings->doNCursesPrint;
    bool expanded = dungeon->settings->expandedPrint;

    if (expanded) {
        print(dungeon->window, ncurses, "  ");
        for (width = 0; width < FLOOR_WIDTH; width++) {
            print(dungeon->window, ncurses, "%3d", width);
        }
        print(dungeon->window, ncurses, "\n");
    }

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->window, ncurses, "%2d ", height);
        }

        for (width = 0; width < FLOOR_WIDTH; width++) {
            if (expanded) {
                print(dungeon->window, ncurses, " %c ", floor_character_at(floor, width, height));
            } else {
                print(dungeon->window, ncurses, "%c", floor_character_at(floor, width, height));
            }
        }

        print(dungeon->window, ncurses, "\n");
    }
}

void output_print_floor_hardness(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->dungeon;
    bool ncurses = dungeon->settings->doNCursesPrint;
    bool expanded = dungeon->settings->expandedPrint;

    if (expanded) {
        print(dungeon->window, ncurses, "  ");
        for (width = 0; width < FLOOR_WIDTH; width++) {
            print(dungeon->window, ncurses, "%3d", width);
        }
        print(dungeon->window, ncurses, "\n");
    }

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->window, ncurses, "%2d ", height);
        }

        for (width = 0; width < FLOOR_WIDTH; width++) {
            if (expanded) {
                if (floor->terrains[height][width]->isImmutable) {
                    print(dungeon->window, ncurses, " %c ", floor_character_at(floor, width, height));
                } else {
                    print(dungeon->window, ncurses, "%3d", floor->terrains[height][width]->hardness);
                }
            } else {
                if (floor->terrains[height][width]->isImmutable) {
                    print(dungeon->window, ncurses, "%c", floor_character_at(floor, width, height));
                } else {
                    print(dungeon->window, ncurses, "%d", floor->terrains[height][width]->hardness % 10);
                }
            }
        }

        print(dungeon->window, ncurses, "\n");
    }
}

void output_print_floor_tunneler_view(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->dungeon;
    bool ncurses = dungeon->settings->doNCursesPrint;
    bool expanded = dungeon->settings->expandedPrint;

    if (expanded) {
        print(dungeon->window, ncurses, "  ");
        for (width = 0; width < FLOOR_WIDTH; width++) {
            print(dungeon->window, ncurses, "%3d", width);
        }
        print(dungeon->window, ncurses, "\n");
    }

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->window, ncurses, "%2d ", height);
        }

        for (width = 0; width < FLOOR_WIDTH; width++) {
            if (expanded) {
                if (floor->terrains[height][width]->isImmutable) {
                    print(dungeon->window, ncurses, " %c ", floor_character_at(floor, width, height));
                } else {
                    print(dungeon->window, ncurses, "%3d", floor->tunnelerView[height][width]);
                }
            } else {
                if (floor->terrains[height][width]->isImmutable) {
                    print(dungeon->window, ncurses, "%c", floor_character_at(floor, width, height));
                } else {
                    print(dungeon->window, ncurses, "%d", floor->tunnelerView[height][width] % 10);
                }
            }
        }

        print(dungeon->window, ncurses, "\n");
    }
}

void output_print_floor_non_tunneler_view(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->dungeon;
    bool ncurses = dungeon->settings->doNCursesPrint;
    bool expanded = dungeon->settings->expandedPrint;

    if (expanded) {
        print(dungeon->window, ncurses, "  ");
        for (width = 0; width < FLOOR_WIDTH; width++) {
            print(dungeon->window, ncurses, "%3d", width);
        }
        print(dungeon->window, ncurses, "\n");
    }

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->window, ncurses, "%2d ", height);
        }

        for (width = 0; width < FLOOR_WIDTH; width++) {
            if (expanded) {
                if (floor->terrains[height][width]->isImmutable) {
                    print(dungeon->window, ncurses, " %c ", floor_character_at(floor, width, height));
                } else if (floor->terrains[height][width]->isRock) {
                    print(dungeon->window, ncurses, "   ");
                } else {
                    print(dungeon->window, ncurses, "%3d", floor->nonTunnelerView[height][width]);
                }
            } else {
                if (floor->terrains[height][width]->isImmutable) {
                    print(dungeon->window, ncurses, "%c", floor_character_at(floor, width, height));
                } else if (floor->terrains[height][width]->isRock) {
                    print(dungeon->window, ncurses, " ");
                } else {
                    print(dungeon->window, ncurses, "%d", floor->nonTunnelerView[height][width] % 10);
                }
            }
        }

        print(dungeon->window, ncurses, "\n");
    }
}

void output_print_floor_shortest_path(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->dungeon;
    bool ncurses = dungeon->settings->doNCursesPrint;
    bool expanded = dungeon->settings->expandedPrint;

    if (expanded) {
        print(dungeon->window, ncurses, "  ");
        for (width = 0; width < FLOOR_WIDTH; width++) {
            print(dungeon->window, ncurses, "%3d", width);
        }
        print(dungeon->window, ncurses, "\n");
    }

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->window, ncurses, "%2d ", height);
        }

        for (width = 0; width < FLOOR_WIDTH; width++) {
            if (expanded) {
                if (floor->terrains[height][width]->isImmutable) {
                    print(dungeon->window, ncurses, " %c ", floor_character_at(floor, width, height));
                } else {
                    print(dungeon->window, ncurses, "%3d", floor->cheapestPathToPlayer[height][width]);
                }
            } else {
                if (floor->terrains[height][width]->isImmutable) {
                    print(dungeon->window, ncurses, "%c", floor_character_at(floor, width, height));
                } else {
                    print(dungeon->window, ncurses, "%d", floor->cheapestPathToPlayer[height][width] % 10);
                }
            }
        }

        print(dungeon->window, ncurses, "\n");
    }
}

void output_print_floor_monster_menu(Floor* floor, u_short startIndex) {
    u_char height;
    u_short index;
    char locationBuffer[19] = "";
    WINDOW* window = floor->dungeon->window;
    bool ncurses = floor->dungeon->settings->doNCursesPrint;

    if(ncurses) {
        clear();
    }

    print(window, ncurses, "+---------+-------------+------------+----------+---------+--------------------+\n");
    print(window, ncurses, "| MONSTER | INTELLIGENT | TELEPATHIC | TUNNELER | ERRATIC |      LOCATION      |\n");
    print(window, ncurses, "+---------+-------------+------------+----------+---------+--------------------+\n");
    for (index = startIndex, height = 0; height < FLOOR_HEIGHT && index < floor->monsterCount; index++, height++) {
        print(window, ncurses, "| %7d | %11s | %10s | %8s | %7s | %18s |\n",
              index,
              monster_is_intelligent(floor->monsters[index]) ? "YES" : "NO",
              monster_is_telepathic(floor->monsters[index]) ? "YES" : "NO",
              monster_is_tunneler(floor->monsters[index]) ? "YES" : "NO",
              monster_is_erratic(floor->monsters[index]) ? "YES" : "NO",
              monster_location_string(floor->monsters[index], locationBuffer)
        );
    }
    print(window, ncurses, "+---------+-------------+------------+----------+---------+--------------------+\n");

    if(ncurses) {
        refresh();
    }
}

void output_print_endgame(Dungeon* dungeon) {
    bool ncurses = dungeon->settings->doNCursesPrint;

    if (ncurses) {
        clear();
    }

    if (dungeon->player->requestTerminate) {
        print(dungeon->window, ncurses, "Thank you for playing, safely exiting\n");
    } else if (dungeon->player->isAlive && monster_alive_count(dungeon) > 0) {
        print(dungeon->window, ncurses, "Queue completely empty, terminating the program safely\n");
    } else {
        u_char height;
        if (!ncurses) {
            for (height = 0; height < FLOOR_HEIGHT; height++) {
                print(dungeon->window, ncurses, "\n");
            }
        }

        print(dungeon->window, ncurses, "+----------------+-------+--- PLAYER  STATISTICS -----------------------------+\n");
        print(dungeon->window, ncurses, "| Player Level   | %5d |                                                    |\n", dungeon->player->level);
        print(dungeon->window, ncurses, "| Days Survived  | %5d |                                                    |\n", dungeon->player->daysSurvived);
        print(dungeon->window, ncurses, "| Monsters Slain | %5d |                                                    |\n", dungeon->player->monstersSlain);
        print(dungeon->window, ncurses, "| Alive          | %5d |                                                    |\n", dungeon->player->isAlive);
        print(dungeon->window, ncurses, "+----------------+-------+--- PLAYER  STATISTICS -----------------------------+\n");
    }

    if (ncurses) {
        refresh();
        print(dungeon->window, ncurses, "Press any key to continue...\n");
        getch();
    }

}

void print_error(WINDOW* window, const bool ncurses, const char* format, ...) {
    if (ncurses) {
        // If ncurses, clear the screen
        clear();
    }

    va_list args;
    va_start(args, format);

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

void print(WINDOW* window, const bool ncurses, const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (ncurses) {
        vw_printw(window, format, args);
    } else {
        vprintf(format, args);
    }

    va_end(args);
}