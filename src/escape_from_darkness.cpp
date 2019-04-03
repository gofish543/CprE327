#include "escape_from_darkness.h"

int main(int argc, char* argv[]) {
    auto dungeon = new Dungeon(argc, argv);

    while (dungeon->getPlayer()->getIsAlive() && Monster::AliveCount(dungeon) && !dungeon->getPlayer()->getRequestTerminate() && dungeon->getEventManager()->getQueue()->size > 0) {
        if (game_tick(dungeon)) {
            dungeon->getOutput()->printError("Game tick error encountered, exiting while loop\n");
            break;
        }
    }
    dungeon->getOutput()->printEndgame();

    delete (dungeon);

    return 0;
}

int game_tick(Dungeon* dungeon) {
    if (dungeon->getEventManager()->peekNext()->getType() == event_type_player) {
        dungeon->getOutput()->print();
    }

    dungeon->getEventManager()->handleNext();

    dungeon->getPlayer()->incrementDaysSurvived();

    if (dungeon->getPlayer()->getTakingStaircase() != null) {
        dungeon->getPlayer()->getTakingStaircase()->take();
        dungeon->getPlayer()->setTakingStaircase(null);
    }

    return 0;
}