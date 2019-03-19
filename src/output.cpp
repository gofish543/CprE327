#include "output.h"

void output(Dungeon* dungeon, void (* targetOutputFunction)(Dungeon*)) {
    if (dungeon->getSettings()->doNCursesPrint()) {
        // Clear the current window
        clear();
    }
    // Run the output function
    targetOutputFunction(dungeon);

    if (targetOutputFunction != output_print_endgame) {
        print(dungeon->getWindow(), dungeon->getSettings()->doNCursesPrint(), "%s\n", dungeon->getText(0).c_str());
        print(dungeon->getWindow(), dungeon->getSettings()->doNCursesPrint(), "%s\n", dungeon->getText(1).c_str());
        print(dungeon->getWindow(), dungeon->getSettings()->doNCursesPrint(), "%s\n", dungeon->getText(2).c_str());
    }

    if (dungeon->getSettings()->doNCursesPrint()) {
        // Refresh the window
        refresh();
    }
}

void output_print_all_floors(Dungeon* dungeon) {
    u_char index;
    for (index = 0; index < dungeon->getFloorCount(); index++) {
        output_print_floor(dungeon->getFloors().at(index));
    }
}

void output_print_current_floor(Dungeon* dungeon) {
    output_print_floor(dungeon->getCurrentFloor());
}

void output_print_current_floor_hardness(Dungeon* dungeon) {
    output_print_floor_hardness(dungeon->getCurrentFloor());
}

void output_print_current_floor_tunneler(Dungeon* dungeon) {
    output_print_floor_tunneler_view(dungeon->getCurrentFloor());
}

void output_print_current_floor_non_tunneler(Dungeon* dungeon) {
    output_print_floor_non_tunneler_view(dungeon->getCurrentFloor());
}

void output_print_current_floor_shortest_path(Dungeon* dungeon) {
    output_print_floor_shortest_path(dungeon->getCurrentFloor());
}

void output_print_current_floor_monster_menu(Dungeon* dungeon, u_short startIndex) {
    output_print_floor_monster_menu(dungeon->getCurrentFloor(), startIndex);
}

void output_print_floor(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->getDungeon();
    bool ncurses = dungeon->getSettings()->doNCursesPrint();
    bool expanded = dungeon->getSettings()->doExpandPrint();

    if (expanded) {
        print(dungeon->getWindow(), ncurses, "  ");
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            print(dungeon->getWindow(), ncurses, "%3d", width);
        }
        print(dungeon->getWindow(), ncurses, "\n");
    }

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->getWindow(), ncurses, "%2d ", height);
        }

        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            if (expanded) {
                print(dungeon->getWindow(), ncurses, " %c ", floor->getPrintCharacterAt(width, height));
            } else {
                print(dungeon->getWindow(), ncurses, "%c", floor->getPrintCharacterAt(width, height));
            }
        }

        print(dungeon->getWindow(), ncurses, "\n");
    }
}

void output_print_floor_hardness(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->getDungeon();
    bool ncurses = dungeon->getSettings()->doNCursesPrint();
    bool expanded = dungeon->getSettings()->doExpandPrint();

    if (expanded) {
        print(dungeon->getWindow(), ncurses, "  ");
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            print(dungeon->getWindow(), ncurses, "%3d", width);
        }
        print(dungeon->getWindow(), ncurses, "\n");
    }

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->getWindow(), ncurses, "%2d ", height);
        }

        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            if (expanded) {
                if (floor->getTerrainAt(width, height)->isImmutable()) {
                    print(dungeon->getWindow(), ncurses, " %c ", floor->getTerrainAt(width, height)->getCharacter());
                } else {
                    print(dungeon->getWindow(), ncurses, "%3d", floor->getTerrainAt(width, height)->getHardness());
                }
            } else {
                if (floor->getTerrainAt(width, height)->isImmutable()) {
                    print(dungeon->getWindow(), ncurses, "%c", floor->getTerrainAt(width, height)->getCharacter());
                } else {
                    print(dungeon->getWindow(), ncurses, "%d", floor->getTerrainAt(width, height)->getHardness() % 10);
                }
            }
        }

        print(dungeon->getWindow(), ncurses, "\n");
    }
}

