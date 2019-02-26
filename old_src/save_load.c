#include "save_load.h"

int save_to_file(Dungeon* dungeon) {
    if (dungeon->settings->doSave) {
        if (dungeon->settings->file_version == 1) {
            return file_save_1(dungeon);
        } else {
            return file_save_0(dungeon);
        }
    }

    return 0;
}

void save_error(Dungeon* dungeon) {
    bool ncurses = dungeon->settings->doNCursesPrint;

    print(ncurses, "Failed to save file\n");
    print(ncurses, "Attempting to restore original file contents...\n");
    char* tempName = malloc((strlen(dungeon->settings->savePath) * sizeof(char)) + (strlen(".tmp") * sizeof(char)) + 1);
    sprintf(tempName, "%s%s", dungeon->settings->savePath, ".tmp");
    rename(tempName, dungeon->settings->savePath);
    free(tempName);
    print(ncurses, "File contents should have been restored\n");
}

int load_from_file(Dungeon* dungeon) {
    if (dungeon->settings->doLoad) {
        if (dungeon->settings->file_version == 1) {
            return file_load_1(dungeon);
        } else {
            return file_load_0(dungeon);
        }
    }

    return 0;
}

int load_from_program(Dungeon* dungeon) {
    dungeon->floorCount = randomNumberBetween(DUNGEON_FLOORS_MIN, DUNGEON_FLOORS_MAX);
    u_short stairCount = randomNumberBetween(FLOOR_STAIRS_MIN, FLOOR_STAIRS_MAX);

    // This creates a dynamically sized array. Going to need to do that because of load and save features later
    u_char index;
    dungeon->floors = malloc(dungeon->floorCount * sizeof(Floor*));
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon->floors[index] = floor_initialize(dungeon, index, stairCount, stairCount);
        if (floor_generate_monsters(dungeon->floors[index]) != 0) {
            print_error(dungeon->settings->doNCursesPrint, "Failed to generate monsters\n");
            return 1;
        }
    }
    dungeon->floor = dungeon->floors[0];

    // Select random room and random coords and place character there
    u_char playerX;
    u_char playerY;
    u_short room = randomNumberBetween(0, dungeon->floor->roomCount - 1);
    do {
        playerX = randomNumberBetween(dungeon->floor->rooms[room]->startX, dungeon->floor->rooms[room]->startX + dungeon->floor->rooms[room]->width - 1);
        playerY = randomNumberBetween(dungeon->floor->rooms[room]->startY, dungeon->floor->rooms[room]->startY + dungeon->floor->rooms[room]->height - 1);
    } while (dungeon->floor->floorPlan[playerY][playerX]->character != ROOM_CHARACTER);

    dungeon->player = player_initialize(dungeon, playerX, playerY, dungeon->floor);

    return 0;
}