#include "save_load.h"

int save_to_file(Dungeon* dungeon) {
    switch (dungeon->getSettings()->getFileVersion()) {
        case 0:
        default:
            return file_save_0(dungeon);
    }
}

int load_from_file(Dungeon* dungeon) {
    switch (dungeon->getSettings()->getFileVersion()) {
        case 0:
        default:
            return file_load_0(dungeon);
    }
}

void save_error(Dungeon* dungeon) {
    dungeon->getOutput()->printError("Failed to save file\n");
    dungeon->getOutput()->printError("Attempting to restore original file contents...\n");
    std::string tempName = *dungeon->getSettings()->getSavePath() + std::string(".tmp");
    std::rename(tempName.c_str(), dungeon->getSettings()->getSavePath()->c_str());
    dungeon->getOutput()->printError("File contents should have been restored\n");
}