void output_print_floor_tunneler_view(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->getDungeon();
    bool ncurses = dungeon->getSettings()->doNCursesPrint();
    bool expanded = dungeon->getSettings()->doExpandPrint();

    if (expanded) {
        print(dungeon->getWindow(), ncurses, "  ");
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            print(dungeon->getWindow(), ncurses, "%3d", width);
        }
        print(dungeon->getWindow(), ncurses, "\n");
    }

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->getWindow(), ncurses, "%2d ", height);
        }

        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            if (expanded) {
                if (floor->getTerrainAt(width, height)->isImmutable()) {
                    print(dungeon->getWindow(), ncurses, " %c ", floor->getTerrainAt(width, height)->getCharacter());
                } else {
                    print(dungeon->getWindow(), ncurses, "%3d", floor->getTunnelerViewAt(width, height));
                }
            } else {
                if (floor->getTerrainAt(width, height)->isImmutable()) {
                    print(dungeon->getWindow(), ncurses, "%c", floor->getTerrainAt(width, height)->getCharacter());
                } else {
                    print(dungeon->getWindow(), ncurses, "%d", floor->getTunnelerViewAt(width, height) % 10);
                }
            }
        }

        print(dungeon->getWindow(), ncurses, "\n");
    }
}

void output_print_floor_non_tunneler_view(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->getDungeon();
    bool ncurses = dungeon->getSettings()->doNCursesPrint();
    bool expanded = dungeon->getSettings()->doExpandPrint();

    if (expanded) {
        print(dungeon->getWindow(), ncurses, "  ");
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            print(dungeon->getWindow(), ncurses, "%3d", width);
        }
        print(dungeon->getWindow(), ncurses, "\n");
    }

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->getWindow(), ncurses, "%2d ", height);
        }

        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            if (expanded) {
                if (floor->getTerrainAt(width, height)->isImmutable()) {
                    print(dungeon->getWindow(), ncurses, " %c ", floor->getTerrainAt(width, height)->getCharacter());
                } else if (floor->getTerrainAt(width, height)->isRock()) {
                    print(dungeon->getWindow(), ncurses, "   ");
                } else {
                    print(dungeon->getWindow(), ncurses, "%3d", floor->getTerrainAt(width, height)->getCharacter());
                }
            } else {
                if (floor->getTerrainAt(width, height)->isImmutable()) {
                    print(dungeon->getWindow(), ncurses, "%c", floor->getCharacterAt(width, height));
                } else if (floor->getTerrainAt(width, height)->isRock()) {
                    print(dungeon->getWindow(), ncurses, " ");
                } else {
                    print(dungeon->getWindow(), ncurses, "%d", floor->getNonTunnelerViewAt(width, height) % 10);
                }
            }
        }

        print(dungeon->getWindow(), ncurses, "\n");
    }
}

void output_print_floor_shortest_path(Floor* floor) {
    u_char width;
    u_char height;
    Dungeon* dungeon = floor->getDungeon();
    bool ncurses = dungeon->getSettings()->doNCursesPrint();
    bool expanded = dungeon->getSettings()->doExpandPrint();

    if (expanded) {
        print(dungeon->getWindow(), ncurses, "  ");
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            print(dungeon->getWindow(), ncurses, "%3d", width);
        }
        print(dungeon->getWindow(), ncurses, "\n");
    }

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        if (expanded) {
            print(dungeon->getWindow(), ncurses, "%2d ", height);
        }

        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            if (expanded) {
                if (floor->getTerrainAt(width, height)->isImmutable()) {
                    print(dungeon->getWindow(), ncurses, " %c ", floor->getTerrainAt(width, height)->getCharacter());
                } else {
                    print(dungeon->getWindow(), ncurses, "%3d", floor->getCheapestPathToPlayerAt(width, height));
                }
            } else {
                if (floor->getTerrainAt(width, height)->isImmutable()) {
                    print(dungeon->getWindow(), ncurses, "%c", floor->getTerrainAt(width, height)->getCharacter());
                } else {
                    print(dungeon->getWindow(), ncurses, "%d", floor->getCheapestPathToPlayerAt(width, height) % 10);
                }
            }
        }

        print(dungeon->getWindow(), ncurses, "\n");
    }
}

