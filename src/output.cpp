#include "output.h"

Output::Output(Dungeon* dungeon) {
    this->doNCurses = dungeon->getSettings()->doNCursesPrint();
    this->dontNCurses = !this->doNCurses;
    this->doExpanded = dungeon->getSettings()->doExpandPrint();
    this->window = dungeon->getWindow();
    this->dungeon = dungeon;
}

Output::~Output() = default;

Output* Output::print() {
    // Ncurses clear window
    if (this->doNCurses) {
        clear();
    }

    u_char y;
    u_char x;
    u_char index;
    Floor* floor = dungeon->getCurrentFloor();

    if (this->doExpanded) {
        this->print("   ");
        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            this->print("%3d", x);
        }
        this->print("\n");
    }

    for (y = 0; y < DUNGEON_FLOOR_HEIGHT; y++) {
        if (this->doExpanded) {
            this->print("%2d ", y);
        }
        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            if (this->doExpanded) {
                this->print(" %c ", floor->getOutputCharacterAt(x, y));
            } else {
                this->print("%c", floor->getOutputCharacterAt(x, y));
            }
        }
        this->print("\n");
    }

    for (index = 0; index < DUNGEON_TEXT_LINES; index++) {
        this->print(this->dungeon->getText(index));
    }

    // Ncurses refresh
    if (this->doNCurses) {
        refresh();
    }

    return this;
}

Output* Output::print(std::string* format, ...) {
    va_list args;
    va_start(args, format);

    if (this->doNCurses) {
        vw_printw(window, format->c_str(), args);
    } else {
        vprintf(format->c_str(), args);
    }

    va_end(args);

    return this;
}

Output* Output::print(const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (this->doNCurses) {
        vw_printw(window, format, args);
    } else {
        vprintf(format, args);
    }

    va_end(args);

    return this;
}

Output* Output::print(u_int debugFunctions, Floor* floor) {
    if (floor == null) {
        floor = dungeon->getCurrentFloor();
    }

    if (this->doNCurses) {
        clear();
    }

    if (debugFunctions & OUTPUT_DEBUG_HARDNESS) {
        this->printHardness(floor);
    }

    if (debugFunctions & OUTPUT_DEBUG_TUNNELER) {
        this->printTunneler(floor);
    }

    if (debugFunctions & OUTPUT_DEBUG_NON_TUNNELER) {
        this->printNonTunneler(floor);
    }

    if (debugFunctions & OUTPUT_DEBUG_SHORTEST_PATH) {
        this->printShortestPath(floor);
    }

    if (debugFunctions & OUTPUT_DEBUG_MONSTER_TEMPLATES) {
        this->printMonsterTemplates();
    }

    if (debugFunctions * OUTPUT_DEBUG_COLOR_PALLET) {
        this->printColorPallet();
    }

    if (debugFunctions & OUTPUT_DEBUG_TERMINATE) {
        this->printTerminate();
    }

    if (this->doNCurses) {
        refresh();
    }

    return this;
}

Output* Output::printEndgame() {
    if (this->doNCurses) {
        clear();
    }

    if (this->dungeon->getPlayer()->getRequestTerminate()) {
        this->print("Thank you for playing, safely exiting\n");
    } else if (dungeon->getPlayer()->getIsAlive() && Monster::AliveCount(dungeon) > 0) {
        this->print("Queue completely empty, terminating the program safely\n");
    } else {
        u_char y;

        if (this->dontNCurses) {
            for (y = 0; y < DUNGEON_FLOOR_HEIGHT; y++) {
                print("\n");
            }
        }

        print("+----------------+-------+--- PLAYER  STATISTICS -----------------------------+\n");
        print("| Player Level   | %5d |                                                    |\n", this->dungeon->getPlayer()->getLevel());
        print("| Days Survived  | %5d |                                                    |\n", this->dungeon->getPlayer()->getDaysSurvived());
        print("| Monsters Slain | %5d |                                                    |\n", this->dungeon->getPlayer()->getMonstersSlain());
        print("| Alive          | %5d |                                                    |\n", this->dungeon->getPlayer()->getIsAlive());
        print("+----------------+-------+--- PLAYER  STATISTICS -----------------------------+\n");
    }

    if (this->doNCurses) {
        refresh();
        print("Press any key to continue...\n");
        getch();
    }

    return this;
}

