#include "escape_from_darkness.h"

int main(int argc, char* argv[]) {
    Dungeon* dungeon = NULL;

    initialize(&dungeon);

    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon->currentFloor = index;
        print_current_floor(dungeon);
    }

    terminate(&dungeon);
    return 0;
}

void initialize(Dungeon** dungeon) {
    *dungeon = dungeon_initialize();
}

void terminate(Dungeon** dungeon) {
    *dungeon = dungeon_terminate(*dungeon);
}