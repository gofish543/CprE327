#include "dungeon.h"

Dungeon* dungeon_initialize(Settings* settings) {
    Dungeon* dungeon = malloc(sizeof(Dungeon));
    dungeon->settings = settings;
    dungeon->eventManager = event_manager_initialize(dungeon);

    dungeon->textLine1 = malloc(sizeof(char) * FLOOR_WIDTH + 1);
    dungeon->textLine1[0] = '\0';
    dungeon->textLine2 = malloc(sizeof(char) * FLOOR_WIDTH + 1);
    dungeon->textLine2[0] = '\0';
    dungeon->textLine3 = malloc(sizeof(char) * FLOOR_WIDTH + 1);
    dungeon->textLine3[0] = '\0';

    // Check if loading from file or from program
    if (dungeon->settings->doLoad) {
        if (load_from_file(dungeon)) {
            printf("Failed to generate dungeon from file specified\n");
            exit(1);
            return null;
        }
    }
        // Else, load the the game from a program
    else if (load_from_program(dungeon)) {
        printf("Failed to generate a dungeon programmatically\n");
        exit(1);
        return null;
    }

    // Run dijkstra on all floors
    if (monster_run_dijkstra_on_all_floors(dungeon)) {
        printf("Failed to generate monster fastest paths\n");
        exit(1);
        return null;
    }

    // After everything is setup, add the user's movement to the event queue and add
    event_initialize(dungeon->eventManager, 0, type_player, dungeon->player, player_event);
    u_short index;
    for (index = 0; index < dungeon->floors[dungeon->currentFloor]->monsterCount; index++) {
        event_initialize(dungeon->eventManager, index + 1, type_monster, dungeon->floors[dungeon->currentFloor]->monsters[index], monster_event);
    }
    event_increment_tick(dungeon->eventManager, dungeon->floors[dungeon->currentFloor]->monsterCount + 1);

    return dungeon;
}

Dungeon* dungeon_terminate(Dungeon* dungeon) {
    dungeon->eventManager = event_manager_terminate(dungeon->eventManager);
    free(dungeon->textLine1);
    free(dungeon->textLine2);
    free(dungeon->textLine3);
    dungeon->textLine1 = null;
    dungeon->textLine2 = null;
    dungeon->textLine3 = null;

    // Free all monsters, lift them off dungeon floor
    monster_free_dungeon(dungeon);

    // Free player, lift them off dungeon floor
    player_free(dungeon->player);

    if (dungeon->settings->doSave) {
        if (save_to_file(dungeon)) {
            save_error(dungeon);
        }
    }

    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon->floors[index] = floor_terminate(dungeon->floors[index]);
    }
    dungeon->player = player_terminate(dungeon->player);

    dungeon->settings = null;
    free(dungeon->floors);
    free(dungeon);

    return null;
}

int dungeon_prepend_message(Dungeon* dungeon, const char* message) {
    strcpy(dungeon->textLine3, dungeon->textLine2);
    strcpy(dungeon->textLine2, dungeon->textLine1);
    strcpy(dungeon->textLine1, message);

    return 0;
}

int dungeon_append_message(Dungeon* dungeon, const char* message) {
    strcpy(dungeon->textLine1, dungeon->textLine2);
    strcpy(dungeon->textLine2, dungeon->textLine3);
    strcpy(dungeon->textLine3, message);

    return 0;
}