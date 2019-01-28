#include "dungeon.h"

Dungeon* dungeon_initialize(Settings* settings) {
    Dungeon* dungeon = malloc(sizeof(Dungeon));
    dungeon->settings = settings;

    // If failed loaded from file load up a new map
    if (dungeon->settings->doLoad) {
        bool loadedSuccessfully = dungeon_load_from_file(dungeon);

        if (!loadedSuccessfully) {
            return NULL;
        }
    } else {
        dungeon->currentFloor = 0;
        dungeon->floorCount = randomNumberBetween(DUNGEON_FLOORS_MIN, DUNGEON_FLOORS_MAX);
        u_char stairUpCount = randomNumberBetween(FLOOR_STAIRS_MIN, FLOOR_STAIRS_MAX);
        u_char stairDownCount = stairDownCount;

        // This creates a dynamically sized array. Going to need to do that because of load and save features later
        dungeon->floors = malloc(dungeon->floorCount * sizeof(Floor*));
        u_char index;
        for (index = 0; index < dungeon->floorCount; index++) {
            dungeon->floors[index] = floor_initialize(index, dungeon->floorCount, stairUpCount, stairDownCount);
        }

        dungeon_place_character(dungeon);
    }

    return dungeon;
}

Dungeon* dungeon_terminate(Dungeon* dungeon) {
    dungeon_save_to_file(dungeon);

    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon->floors[index] = floor_terminate(dungeon->floors[index]);
    }
    dungeon->player = player_terminate(dungeon->player);


    if (dungeon->settings->doSave) {
        free(dungeon->settings->savePath);
    }

    if (dungeon->settings->doLoad) {
        free(dungeon->settings->loadPath);
    }

    free(dungeon->settings);
    free(dungeon->floors);
    free(dungeon);

    return NULL;
}

void dungeon_place_character(Dungeon* dungeon) {
    // Select random room and random coords and place character there
    u_char playerX;
    u_char playerY;
    u_char room = randomNumberBetween(0, dungeon->floors[dungeon->currentFloor]->roomCount - 1);
    do {
        playerX = randomNumberBetween(dungeon->floors[dungeon->currentFloor]->rooms[room]->startX, dungeon->floors[dungeon->currentFloor]->rooms[room]->startX + dungeon->floors[dungeon->currentFloor]->rooms[room]->width - 1);
        playerY = randomNumberBetween(dungeon->floors[dungeon->currentFloor]->rooms[room]->startY, dungeon->floors[dungeon->currentFloor]->rooms[room]->startY + dungeon->floors[dungeon->currentFloor]->rooms[room]->height - 1);
    } while (dungeon->floors[dungeon->currentFloor]->floorPlan[playerY][playerX]->character != ROOM_CHARACTER);

    dungeon->player = player_initialize(playerX, playerY, dungeon->currentFloor);
    dungeon->floors[dungeon->currentFloor]->floorPlan[playerY][playerX]->character = PLAYER_CHARACTER;
    dungeon->floors[dungeon->currentFloor]->floorPlan[playerY][playerX]->hardness = 0;
    dungeon->floors[dungeon->currentFloor]->floorPlan[playerY][playerX]->type = type_player;
    dungeon->floors[dungeon->currentFloor]->floorPlan[playerY][playerX]->internalObject = dungeon->player;
}

void print_current_floor(Dungeon* dungeon) {
    u_char width;
    u_char height;

    for (height = 0; height < dungeon->floors[dungeon->currentFloor]->height; height++) {
        for (width = 0; width < dungeon->floors[dungeon->currentFloor]->width; width++) {
            printf("%c", dungeon->floors[dungeon->currentFloor]->floorPlan[height][width]->character);
        }
        printf("\n");
    }
}