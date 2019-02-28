#include "escape_from_darkness.h"

int main(int argc, char* argv[]) {
    Dungeon* dungeon = null;

    if (initialize(&dungeon, argc, argv)) {
        print_error(dungeon->window, dungeon->settings->doNCursesPrint, "An error has occurred initializing the application, exiting\n");
        return 1;
    }
    dungeon->floor->characters[dungeon->player->character->y][dungeon->player->character->x] = null;

    dungeon->player->character->x = dungeon->floor->upStairs[0]->x;
    dungeon->player->character->y = dungeon->floor->upStairs[0]->y;
    dungeon->floor->characters[dungeon->player->character->y][dungeon->player->character->x] = dungeon->player->character;

    while (dungeon->player->isAlive && monster_alive_count(dungeon) && !dungeon->player->requestTerminate) {
        if (game_tick(dungeon)) {
            print_error(dungeon->window, dungeon->settings->doNCursesPrint, "Game tick error encountered, exiting while loop\n");
            break;
        }
    }
    output(dungeon, output_print_endgame);

    if (terminate(&dungeon)) {
        print_error(dungeon->window, dungeon->settings->doNCursesPrint, "An error has occurred terminating the application, exiting\n");
        return 1;
    }

    return 0;
}

int game_tick(Dungeon* dungeon) {
    if (event_peek_next(dungeon->eventManager)->type == event_type_player) {
        output(dungeon, output_print_current_floor);
    }

    event_handle_next(dungeon->eventManager);

    dungeon->player->daysSurvived++;

    // Check if a staircase is taken
    if (dungeon->player->takingStaircase != null) {
        staircase_take(dungeon, dungeon->player->takingStaircase);
        dungeon->player->takingStaircase = null;
    }

    return 0;
}

int initialize(Dungeon** dungeon, int argc, char* argv[]) {
    Settings* settings = settings_initialize(argc, argv);
    if (settings == null) {
        print_error((*dungeon)->window, (*dungeon)->settings->doNCursesPrint, "Failed to initialize settings\n");
        exit(1);
    }

    *dungeon = dungeon_initialize(settings);

    if (*dungeon == null) {
        print_error((*dungeon)->window, (*dungeon)->settings->doNCursesPrint, "Failed to initialize the dungeon\n");
        exit(1);
    }

    if (settings->doNCursesPrint) {
        (*dungeon)->window = initscr();
        raw();
        keypad(stdscr, true);
        noecho();
    }

    return 0;
}

int terminate(Dungeon** dungeon) {
    Settings* settings = (*dungeon)->settings;

    if (settings->doNCursesPrint) {
        endwin();
    }

    *dungeon = dungeon_terminate(*dungeon);

    settings = settings_terminate(settings);

    return 0;
}