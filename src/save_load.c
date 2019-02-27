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

    print(dungeon->window, ncurses, "Failed to save file\n");
    print(dungeon->window, ncurses, "Attempting to restore original file contents...\n");
    char* tempName = malloc((strlen(dungeon->settings->savePath) * sizeof(char)) + (strlen(".tmp") * sizeof(char)) + 1);
    sprintf(tempName, "%s%s", dungeon->settings->savePath, ".tmp");
    rename(tempName, dungeon->settings->savePath);
    free(tempName);
    print(dungeon->window, ncurses, "File contents should have been restored\n");
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
    dungeon->floorCount = random_number_between(DUNGEON_FLOORS_MIN, DUNGEON_FLOORS_MAX);
    u_short stairCount = random_number_between(FLOOR_STAIRS_MIN, FLOOR_STAIRS_MAX);
    u_short monsterCount;
    u_short roomCount;

    // This creates a dynamically sized array. Going to need to do that because of load and save features later
    u_char index;
    dungeon->floors = malloc(dungeon->floorCount * sizeof(Floor*));
    for (index = 0; index < dungeon->floorCount; index++) {
        monsterCount = random_number_between(FLOOR_MONSTERS_MIN, FLOOR_MONSTERS_MAX);
        roomCount = random_number_between(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX);
        dungeon->floors[index] = floor_initialize(dungeon, index, roomCount, stairCount, stairCount, monsterCount);
    }
    dungeon->floor = dungeon->floors[0];

    // Select random room and random coords and place character there
    u_char playerX;
    u_char playerY;
    u_short room = random_number_between(0, dungeon->floor->roomCount - 1);
    do {
        playerX = random_number_between(dungeon->floor->rooms[room]->startX, dungeon->floor->rooms[room]->startX + dungeon->floor->rooms[room]->width - 1);
        playerY = random_number_between(dungeon->floor->rooms[room]->startY, dungeon->floor->rooms[room]->startY + dungeon->floor->rooms[room]->height - 1);
    } while (dungeon->floor->terrains[playerY][playerX]->staircase != null);

    dungeon->player = player_initialize(dungeon->floor, playerX, playerY);
    dungeon->floor->characters[playerY][playerX] = dungeon->player->character;

    for(index = 0; index < dungeon->floorCount; index++) {
        if (floor_generate_monsters(dungeon->floors[index])) {
            print_error(dungeon->window, dungeon->settings->doNCursesPrint, "Failed to generate monsters\n");
            return 1;
        }
    }

    return 0;
}