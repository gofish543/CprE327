#include "save_load.h"

int save_to_file(Dungeon* dungeon) {
    if (dungeon->getSettings()->getFileVersion() == 1) {
        return file_save_1(dungeon);
    } else {
        return file_save_0(dungeon);
    }
}

void save_error(Dungeon* dungeon) {
    bool ncurses = dungeon->getSettings()->doNCursesPrint();

    print(dungeon->getWindow(), ncurses, "Failed to save file\n");
    print(dungeon->getWindow(), ncurses, "Attempting to restore original file contents...\n");
    std::string tempName = dungeon->getSettings()->getSavePath() + ".tmp";
    std::rename(tempName.c_str(), dungeon->getSettings()->getSavePath().c_str());
    print(dungeon->getWindow(), ncurses, "File contents should have been restored\n");
}

int load_from_file(Dungeon* dungeon) {
    if (dungeon->getSettings()->getFileVersion() == 1) {
        return file_load_1(dungeon);
    } else {
        return file_load_0(dungeon);
    }
}