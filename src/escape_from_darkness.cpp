#include "escape_from_darkness.h"

int main(int argc, char* argv[]) {
    auto dungeon = new Dungeon(argc, argv);

    while (dungeon->continueGame()) {
        dungeon->getEventManager()->handleNext();
        dungeon->getPlayer()->incrementDaysSurvived();

        if(dungeon->getPlayer()->getTakingStaircase() != null) {
            dungeon->getPlayer()->getTakingStaircase()->take();
            dungeon->getPlayer()->setTakingStaircase(null);
        }
    }
    dungeon->getOutput()->printEndgame();

    delete (dungeon);

    return 0;
}
