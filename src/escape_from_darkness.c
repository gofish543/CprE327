#include "escape_from_darkness.h"

int main(int argc, char* argv[]) {
    int status = 0;
    Dungeon* dungeon = null;

    status = initialize(&dungeon, argc, argv);
    if (status) {
        printf("An error has occurred initializing the application, exiting\n");
        return status;
    }

    output_print_current_floor(dungeon);
    while (dungeon->player->isAlive && monster_count(dungeon) != 0 && dungeon->eventManager->queue->size != 0) {
        if (game_tick(dungeon)) {
            printf("Game tick error encountered, exiting while loop\n");
            break;
        }
        usleep(TIME_HALF_SECOND_MS);
    }
    output_print_endgame(dungeon);

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
    Event* event = event_peek_next(dungeon->eventManager);

    bool isPlayerTurn = event->type == type_player;

    if (isPlayerTurn) {
        output_print_current_floor(dungeon);
    }

    event_handle_next(dungeon->eventManager);
    event_increment_tick(dungeon->eventManager, 1);

    if (isPlayerTurn) {
        output_print_current_floor(dungeon);
        dungeon->player->tilesExplored++;
    }

    dungeon->player->daysSurvived++;
    return 0;
}