void output_print_floor_monster_menu(Floor* floor, u_short startIndex) {
    u_char height;
    u_short index;
    WINDOW* window = floor->getDungeon()->getWindow();
    bool ncurses = floor->getDungeon()->getSettings()->doNCursesPrint();

    if (ncurses) {
        clear();
    }

    print(window, ncurses, "+---------+-------------+------------+----------+---------+--------------------+\n");
    print(window, ncurses, "| MONSTER | INTELLIGENT | TELEPATHIC | TUNNELER | ERRATIC |      LOCATION      |\n");
    print(window, ncurses, "+---------+-------------+------------+----------+---------+--------------------+\n");
    for (index = startIndex, height = 0; height < DUNGEON_FLOOR_HEIGHT && index < floor->getMonsterCount(); index++, height++) {
        print(window, ncurses, "| %7d | %11s | %10s | %8s | %7s | %18s |\n",
              index,
              (floor->getMonsters().at(index)->isIntelligent()) ? "YES" : "NO",
              (floor->getMonsters().at(index)->isTelepathic())? "YES" : "NO",
              (floor->getMonsters().at(index)->isTunneler()) ? "YES" : "NO",
              (floor->getMonsters().at(index)->isErratic())? "YES" : "NO",
              floor->getMonsters().at(index)->locationString().c_str()
        );
    }
    print(window, ncurses, "+---------+-------------+------------+----------+---------+--------------------+\n");

    if (ncurses) {
        refresh();
    }
}

void output_print_endgame(Dungeon* dungeon) {
    bool ncurses = dungeon->getSettings()->doNCursesPrint();

    if (ncurses) {
        clear();
    }

    if (dungeon->getPlayer()->getRequestTerminate()) {
        print(dungeon->getWindow(), ncurses, "Thank you for playing, safely exiting\n");
    } else if (dungeon->getPlayer()->getIsAlive() && Monster::MonstersAlive(dungeon) > 0) {
        print(dungeon->getWindow(), ncurses, "Queue completely empty, terminating the program safely\n");
    } else {
        u_char height;
        if (!ncurses) {
            for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
                print(dungeon->getWindow(), ncurses, "\n");
            }
        }

        print(dungeon->getWindow(), ncurses, "+----------------+-------+--- PLAYER  STATISTICS -----------------------------+\n");
        print(dungeon->getWindow(), ncurses, "| Player Level   | %5d |                                                    |\n", dungeon->getPlayer()->getLevel());
        print(dungeon->getWindow(), ncurses, "| Days Survived  | %5d |                                                    |\n", dungeon->getPlayer()->getDaysSurvived());
        print(dungeon->getWindow(), ncurses, "| Monsters Slain | %5d |                                                    |\n", dungeon->getPlayer()->getMonstersSlain());
        print(dungeon->getWindow(), ncurses, "| Alive          | %5d |                                                    |\n", dungeon->getPlayer()->getIsAlive());
        print(dungeon->getWindow(), ncurses, "+----------------+-------+--- PLAYER  STATISTICS -----------------------------+\n");
    }

    if (ncurses) {
        refresh();
        print(dungeon->getWindow(), ncurses, "Press any key to continue...\n");
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
        vw_printw(window, format, args);
        vw_printw(window, "Press any key to continue...\n", null);

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