Output* Output::printMonsterMenu(u_short startIndex) {
    u_char y;
    u_short index;
    char location[19] = "";

    if (this->doNCurses) {
        clear();
    }

    this->print("+---------+-------------+------------+----------+---------+--------------------+\n");
    this->print("| MONSTER | INTELLIGENT | TELEPATHIC | TUNNELER | ERRATIC |      LOCATION      |\n");
    this->print("+---------+-------------+------------+----------+---------+--------------------+\n");
    for (index = startIndex, y = 0; y < DUNGEON_FLOOR_HEIGHT && index < dungeon->getCurrentFloor()->getMonsterCount(); index++, y++) {
        this->print("| %7d | %11s | %10s | %8s | %7s | %18s |\n",
                    index,
                    (dungeon->getCurrentFloor()->getMonster(index)->isIntelligent()) ? "YES" : "NO",
                    (dungeon->getCurrentFloor()->getMonster(index)->isTelepathic()) ? "YES" : "NO",
                    (dungeon->getCurrentFloor()->getMonster(index)->isTunneler()) ? "YES" : "NO",
                    (dungeon->getCurrentFloor()->getMonster(index)->isErratic()) ? "YES" : "NO",
                    dungeon->getCurrentFloor()->getMonster(index)->locationString(location)
        );
    }
    this->print("+---------+-------------+------------+----------+---------+--------------------+\n");

    if (this->doNCurses) {
        refresh();
    }

    return this;
}

Output* Output::printHardness(Floor* floor) {
    u_char x;
    u_char y;
    Terrain* terrain;

    if (this->doExpanded) {
        this->print("   ");
        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            this->print("%3d", x);
        }
        this->print("\n");
    }

    for (y = 0; y < DUNGEON_FLOOR_HEIGHT; y++) {
        if (this->doExpanded) {
            this->print("%2d ", y);
        }

        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            terrain = floor->getTerrainAt(x, y);

            if (terrain->isImmutable()) {
                if (this->doExpanded) {
                    this->print(" %c ", terrain->getCharacter());
                } else {
                    this->print("%c", terrain->getCharacter());
                }
            } else {
                if (this->doExpanded) {
                    this->print("%3d", terrain->getHardness());
                } else {
                    this->print("%d", terrain->getHardness() % 10);
                }
            }
        }

        this->print("\n");
    }

    return this;
}

Output* Output::printTunneler(Floor* floor) {
    u_char x;
    u_char y;
    Terrain* terrain;

    if (this->doExpanded) {
        this->print("   ");
        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            this->print("%3d", x);
        }
        this->print("\n");
    }

    for (y = 0; y < DUNGEON_FLOOR_HEIGHT; y++) {
        if (this->doExpanded) {
            this->print("%2d ", y);
        }

        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            terrain = floor->getTerrainAt(x, y);

            if (terrain->isImmutable()) {
                if (this->doExpanded) {
                    this->print(" %c ", terrain->getCharacter());
                } else {
                    this->print("%c", terrain->getCharacter());
                }
            } else {
                if (this->doExpanded) {
                    this->print("%3d", floor->getTunnelerViewAt(x, y));
                } else {
                    this->print("%d", floor->getTunnelerViewAt(x, y) % 10);
                }
            }
        }

        this->print("\n");
    }

    return this;
}

Output* Output::printNonTunneler(Floor* floor) {
    u_char x;
    u_char y;
    Terrain* terrain;

    if (this->doExpanded) {
        this->print("   ");
        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            this->print("%3d", x);
        }
        this->print("\n");
    }

    for (y = 0; y < DUNGEON_FLOOR_HEIGHT; y++) {
        if (this->doExpanded) {
            this->print("%2d ", y);
        }

        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            terrain = floor->getTerrainAt(x, y);

            if (terrain->isImmutable()) {
                if (this->doExpanded) {
                    this->print(" %c ", terrain->getCharacter());
                } else {
                    this->print("%c", terrain->getCharacter());
                }
            } else {
                if (this->doExpanded) {
                    this->print("%3d", floor->getNonTunnelerViewAt(x, y));
                } else {
                    this->print("%d", floor->getNonTunnelerViewAt(x, y) % 10);
                }
            }
        }

        this->print("\n");
    }

    return this;
}

Output* Output::printShortestPath(Floor* floor) {
    u_char x;
    u_char y;
    Terrain* terrain;

    if (this->doExpanded) {
        this->print("   ");
        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            this->print("%3d", x);
        }
        this->print("\n");
    }

    for (y = 0; y < DUNGEON_FLOOR_HEIGHT; y++) {
        if (this->doExpanded) {
            this->print("%2d ", y);
        }

        for (x = 0; x < DUNGEON_FLOOR_WIDTH; x++) {
            terrain = floor->getTerrainAt(x, y);

            if (terrain->isImmutable()) {
                if (this->doExpanded) {
                    this->print(" %c ", terrain->getCharacter());
                } else {
                    this->print("%c", terrain->getCharacter());
                }
            } else {
                if (this->doExpanded) {
                    this->print("%3d", floor->getCheapestPathToPlayerAt(x, y));
                } else {
                    this->print("%d", floor->getCheapestPathToPlayerAt(x, y) % 10);
                }
            }
        }

        this->print("\n");
    }

    return this;
}

