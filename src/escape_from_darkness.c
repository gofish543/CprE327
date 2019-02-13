#include "escape_from_darkness.h"

int main(int argc, char* argv[]) {
    int status = 0;
    Dungeon* dungeon = null;

    status = initialize(&dungeon, argc, argv);
    if (status) {
        printf("An error has occurred initializing the application, exiting\n");
        return status;
    }

//    output_print_current_floor(dungeon);
    while (true) {
        if (game_tick(dungeon)) {
            printf("Game tick error encountered, exiting while loop\n");
            break;
        }
        sleep(1);
        break;
    }

    status = terminate(&dungeon);
    if (status) {
        printf("An error has occurred terminating the application, exiting\n");
        return status;
    }

    return 0;
}

int initialize(Dungeon** dungeon, int argc, char* argv[]) {
    Settings* settings = settings_initialize(argc, argv);
    if (settings == null) {
        printf("Failed to initialize settings\n");
        exit(1);
    }

    *dungeon = dungeon_initialize(settings);

    if (*dungeon == null) {
        printf("Failed to initialize the dungeon\n");
        exit(1);
    }

    return 0;
}

int terminate(Dungeon** dungeon) {
    Settings* settings = (*dungeon)->settings;

    *dungeon = dungeon_terminate(*dungeon);

    settings = settings_terminate(settings);

    return 0;
}

int game_tick(Dungeon* dungeon) {
    // Move all monsters on the current floor
//    monsters_move(dungeon->floors[dungeon->currentFloor]);
    // Rerun dijkstras for the current floor
//    monster_run_dijkstra_on_floor(dungeon->floors[dungeon->currentFloor]);

    printf("----- CURRENT FLOOR -----\n");
    output_print_current_floor(dungeon);
    printf("----- HARDNESS CHART -----\n");
    output_print_current_floor_hardness(dungeon);
    printf("----- TUNNELER CHART -----\n");
    output_print_current_floor_tunneler_view(dungeon);
    printf("----- NON TUNNELER CHART -----\n");
    output_print_current_floor_non_tunneler_view(dungeon);
    return 0;
}