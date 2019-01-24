#include "escape_from_darkness.h"

int main(int argc, char* argv[]) {
    Dungeon* dungeon = NULL;

    initialize(&dungeon);

    print_current_floor(dungeon);

    terminate(&dungeon);
    return 0;
}

void initialize(Dungeon** dungeon) {
    *dungeon = dungeon_initialize();
}

void terminate(Dungeon** dungeon) {
    *dungeon = dungeon_terminate(*dungeon);
}