Output* Output::printMonsterTemplates() {
    for (auto const& monsterTemplate: dungeon->getMonsterTemplates()) {
        this->print("Parsed Monster %s\n", monsterTemplate->getName().c_str());
        this->print("\tDescription: %s\n", monsterTemplate->getDescription().c_str());
        this->print("\tColor: %d\n", monsterTemplate->getColor());
        this->print("\tSpeed: %d\n", monsterTemplate->getSpeed());
        this->print("\tAbilities: %d\n", monsterTemplate->getAbilities());
        this->print("\tHit Points: %d\n", monsterTemplate->getHitPoints());
        this->print("\tAttack Damage: %d\n", monsterTemplate->getAttackDamage());
        this->print("\tSymbol: %c\n", monsterTemplate->getSymbol());
        this->print("\tRarity: %d\n", monsterTemplate->getRarity());
        this->print("\n\n");
    }

    return this;
}

Output* Output::printColorPallet() {
    if (this->doNCurses) {

    } else {
        printf(SHELL_BG_BLACK "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_GREEN "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_CYAN "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_BLUE "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_RED "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_PURPLE "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_YELLOW "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_LIGHT_GREY "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_DARK_GREY "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_LIGHT_BLUE "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_LIGHT_GREEN "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_LIGHT_CYAN "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_LIGHT_RED "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_LIGHT_PURPLE "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_LIGHT_YELLOW "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_BG_WHITE "\n");
        printf(SHELL_TEXT_BLACK "Hello World\n");
        printf(SHELL_TEXT_BLUE "Hello World\n");
        printf(SHELL_TEXT_GREEN "Hello World\n");
        printf(SHELL_TEXT_CYAN "Hello World\n");
        printf(SHELL_TEXT_RED "Hello World\n");
        printf(SHELL_TEXT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREY "Hello World\n");
        printf(SHELL_TEXT_DARK_GREY "Hello World\n");
        printf(SHELL_TEXT_LIGHT_BLUE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_GREEN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_CYAN "Hello World\n");
        printf(SHELL_TEXT_LIGHT_RED "Hello World\n");
        printf(SHELL_TEXT_LIGHT_PURPLE "Hello World\n");
        printf(SHELL_TEXT_LIGHT_YELLOW "Hello World\n");
        printf(SHELL_TEXT_WHITE "Hello World\n");

        printf(SHELL_DEFAULT "\n");

        printf(SHELL_UNDERLINE "Hello World\n");

        printf(SHELL_DEFAULT "\n");

        printf(SHELL_BLINK "Hello World\n");

        printf(SHELL_DEFAULT "\n");

        printf(SHELL_INVERSE "Hello World\n");

        printf(SHELL_DEFAULT "\n");

        printf(SHELL_CONCEALED "Hello World\n");

        printf(SHELL_DEFAULT "\n");
    }

    return this;
}

void Output::printTerminate() {
    if (this->doNCurses) {
        endwin();
    }

    printf(SHELL_BG_BLACK "\n");
    printf(SHELL_TEXT_RED "Forceful termination\n");
    printf(SHELL_DEFAULT "\n");
    exit(1);
}

Output* Output::printError(std::string* format, ...) {
    if (this->doNCurses) {
        // If ncurses, clear the screen
        clear();
    }

    // Pull a list of args
    va_list args;
    va_start(args, format);

    if (this->doNCurses) {
        vw_printw(this->window, format->c_str(), args);
        vw_printw(this->window, "Press any key to continue...\n", null);
        refresh();

        getch();
    } else {
        printf(SHELL_TEXT_RED "");
        vprintf(format->c_str(), args);
        printf(SHELL_DEFAULT "\n");
    }

    va_end(args);

    return this;
}

Output* Output::printError(const char* format, ...) {
    if (this->doNCurses) {
        // If ncurses, clear the screen
        clear();
    }

    // Pull a list of args
    va_list args;
    va_start(args, format);

    if (this->doNCurses) {
        vw_printw(this->window, format, args);
        vw_printw(this->window, "Press any key to continue...\n", null);
        refresh();

        getch();
    } else {
        printf(SHELL_TEXT_RED "");
        vprintf(format, args);
        printf(SHELL_DEFAULT "\n");
    }

    va_end(args);

    return this;
}