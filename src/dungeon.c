#include "dungeon.h"

Dungeon* dungeon_initialize(Settings* settings) {
    Dungeon* dungeon = malloc(sizeof(Dungeon));
    dungeon->settings = settings;

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

    return dungeon;
}

Dungeon* dungeon_terminate(Dungeon* dungeon) {
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