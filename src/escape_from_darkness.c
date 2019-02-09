#include "escape_from_darkness.h"


int main(int argc, char* argv[]) {
    int status = 0;
    Dungeon* dungeon = NULL;

    status = initialize(&dungeon, argc, argv);
    if (status != 0) {
        printf("An error has occurred initializing the application, exiting\n");
        return status;
    }

    while (true) {
        game_tick(dungeon);
        sleep(2);
    }

    status = terminate(&dungeon);
    if (status != 0) {
        printf("An error has occurred terminating the application, exiting\n");
        return status;
    }

    return 0;
}

int initialize(Dungeon** dungeon, int argc, char* argv[]) {
    Settings* settings = settings_initialize(argc, argv);
    if (settings == NULL) {
        return 1;
    }

    *dungeon = dungeon_initialize(settings);

    if (*dungeon == NULL) {
        return 1;
    }

    return 0;
}

int terminate(Dungeon** dungeon) {
    Settings* settings = (*dungeon)->settings;

    *dungeon = dungeon_terminate(*dungeon);

    settings = settings_terminate(settings);

    return 0;
}

void game_tick(Dungeon* dungeon) {
    dungeon_print_current_floor(dungeon);
//    dungeon_print_current_floor_hardness(dungeon);
//    dungeon_print_current_floor_tunneler_view(dungeon);
//    dungeon_print_current_floor_non_tunneler_view(dungeon);

    // Move all monsters on the current floor
    monsters_move(dungeon->floors[dungeon->currentFloor]);
    // Rerun dijkstras for the current floor
    monster_dijkstra_tunneler(dungeon->floors[dungeon->currentFloor]);
    monster_dijkstra_non_tunneler(dungeon->floors[dungeon->currentFloor]);
}