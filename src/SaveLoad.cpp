#include "SaveLoad.h"

int save_to_file(Dungeon* dungeon) {
    return 0;
//    switch (dungeon->getSettings()->getFileVersion()) {
//        case 1:
//            file_save_1(dungeon);
//            break;
//        case 0:
//        default:
//            file_save_0(dungeon);
//            break;
//    }
}

int load_from_file(Dungeon* dungeon) {
    return 0;
//    switch (dungeon->getSettings()->getFileVersion()) {
//        case 1:
//            file_load_1(dungeon);
//            break;
//        case 0:
//        default:
//            file_load_0(dungeon);
//            break;
//    }
}

void save_error(Dungeon* dungeon) {
    dungeon->getOutput()->printError("Failed to save file\n");
    dungeon->getOutput()->printError("Attempting to restore original file contents...\n");
    std::string tempName = *dungeon->getSettings()->getSavePath() + std::string(".tmp");
    std::rename(tempName.c_str(), dungeon->getSettings()->getSavePath()->c_str());
    dungeon->getOutput()->printError("File contents should have been restored\n");
}