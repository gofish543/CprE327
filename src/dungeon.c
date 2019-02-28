#include "dungeon.h"

Dungeon* dungeon_initialize(Settings* settings) {
    u_short index;

    Dungeon* dungeon = malloc(sizeof(Dungeon));

    dungeon->settings = settings;
    dungeon->eventManager = event_manager_initialize(dungeon);

    dungeon->textLine1 = malloc(sizeof(u_char) * (FLOOR_WIDTH + 1));
    dungeon->textLine2 = malloc(sizeof(u_char) * (FLOOR_WIDTH + 1));
    dungeon->textLine3 = malloc(sizeof(u_char) * (FLOOR_WIDTH + 1));
    for (index = 0; index < FLOOR_WIDTH + 1; index++) {
        dungeon->textLine1[index] = '\0';
        dungeon->textLine2[index] = '\0';
        dungeon->textLine3[index] = '\0';
    }

    // Check if loading from file or from program
    if (dungeon->settings->doLoad) {
        if (load_from_file(dungeon)) {
            print_error(dungeon->window, dungeon->settings->doNCursesPrint, "Failed to generate dungeon from file specified\n");
            exit(1);
            return null;
        }
    } else if (load_from_program(dungeon)) {
        print_error(dungeon->window, dungeon->settings->doNCursesPrint, "Failed to generate a dungeon programmatically\n");
        exit(1);
        return null;
    }

    // Run dijkstra on current floor
    if (monster_run_dijkstra_on_floor(dungeon->floor)) {
        print_error(dungeon->window, dungeon->settings->doNCursesPrint, "Failed to generate monster fastest paths\n");
        exit(1);
        return null;
    }

    // Add the player action to the event queue
    event_initialize(dungeon->eventManager, 0, event_type_player, dungeon->player, player_event, player_next_tick);
    for (index = 0; index < dungeon->floor->monsterCount; index++) {
        event_initialize(dungeon->eventManager, dungeon->eventManager->tick, event_type_monster, dungeon->floor->monsters[index], monster_event, monster_next_tick);
    }

    return dungeon;
}

Dungeon* dungeon_terminate(Dungeon* dungeon) {
    // Attempt to save entire game
    if (dungeon->settings->doSave && save_to_file(dungeon)) {
        save_error(dungeon);
    }

    free(dungeon->textLine1);
    free(dungeon->textLine2);
    free(dungeon->textLine3);
    dungeon->textLine1 = null;
    dungeon->textLine2 = null;
    dungeon->textLine3 = null;

    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon->floors[index] = floor_terminate(dungeon->floors[index]);
    }
    dungeon->player = player_terminate(dungeon->player);
    dungeon->eventManager = event_manager_terminate(dungeon->eventManager);
    dungeon->settings = null;

    free(dungeon->floors);
    free(dungeon);

    return null;
}

int dungeon_prepend_message(Dungeon* dungeon, const char* message, ...) {
    va_list args;
    va_start(args, message);

    strcpy(dungeon->textLine3, dungeon->textLine2);
    strcpy(dungeon->textLine2, dungeon->textLine1);
    vsprintf(dungeon->textLine1, message, args);

    va_end(args);

    return 0;
}

int dungeon_append_message(Dungeon* dungeon, const char* message, ...) {
    va_list args;
    va_start(args, message);

    strcpy(dungeon->textLine1, dungeon->textLine2);
    strcpy(dungeon->textLine2, dungeon->textLine3);
    vsprintf(dungeon->textLine3, message, args);

    va_end(args);

    return 0;
}