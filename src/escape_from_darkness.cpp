#include "escape_from_darkness.h"

int main(int argc, char* argv[]) {
    Dungeon dungeon(argc, argv);

    output(&dungeon, output_print_current_floor_hardness);

    return 0;
}