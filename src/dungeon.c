#include "dungeon.h"

Dungeon* dungeon_initialize(Settings* settings) {
    Dungeon* dungeon = malloc(sizeof(Dungeon));
    dungeon->settings = settings;

    // Check if loading from file or from program
    if (dungeon->settings->doLoad) {
        if (load_from_file(dungeon) != 0) {
            load_error();
            return NULL;
        }
    } else {
        if (dungeon_load_from_program(dungeon) != 0) {
            return NULL;
        }
    }

    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        monster_dijkstra_tunneler(dungeon->floors[index]);
        monster_dijkstra_non_tunneler(dungeon->floors[index]);
    }

    return dungeon;
}

Dungeon* dungeon_terminate(Dungeon* dungeon) {
    if (dungeon->settings->doSave) {
        if (save_to_file(dungeon) != 0) {
            save_error(dungeon);
        }
    }

    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon->floors[index] = floor_terminate(dungeon->floors[index]);
    }
    dungeon->player = player_terminate(dungeon->player);

    dungeon->settings = NULL;
    free(dungeon->floors);
    free(dungeon);

    return NULL;
}

int dungeon_load_from_program(Dungeon* dungeon) {
    dungeon->currentFloor = 0;
    dungeon->floorCount = randomNumberBetween(DUNGEON_FLOORS_MIN, DUNGEON_FLOORS_MAX);
    u_char stairCount = randomNumberBetween(FLOOR_STAIRS_MIN, FLOOR_STAIRS_MAX);

    // This creates a dynamically sized array. Going to need to do that because of load and save features later
    dungeon->floors = malloc(dungeon->floorCount * sizeof(Floor*));
    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon->floors[index] = floor_initialize(dungeon, index, stairCount, stairCount);
        if (floor_generate_monsters(dungeon->floors[index]) != 0) {
            printf("Failed to generate monsters\n");
            return 1;
        }
    }

    dungeon_place_character(dungeon);

    return 0;
}

void dungeon_place_character(Dungeon* dungeon) {
    // Select random room and random coords and place character there
    u_char playerX;
    u_char playerY;
    u_short room = randomNumberBetween(0, dungeon->floors[dungeon->currentFloor]->roomCount - 1);
    do {
        playerX = randomNumberBetween(dungeon->floors[dungeon->currentFloor]->rooms[room]->startX, dungeon->floors[dungeon->currentFloor]->rooms[room]->startX + dungeon->floors[dungeon->currentFloor]->rooms[room]->width - 1);
        playerY = randomNumberBetween(dungeon->floors[dungeon->currentFloor]->rooms[room]->startY, dungeon->floors[dungeon->currentFloor]->rooms[room]->startY + dungeon->floors[dungeon->currentFloor]->rooms[room]->height - 1);
    } while (dungeon->floors[dungeon->currentFloor]->floorPlan[playerY][playerX]->character != ROOM_CHARACTER);

    dungeon->player = player_initialize(dungeon, playerX, playerY, dungeon->currentFloor);
    dungeon->floors[dungeon->currentFloor]->floorPlan[playerY][playerX]->character = PLAYER_CHARACTER;
    dungeon->floors[dungeon->currentFloor]->floorPlan[playerY][playerX]->hardness = 0;
    dungeon->floors[dungeon->currentFloor]->floorPlan[playerY][playerX]->type = type_player;
}

void dungeon_print_current_floor(Dungeon* dungeon) {
    dungeon_print_floor(dungeon, dungeon->currentFloor);
}

void dungeon_print_current_floor_hardness(Dungeon* dungeon) {
    dungeon_print_floor_hardness(dungeon, dungeon->currentFloor);
}

void dungeon_print_current_floor_tunneler_view(Dungeon* dungeon) {
    dungeon_print_floor_tunneler_view(dungeon, dungeon->currentFloor);
}

void dungeon_print_current_floor_non_tunneler_view(Dungeon* dungeon) {
    dungeon_print_floor_non_tunneler_view(dungeon, dungeon->currentFloor);
}

void dungeon_print_floor(Dungeon* dungeon, u_char floor) {
    u_char width;
    u_char height;

    printf("  ");
    for (width = 0; width < dungeon->floors[floor]->width; width++) {
        printf("%3d", width);
    }
    printf("\n");

    for (height = 0; height < dungeon->floors[floor]->height; height++) {
        printf("%2d ", height);
        for (width = 0; width < dungeon->floors[floor]->width; width++) {
            printf(" %c ", dungeon->floors[floor]->floorPlan[height][width]->character);
        }
        printf("\n");
    }
}

void dungeon_print_floor_hardness(Dungeon* dungeon, u_char floor) {
    u_char width;
    u_char height;

    printf("   ");
    for (width = 0; width < dungeon->floors[floor]->width; width++) {
        printf("%3d", width);
    }
    printf("\n");

    for (height = 0; height < dungeon->floors[floor]->height; height++) {
        printf("%2d ", height);
        for (width = 0; width < dungeon->floors[floor]->width; width++) {
            printf("%3d", dungeon->floors[floor]->floorPlan[height][width]->hardness);
        }
        printf("\n");
    }
}

void dungeon_print_floor_tunneler_view(Dungeon* dungeon, u_char floor) {
    u_char width;
    u_char height;

    printf("   ");
    for (width = 0; width < dungeon->floors[floor]->width; width++) {
        printf("%3d", width);
    }
    printf("\n");

    for (height = 0; height < dungeon->floors[floor]->height; height++) {
        printf("%2d ", height);
        for (width = 0; width < dungeon->floors[floor]->width; width++) {
            printf("%3d", dungeon->floors[floor]->tunnelerCost[height][width]);
        }
        printf("\n");
    }
}

void dungeon_print_floor_non_tunneler_view(Dungeon* dungeon, u_char floor) {
    u_char width;
    u_char height;

    printf("   ");
    for (width = 0; width < dungeon->floors[floor]->width; width++) {
        printf("%3d", width);
    }
    printf("\n");

    for (height = 0; height < dungeon->floors[floor]->height; height++) {
        printf("%2d ", height);
        for (width = 0; width < dungeon->floors[floor]->width; width++) {
            if (dungeon->floors[floor]->nonTunnelerCost[height][width] == BORDER_HARDNESS) {
                printf("   ");
            } else {
                printf("%3d", dungeon->floors[floor]->nonTunnelerCost[height][width]);
            }
        }
        printf("\n");
    }
}

void dungeon_print_all_floors(Dungeon* dungeon) {
    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon_print_floor(dungeon, index);
    }
}